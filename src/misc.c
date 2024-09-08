/* Copyright (c) Mark Harmstone 2020
 *
 * This file is part of btrfs-efi.
 *
 * btrfs-efi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public Licence as published by
 * the Free Software Foundation, either version 3 of the Licence, or
 * (at your option) any later version.
 *
 * btrfs-efi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 *
 * You should have received a copy of the GNU Lesser General Public Licence
 * along with btrfs-efi.  If not, see <http://www.gnu.org/licenses/>. */

#include "misc.h"
#include <stdbool.h>

size_t wcslen(const wchar_t* s) {
    size_t i = 0;

    while (s[i] != 0) {
        i++;
    }

    return i;
}

size_t strlen(const char* s) {
    size_t i = 0;

    while (s[i] != 0) {
        i++;
    }

    return i;
}

int memcmp(const void* s1, const void* s2, size_t n) {
#if __INTPTR_WIDTH__ == 64
    while (n > sizeof(uint64_t)) {
        uint64_t c1 = *(uint64_t*)s1;
        uint64_t c2 = *(uint64_t*)s2;

        if (c1 != c2)
            return c1 > c2 ? 1 : -1;

        s1 = (uint64_t*)s1 + 1;
        s2 = (uint64_t*)s2 + 1;
        n -= sizeof(uint64_t);
    }
#endif

    while (n > sizeof(uint32_t)) {
        uint32_t c1 = *(uint32_t*)s1;
        uint32_t c2 = *(uint32_t*)s2;

        if (c1 != c2)
            return c1 > c2 ? 1 : -1;

        s1 = (uint32_t*)s1 + 1;
        s2 = (uint32_t*)s2 + 1;
        n -= sizeof(uint32_t);
    }

    while (n > 0) {
        uint8_t c1 = *(uint8_t*)s1;
        uint8_t c2 = *(uint8_t*)s2;

        if (c1 != c2)
            return c1 > c2 ? 1 : -1;

        s1 = (uint8_t*)s1 + 1;
        s2 = (uint8_t*)s2 + 1;
        n--;
    }

    return 0;
}

char* strcpy(char* dest, const char* src) {
    char* orig_dest = dest;

    while (*src != 0) {
        *dest = *src;
        src++;
        dest++;
    }

    *dest = 0;

    return orig_dest;
}

EFI_STATUS utf8_to_utf16(wchar_t* dest, unsigned int dest_max, unsigned int* dest_len, const char* src, unsigned int src_len) {
    EFI_STATUS Status = EFI_SUCCESS;
    uint8_t* in = (uint8_t*)src;
    uint16_t* out = (uint16_t*)dest;
    unsigned int needed = 0, left = dest_max / sizeof(uint16_t);

    for (unsigned int i = 0; i < src_len; i++) {
        uint32_t cp;

        if (!(in[i] & 0x80))
            cp = in[i];
        else if ((in[i] & 0xe0) == 0xc0) {
            if (i == src_len - 1 || (in[i+1] & 0xc0) != 0x80) {
                cp = 0xfffd;
                Status = EFI_INVALID_PARAMETER;
            } else {
                cp = ((in[i] & 0x1f) << 6) | (in[i+1] & 0x3f);
                i++;
            }
        } else if ((in[i] & 0xf0) == 0xe0) {
            if (i >= src_len - 2 || (in[i+1] & 0xc0) != 0x80 || (in[i+2] & 0xc0) != 0x80) {
                cp = 0xfffd;
                Status = EFI_INVALID_PARAMETER;
            } else {
                cp = ((in[i] & 0xf) << 12) | ((in[i+1] & 0x3f) << 6) | (in[i+2] & 0x3f);
                i += 2;
            }
        } else if ((in[i] & 0xf8) == 0xf0) {
            if (i >= src_len - 3 || (in[i+1] & 0xc0) != 0x80 || (in[i+2] & 0xc0) != 0x80 || (in[i+3] & 0xc0) != 0x80) {
                cp = 0xfffd;
                Status = EFI_INVALID_PARAMETER;
            } else {
                cp = ((in[i] & 0x7) << 18) | ((in[i+1] & 0x3f) << 12) | ((in[i+2] & 0x3f) << 6) | (in[i+3] & 0x3f);
                i += 3;
            }
        } else {
            cp = 0xfffd;
            Status = EFI_INVALID_PARAMETER;
        }

        if (cp > 0x10ffff) {
            cp = 0xfffd;
            Status = EFI_INVALID_PARAMETER;
        }

        if (dest) {
            if (cp <= 0xffff) {
                if (left < 1)
                    return EFI_BUFFER_TOO_SMALL;

                *out = (uint16_t)cp;
                out++;

                left--;
            } else {
                if (left < 2)
                    return EFI_BUFFER_TOO_SMALL;

                cp -= 0x10000;

                *out = 0xd800 | ((cp & 0xffc00) >> 10);
                out++;

                *out = 0xdc00 | (cp & 0x3ff);
                out++;

                left -= 2;
            }
        }

        if (cp <= 0xffff)
            needed += sizeof(uint16_t);
        else
            needed += 2 * sizeof(uint16_t);
    }

    if (dest_len)
        *dest_len = needed;

    return Status;
}

