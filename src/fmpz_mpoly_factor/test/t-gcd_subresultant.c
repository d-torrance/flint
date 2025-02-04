/*
    Copyright (C) 2021 Daniel Schultz
    Copyright (C) 2023 Albin Ahlbäck

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpz_mpoly_factor.h"

/* Defined in t-gcd_brown.c, t-gcd_brown_threaded.c, t-gcd_subresultant.c,
 * t-gcd_zippel.c, t-gcd_zippel2.c */
#define compute_gcd compute_gcd_subresultant
int compute_gcd(
    fmpz_mpoly_t G,
    const fmpz_mpoly_t A,
    const fmpz_mpoly_t B,
    const fmpz_mpoly_ctx_t ctx)
{
    int success;
    slong var = 0;
    fmpz_mpoly_t Ac, Bc, Gc, s, t;
    fmpz_mpoly_univar_t Ax, Bx, Gx;

    if (fmpz_mpoly_is_zero(A, ctx) || fmpz_mpoly_is_zero(B, ctx))
        return fmpz_mpoly_gcd(G, A, B, ctx);

    fmpz_mpoly_init(Ac, ctx);
    fmpz_mpoly_init(Bc, ctx);
    fmpz_mpoly_init(Gc, ctx);
    fmpz_mpoly_init(s, ctx);
    fmpz_mpoly_init(t, ctx);
    fmpz_mpoly_univar_init(Ax, ctx);
    fmpz_mpoly_univar_init(Bx, ctx);
    fmpz_mpoly_univar_init(Gx, ctx);

    fmpz_mpoly_to_univar(Ax, A, var, ctx);
    fmpz_mpoly_to_univar(Bx, B, var, ctx);

    success = _fmpz_mpoly_vec_content_mpoly(Ac, Ax->coeffs, Ax->length, ctx) &&
              _fmpz_mpoly_vec_content_mpoly(Bc, Bx->coeffs, Bx->length, ctx) &&
              fmpz_mpoly_gcd(Gc, Ac, Bc, ctx);
    if (!success)
        goto cleanup;

    _fmpz_mpoly_vec_divexact_mpoly(Ax->coeffs, Ax->length, Ac, ctx);
    _fmpz_mpoly_vec_divexact_mpoly(Bx->coeffs, Bx->length, Bc, ctx);

    success = fmpz_cmp(Ax->exps + 0, Bx->exps + 0) > 0 ?
                            fmpz_mpoly_univar_pseudo_gcd(Gx, Ax, Bx, ctx) :
                            fmpz_mpoly_univar_pseudo_gcd(Gx, Bx, Ax, ctx);
    if (!success)
        goto cleanup;

    if (fmpz_mpoly_gcd(t, Ax->coeffs + 0, Bx->coeffs + 0, ctx) &&
                                                                t->length == 1)
    {
        fmpz_mpoly_term_content(s, Gx->coeffs + 0, ctx);
        fmpz_mpoly_divexact(t, Gx->coeffs + 0, s, ctx);
        _fmpz_mpoly_vec_divexact_mpoly(Gx->coeffs, Gx->length, t, ctx);
    }
    else if (fmpz_mpoly_gcd(t, Ax->coeffs + Ax->length - 1,
                           Bx->coeffs + Bx->length - 1, ctx) && t->length == 1)
    {
        fmpz_mpoly_term_content(s, Gx->coeffs + Gx->length - 1, ctx);
        fmpz_mpoly_divexact(t, Gx->coeffs + Gx->length - 1, s, ctx);
        _fmpz_mpoly_vec_divexact_mpoly(Gx->coeffs, Gx->length, t, ctx);
    }

    success = _fmpz_mpoly_vec_content_mpoly(t, Gx->coeffs, Gx->length, ctx);
    if (!success)
        goto cleanup;

    _fmpz_mpoly_vec_divexact_mpoly(Gx->coeffs, Gx->length, t, ctx);
    _fmpz_mpoly_vec_mul_mpoly(Gx->coeffs, Gx->length, Gc, ctx);
    _fmpz_mpoly_from_univar(G, FLINT_MIN(A->bits, B->bits), Gx, var, ctx);

    fmpz_mpoly_unit_normalize(G, ctx);

    success = 1;

cleanup:

    fmpz_mpoly_clear(Ac, ctx);
    fmpz_mpoly_clear(Bc, ctx);
    fmpz_mpoly_clear(Gc, ctx);
    fmpz_mpoly_clear(s, ctx);
    fmpz_mpoly_clear(t, ctx);
    fmpz_mpoly_univar_clear(Ax, ctx);
    fmpz_mpoly_univar_clear(Bx, ctx);
    fmpz_mpoly_univar_clear(Gx, ctx);

    return success;
}

/* Defined in t-gcd_brown.c, t-gcd_brown_threaded.c, t-gcd_subresultant.c,
 * t-gcd_zippel.c, t-gcd_zippel2.c */
