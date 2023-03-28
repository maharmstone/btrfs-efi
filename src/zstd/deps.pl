#!/usr/bin/perl

$s = <<END;
zstd_compress.c: cpu.h, mem.h, mem.h, hist.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_fast.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_double_fast.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_lazy.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_opt.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_ldm.h, zstd.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h
zstd_fast.c: zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_fast.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h
zstd_double_fast.c: zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_double_fast.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h
zstd_lazy.c: zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_lazy.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h
zstd_opt.c: zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, hist.h, zstd_opt.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h
fse_compress.c: compiler.h, mem.h, debug.h, hist.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, error_private.h, zstd_errors.h
zstd_ldm.c: zstd_ldm.h, zstd.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, debug.h, zstd_fast.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h, zstd_double_fast.h, mem.h, zstd_compress_internal.h, zstd_internal.h, compiler.h, mem.h, debug.h, error_private.h, zstd_errors.h, zstd.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, xxhash.h   /* ZSTD_fillDoubleHashTable() */
huf_compress.c: compiler.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, hist.h, fse.h, bitstream.h, mem.h, debug.h, error_private.h, zstd_errors.h, huf.h, mem.h, error_private.h, zstd_errors.h
hist.c: mem.h, debug.h, error_private.h, zstd_errors.h, hist.h
END

@l = split(/\n/, $s);

for ($i = 0; $i <= $#l; $i++) {
	if ($l[$i] =~ /^([^\:]*)\: (.*)$/) {
		$fn = $1;
		$lss = $2;

		@ls = uniq(sort(split(/, /, $lss)));

		print $fn.": ".join(", ", @ls)."\n";
	}
}

sub uniq {
    my %seen;
    grep !$seen{$_}++, @_;
}