EFI_STATUS utf16_to_utf8(char* dest, unsigned int dest_max, unsigned int* dest_len, const wchar_t* src, unsigned int src_len) {
    EFI_STATUS Status = EFI_SUCCESS;
    uint16_t* in = (uint16_t*)src;
    uint8_t* out = (uint8_t*)dest;
    unsigned int in_len = src_len / sizeof(uint16_t);
    unsigned int needed = 0, left = dest_max;

    for (unsigned int i = 0; i < in_len; i++) {
        uint32_t cp = *in;
        in++;

        if ((cp & 0xfc00) == 0xd800) {
            if (i == in_len - 1 || (*in & 0xfc00) != 0xdc00) {
                cp = 0xfffd;
                Status = EFI_INVALID_PARAMETER;
            } else {
                cp = (cp & 0x3ff) << 10;
                cp |= *in & 0x3ff;
                cp += 0x10000;

                in++;
                i++;
            }
        } else if ((cp & 0xfc00) == 0xdc00) {
            cp = 0xfffd;
            Status = EFI_INVALID_PARAMETER;
        }

        if (cp > 0x10ffff) {
            cp = 0xfffd;
            Status = EFI_INVALID_PARAMETER;
        }

        if (dest) {
            if (cp < 0x80) {
                if (left < 1)
                    return EFI_BUFFER_TOO_SMALL;

                *out = (uint8_t)cp;
                out++;

                left--;
            } else if (cp < 0x800) {
                if (left < 2)
                    return EFI_BUFFER_TOO_SMALL;

                *out = 0xc0 | ((cp & 0x7c0) >> 6);
                out++;

                *out = 0x80 | (cp & 0x3f);
                out++;

                left -= 2;
            } else if (cp < 0x10000) {
                if (left < 3)
                    return EFI_BUFFER_TOO_SMALL;

                *out = 0xe0 | ((cp & 0xf000) >> 12);
                out++;

                *out = 0x80 | ((cp & 0xfc0) >> 6);
                out++;

                *out = 0x80 | (cp & 0x3f);
                out++;

                left -= 3;
            } else {
                if (left < 4)
                    return EFI_BUFFER_TOO_SMALL;

                *out = 0xf0 | ((cp & 0x1c0000) >> 18);
                out++;

                *out = 0x80 | ((cp & 0x3f000) >> 12);
                out++;

                *out = 0x80 | ((cp & 0xfc0) >> 6);
                out++;

                *out = 0x80 | (cp & 0x3f);
                out++;

                left -= 4;
            }
        }

        if (cp < 0x80)
            needed++;
        else if (cp < 0x800)
            needed += 2;
        else if (cp < 0x10000)
            needed += 3;
        else
            needed += 4;
    }

    if (dest_len)
        *dest_len = needed;

    return Status;
}

