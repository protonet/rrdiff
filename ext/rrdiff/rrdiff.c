#include <ruby.h>
#include <stdio.h>
#include <stdlib.h>
#include <librsync.h>
#include <limits.h>

VALUE RRDiff = Qnil;

static FILE *my_fopen(const char *path, const char *mode);

static FILE *my_fopen(const char *path, const char *mode)
{
    FILE *result = fopen(path, mode);
    char buf[1024];

    if (! result) {
        snprintf(buf, sizeof(buf), "fopen(\"%s\", \"%s\")", path, mode);
        rb_sys_fail(buf);
    }

    return result;
}

static VALUE rrdiff_signature(VALUE mod, VALUE old_file, VALUE sig_file)
{
    FILE *basis, *signature;

    rs_result result;
    rs_stats_t stats;

    basis = my_fopen(StringValuePtr(old_file), "rb");
    signature = my_fopen(StringValuePtr(sig_file), "wb");

    result = rs_sig_file(basis, signature, RS_DEFAULT_BLOCK_LEN, RS_DEFAULT_STRONG_LEN, &stats);

    fclose(basis);
    fclose(signature);

    return Qnil;
}

static VALUE rrdiff_delta(VALUE mod, VALUE new_file, VALUE sig_file, VALUE delta_file)
{
    FILE *newfile, *sigfile, *deltafile;

    rs_result result;
    rs_stats_t stats;
    rs_signature_t *sig;

    newfile = my_fopen(StringValuePtr(new_file), "rb");
    sigfile = my_fopen(StringValuePtr(sig_file), "rb");
    deltafile = my_fopen(StringValuePtr(delta_file), "wb");

    if((result = rs_loadsig_file(sigfile, &sig, &stats)) != RS_DONE)
        return Qnil;

    if ((result = rs_build_hash_table(sig)) != RS_DONE)
        return Qnil;

    result = rs_delta_file(sig, newfile, deltafile, &stats);

    rs_free_sumset(sig);

    fclose(newfile);
    fclose(sigfile);
    fclose(deltafile);

    return Qnil;
}

static VALUE rrdiff_patch(VALUE mod, VALUE old_file, VALUE delta_file, VALUE patched_file)
{
    FILE *basisfile, *deltafile, *newfile;

    rs_stats_t stats;
    rs_result result;

    basisfile = my_fopen(StringValuePtr(old_file), "rb");
    deltafile = my_fopen(StringValuePtr(delta_file), "rb");
    newfile = my_fopen(StringValuePtr(patched_file), "wb");

    result = rs_patch_file(basisfile, deltafile, newfile, &stats);

    fclose(newfile);
    fclose(deltafile);
    fclose(basisfile);

    return Qnil;
}

void Init_rrdiff()
{
    RRDiff = rb_define_module("RRDiff");

    rb_define_singleton_method(RRDiff, "signature", rrdiff_signature, 2);
    rb_define_singleton_method(RRDiff, "delta", rrdiff_delta, 3);
    rb_define_singleton_method(RRDiff, "patch", rrdiff_patch, 3);
}