#ifndef gcd_check
#define gcd_check gcd_check
void gcd_check(
    fmpz_mpoly_t g,
    fmpz_mpoly_t a,
    fmpz_mpoly_t b,
    const fmpz_mpoly_t gdiv,
    fmpz_mpoly_ctx_t ctx,
    slong i,
    slong j,
    const char * name,
    int compute_gcd_fun(fmpz_mpoly_t, const fmpz_mpoly_t, const fmpz_mpoly_t, const fmpz_mpoly_ctx_t))
{
    int res;
    fmpz_mpoly_t ca, cb, cg;

    fmpz_mpoly_init(ca, ctx);
    fmpz_mpoly_init(cb, ctx);
    fmpz_mpoly_init(cg, ctx);

    res = compute_gcd_fun(g, a, b, ctx);

    fmpz_mpoly_assert_canonical(g, ctx);

    if (!res)
    {
        flint_printf("FAIL: Check gcd can be computed\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    if (!fmpz_mpoly_is_zero(gdiv, ctx))
    {
        if (!fmpz_mpoly_divides(ca, g, gdiv, ctx))
        {
            flint_printf("FAIL: Check divisor of gcd\n");
            flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
            fflush(stdout);
            flint_abort();
        }
    }

    if (fmpz_mpoly_is_zero(g, ctx))
    {
        if (!fmpz_mpoly_is_zero(a, ctx) || !fmpz_mpoly_is_zero(b, ctx))
        {
            flint_printf("FAIL: Check zero gcd\n");
            flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
            fflush(stdout);
            flint_abort();
        }
        goto cleanup;
    }

    if (fmpz_sgn(g->coeffs + 0) <= 0)
    {
        flint_printf("FAIL: Check gcd has positive lc\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    res = 1;
    res = res && fmpz_mpoly_divides(ca, a, g, ctx);
    res = res && fmpz_mpoly_divides(cb, b, g, ctx);
    if (!res)
    {
        flint_printf("FAIL: Check divisibility\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    res = compute_gcd_fun(cg, ca, cb, ctx);
    fmpz_mpoly_assert_canonical(cg, ctx);

    if (!res)
    {
        flint_printf("FAIL: Check gcd of cofactors can be computed\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    if (!fmpz_mpoly_is_one(cg, ctx))
    {
        flint_printf("FAIL: Check gcd of cofactors is one\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

cleanup:

    fmpz_mpoly_clear(ca, ctx);
    fmpz_mpoly_clear(cb, ctx);
    fmpz_mpoly_clear(cg, ctx);
}
#endif

TEST_FUNCTION_START(fmpz_mpoly_factor_gcd_subresultant, state)
{
    slong i, j, tmul = 10;

    for (i = 0; i < tmul * flint_test_multiplier(); i++)
    {
        fmpz_mpoly_ctx_t ctx;
        fmpz_mpoly_t a, b, g, t;
        slong len, len1, len2;
        slong degbound;
        flint_bitcnt_t coeff_bits;

        fmpz_mpoly_ctx_init_rand(ctx, state, 3);
        if (ctx->minfo->nvars < 1)
        {
            fmpz_mpoly_ctx_clear(ctx);
            continue;
        }

        fmpz_mpoly_init(g, ctx);
        fmpz_mpoly_init(a, ctx);
        fmpz_mpoly_init(b, ctx);
        fmpz_mpoly_init(t, ctx);

        len = n_randint(state, 15) + 1;
        len1 = n_randint(state, 15);
        len2 = n_randint(state, 15);

        degbound = 1 + 10/ctx->minfo->nvars;

        for (j = 0; j < 4; j++)
        {
            coeff_bits = n_randint(state, 100) + 1;
            fmpz_mpoly_randtest_bound(t, state, len, coeff_bits, degbound, ctx);
            if (fmpz_mpoly_is_zero(t, ctx))
                fmpz_mpoly_one(t, ctx);
            coeff_bits = n_randint(state, 100) + 1;
            fmpz_mpoly_randtest_bound(a, state, len1, coeff_bits, degbound, ctx);
            coeff_bits = n_randint(state, 100) + 1;
            fmpz_mpoly_randtest_bound(b, state, len2, coeff_bits, degbound, ctx);
            fmpz_mpoly_mul(a, a, t, ctx);
            fmpz_mpoly_mul(b, b, t, ctx);
            fmpz_mpoly_randtest_bits(g, state, len, coeff_bits, FLINT_BITS, ctx);
            gcd_check(g, a, b, t, ctx, i, j, "random dense", compute_gcd);
        }

        fmpz_mpoly_clear(g, ctx);
        fmpz_mpoly_clear(a, ctx);
        fmpz_mpoly_clear(b, ctx);
        fmpz_mpoly_clear(t, ctx);
        fmpz_mpoly_ctx_clear(ctx);
    }

    TEST_FUNCTION_END(state);
}
#undef compute_gcd