char* stpcpy(char* dest, const char* src) {
    while (*src != 0) {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = 0;

    return dest;
}

char* hex_to_str(char* s, uint64_t v) {
    char *end, *p;

    if (v == 0) {
        *s = '0';
        s++;

        *s = 0;
        return s;
    }

    end = s;

    {
        uint64_t n = v;

        while (n != 0) {
            end++;
            n >>= 4;
        }
    }

    *end = 0;

    p = end;

    while (v != 0) {
        p = &p[-1];

        if ((v & 0xf) >= 10)
            *p = (v & 0xf) - 10 + 'a';
        else
            *p = (v & 0xf) + '0';

        v >>= 4;
    }

    return end;
}

char* dec_to_str(char* s, uint64_t v) {
    char *end, *p;

    if (v == 0) {
        *s = '0';
        s++;

        *s = 0;
        return s;
    }

    end = s;

    {
        uint64_t n = v;

        while (n != 0) {
            end++;
            n /= 10;
        }
    }

    *end = 0;

    p = end;

    while (v != 0) {
        p = &p[-1];
        *p = (v % 10) + '0';

        v /= 10;
    }

    return end;
}

const char* error_string(EFI_STATUS Status) {
    switch (Status) {
        case EFI_SUCCESS:
            return "EFI_SUCCESS";

        case EFI_LOAD_ERROR:
            return "EFI_LOAD_ERROR";

        case EFI_INVALID_PARAMETER:
            return "EFI_INVALID_PARAMETER";

        case EFI_UNSUPPORTED:
            return "EFI_UNSUPPORTED";

        case EFI_BAD_BUFFER_SIZE:
            return "EFI_BAD_BUFFER_SIZE";

        case EFI_BUFFER_TOO_SMALL:
            return "EFI_BUFFER_TOO_SMALL";

        case EFI_NOT_READY:
            return "EFI_NOT_READY";

        case EFI_DEVICE_ERROR:
            return "EFI_DEVICE_ERROR";

        case EFI_WRITE_PROTECTED:
            return "EFI_WRITE_PROTECTED";

        case EFI_OUT_OF_RESOURCES:
            return "EFI_OUT_OF_RESOURCES";

        case EFI_VOLUME_CORRUPTED:
            return "EFI_VOLUME_CORRUPTED";

        case EFI_VOLUME_FULL:
            return "EFI_VOLUME_FULL";

        case EFI_NO_MEDIA:
            return "EFI_NO_MEDIA";

        case EFI_MEDIA_CHANGED:
            return "EFI_MEDIA_CHANGED";

        case EFI_NOT_FOUND:
            return "EFI_NOT_FOUND";

        case EFI_ACCESS_DENIED:
            return "EFI_ACCESS_DENIED";

        case EFI_NO_RESPONSE:
            return "EFI_NO_RESPONSE";

        case EFI_NO_MAPPING:
            return "EFI_NO_MAPPING";

        case EFI_TIMEOUT:
            return "EFI_TIMEOUT";

        case EFI_NOT_STARTED:
            return "EFI_NOT_STARTED";

        case EFI_ALREADY_STARTED:
            return "EFI_ALREADY_STARTED";

        case EFI_ABORTED:
            return "EFI_ABORTED";

        case EFI_ICMP_ERROR:
            return "EFI_ICMP_ERROR";

        case EFI_TFTP_ERROR:
            return "EFI_TFTP_ERROR";

        case EFI_PROTOCOL_ERROR:
            return "EFI_PROTOCOL_ERROR";

        case EFI_INCOMPATIBLE_VERSION:
            return "EFI_INCOMPATIBLE_VERSION";

        case EFI_SECURITY_VIOLATION:
            return "EFI_SECURITY_VIOLATION";

        case EFI_CRC_ERROR:
            return "EFI_CRC_ERROR";

        case EFI_END_OF_MEDIA:
            return "EFI_END_OF_MEDIA";

        case EFI_END_OF_FILE:
            return "EFI_END_OF_FILE";

        case EFI_INVALID_LANGUAGE:
            return "EFI_INVALID_LANGUAGE";

        case EFI_COMPROMISED_DATA:
            return "EFI_COMPROMISED_DATA";

        default:
            return "(unknown error)";
    }
}

void memmove(void* dest, const void* src, size_t n) {
    while (n > 0) {
        *(uint8_t*)dest = *(uint8_t*)src;

        dest = (uint8_t*)dest + 1;
        src = (uint8_t*)src + 1;

        n--;
    }
}
