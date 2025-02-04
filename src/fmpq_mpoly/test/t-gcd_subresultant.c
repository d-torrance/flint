/*
    Copyright (C) 2021 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpq_mpoly.h"

/* Defined in t-gcd.c, t-gcd_brown.c, t-gcd_cofactors.c, t-gcd_hensel.c,
 * t-gcd_subresultant.c, t-gcd_zippel2.c */
#define gcd_check gcd_check_gcd_subresultant
void gcd_check(
    fmpq_mpoly_t g,
    fmpq_mpoly_t a,
    fmpq_mpoly_t b,
    fmpq_mpoly_t t,
    fmpq_mpoly_ctx_t ctx,
    slong i,
    slong j,
    const char * name)
{
    fmpq_mpoly_t ca, cb, cg;

    fmpq_mpoly_init(ca, ctx);
    fmpq_mpoly_init(cb, ctx);
    fmpq_mpoly_init(cg, ctx);

    if (!fmpq_mpoly_gcd_subresultant(g, a, b, ctx))
    {
        flint_printf("FAIL: check gcd can be computed\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    fmpq_mpoly_assert_canonical(g, ctx);

    if (fmpq_mpoly_is_zero(g, ctx))
    {
        if (!fmpq_mpoly_is_zero(a, ctx) || !fmpq_mpoly_is_zero(b, ctx))
        {
            flint_printf("FAIL: check zero gcd\n");
            flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
            fflush(stdout);
            flint_abort();
        }
        goto cleanup;
    }

    if (!fmpq_mpoly_is_monic(g, ctx))
    {
        flint_printf("FAIL: check gcd is unit normal\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    if (!fmpq_mpoly_is_zero(t, ctx) && !fmpq_mpoly_divides(cg, g, t, ctx))
    {
        flint_printf("FAIL: check gcd divisor\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    if (!fmpq_mpoly_divides(ca, a, g, ctx) ||
        !fmpq_mpoly_divides(cb, b, g, ctx))
    {
        flint_printf("FAIL: check divisibility\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    if (!fmpq_mpoly_gcd_subresultant(cg, ca, cb, ctx))
    {
        flint_printf("FAIL: check cofactor gcd can be computed\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

    fmpq_mpoly_assert_canonical(cg, ctx);

    if (!fmpq_mpoly_is_one(cg, ctx))
    {
        flint_printf("FAIL: check gcd of cofactors is one\n");
        flint_printf("i = %wd, j = %wd, %s\n", i, j, name);
        fflush(stdout);
        flint_abort();
    }

cleanup:

    fmpq_mpoly_clear(ca, ctx);
    fmpq_mpoly_clear(cb, ctx);
    fmpq_mpoly_clear(cg, ctx);
}

TEST_FUNCTION_START(fmpq_mpoly_gcd_subresultant, state)
{
    slong i, j;
    slong tmul = 5;

    {
        fmpq_mpoly_ctx_t ctx;
        fmpq_mpoly_t g, a, b, t;
        const char * vars[] = {"x", "y", "z", "t"};

        fmpq_mpoly_ctx_init(ctx, 4, ORD_LEX);
        fmpq_mpoly_init(a, ctx);
        fmpq_mpoly_init(b, ctx);
        fmpq_mpoly_init(g, ctx);
        fmpq_mpoly_init(t, ctx);

        fmpq_mpoly_set_str_pretty(t, "39 - t*x + 39*x^100 - t*x^101 + 39*x^3*y - t*x^4*y - 7*x^2*y^3*z^11 - 7*x^102*y^3*z^11 - 7*x^5*y^4*z^11 + 78*t^15*x^78*y^3*z^13 - 2*t^16*x^79*y^3*z^13 + x^1000*y^3*z^20 + x^1100*y^3*z^20 + x^1003*y^4*z^20 - 14*t^15*x^80*y^6*z^24 + 2*t^15*x^1078*y^6*z^33", vars, ctx);
        fmpq_mpoly_set_str_pretty(a, "39 - t*x - 7*x^2*y^3*z^11 + x^1000*y^3*z^20", vars, ctx);
        fmpq_mpoly_set_str_pretty(b, "1 + x^100 + x^3*y + 2*t^15*x^78*y^3*z^13", vars, ctx);
        fmpq_mpoly_mul(a, a, t, ctx);
        fmpq_mpoly_mul(b, b, t, ctx);

        gcd_check(g, a, b, t, ctx, 0, 0, "example");

        fmpq_mpoly_clear(a, ctx);
        fmpq_mpoly_clear(b, ctx);
        fmpq_mpoly_clear(g, ctx);
        fmpq_mpoly_clear(t, ctx);
        fmpq_mpoly_ctx_clear(ctx);
    }

    for (i = 0; i < tmul * flint_test_multiplier(); i++)
    {
        fmpq_mpoly_ctx_t ctx;
        fmpq_mpoly_t a, b, g, t;
        slong len, len1, len2;
        slong degbound;
        flint_bitcnt_t coeff_bits;

        fmpq_mpoly_ctx_init_rand(ctx, state, 3);

        fmpq_mpoly_init(g, ctx);
        fmpq_mpoly_init(a, ctx);
        fmpq_mpoly_init(b, ctx);
        fmpq_mpoly_init(t, ctx);

        len = n_randint(state, 20) + 1;
        len1 = n_randint(state, 20);
        len2 = n_randint(state, 20);

        degbound = 1 + 10/FLINT_MAX(WORD(1), fmpq_mpoly_ctx_nvars(ctx));

        for (j = 0; j < 4; j++)
        {
            coeff_bits = 1 + n_randint(state, 80);
            fmpq_mpoly_randtest_bound(t, state, len, coeff_bits, degbound, ctx);
            if (fmpq_mpoly_is_zero(t, ctx))
                fmpq_mpoly_one(t, ctx);
            coeff_bits = 1 + n_randint(state, 80);
            fmpq_mpoly_randtest_bound(a, state, len1, coeff_bits, degbound, ctx);
            coeff_bits = 1 + n_randint(state, 80);
            fmpq_mpoly_randtest_bound(b, state, len2, coeff_bits, degbound, ctx);
            fmpq_mpoly_mul(a, a, t, ctx);
            fmpq_mpoly_mul(b, b, t, ctx);
            coeff_bits = 1 + n_randint(state, 80);
            fmpq_mpoly_randtest_bits(g, state, len, coeff_bits, FLINT_BITS, ctx);

            gcd_check(g, a, b, t, ctx, i, j, "random small");
        }

        fmpq_mpoly_clear(g, ctx);
        fmpq_mpoly_clear(a, ctx);
        fmpq_mpoly_clear(b, ctx);
        fmpq_mpoly_clear(t, ctx);
        fmpq_mpoly_ctx_clear(ctx);
    }

    TEST_FUNCTION_END(state);
}
#undef gcd_check
