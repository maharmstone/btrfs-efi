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

#pragma once

#include <efibind.h>
#include <efidef.h>
#include <efidevp.h>
#include <efiprot.h>
#include <eficon.h>
#include <efiapi.h>
#include <efierr.h>
#include <stddef.h>

char* stpcpy(char* dest, const char* src);
char* hex_to_str(char* s, uint64_t v);
char* dec_to_str(char* s, uint64_t v);
EFI_STATUS utf8_to_utf16(wchar_t* dest, unsigned int dest_max, unsigned int* dest_len, const char* src, unsigned int src_len);
EFI_STATUS utf16_to_utf8(char* dest, unsigned int dest_max, unsigned int* dest_len, const wchar_t* src, unsigned int src_len);
const char* error_string(EFI_STATUS Status);
