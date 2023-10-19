/*
    Copyright (C) 2021 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpz_mod_mpoly.h"

/* foolproof way to check totdeg_check is correct */
void _check_total_degree(const fmpz_t totdeg_check, const fmpz_mod_mpoly_t A,
                                                const fmpz_mod_mpoly_ctx_t ctx)
{
    fmpz_t totdeg;
    fmpz_init(totdeg);
    mpoly_total_degree_fmpz_ref(totdeg, A->exps, A->length, A->bits, ctx->minfo);
    if (!fmpz_equal(totdeg_check, totdeg))
        flint_throw(FLINT_ERROR, "Total degree is wrong");
    fmpz_clear(totdeg);
}

TEST_FUNCTION_START(fmpz_mod_mpoly_total_degree, state)
{
    slong i, j;

    /* Check total_degree does not go up under addition */
    for (i = 0; i < 40 * flint_test_multiplier(); i++)
    {
        fmpz_mod_mpoly_ctx_t ctx;
        fmpz_mod_mpoly_t f, g, h;
        fmpz_t fdeg, gdeg, hdeg;
        slong len1, len2;
        flint_bitcnt_t exp_bits1, exp_bits2;

        fmpz_mod_mpoly_ctx_init_rand_bits(ctx, state, 20, 200);

        fmpz_mod_mpoly_init(f, ctx);
        fmpz_mod_mpoly_init(g, ctx);
        fmpz_mod_mpoly_init(h, ctx);
        fmpz_init(fdeg);
        fmpz_init(gdeg);
        fmpz_init(hdeg);

        len1 = n_randint(state, 100);
        len2 = n_randint(state, 100);

        exp_bits1 = n_randint(state, 100) + 2;
        exp_bits2 = n_randint(state, 100) + 2;

        for (j = 0; j < 4; j++)
        {
            fmpz_mod_mpoly_randtest_bits(f, state, len1, exp_bits1, ctx);
            fmpz_mod_mpoly_randtest_bits(g, state, len2, exp_bits2, ctx);
            fmpz_mod_mpoly_add(h, f, g, ctx);

            fmpz_mod_mpoly_total_degree_fmpz(hdeg, h, ctx);
            fmpz_mod_mpoly_total_degree_fmpz(fdeg, f, ctx);
            fmpz_mod_mpoly_total_degree_fmpz(gdeg, g, ctx);
            _check_total_degree(hdeg, h, ctx);
            _check_total_degree(fdeg, f, ctx);
            _check_total_degree(gdeg, g, ctx);

            if ((fmpz_cmp(hdeg, fdeg) > 0) && (fmpz_cmp(hdeg, gdeg) > 0))
            {
                flint_printf("FAIL: Check degree does not go up under addition\n");
                flint_printf("i: %wd  j: %wd\n", i, j);
                fflush(stdout);
                flint_abort();
            }
        }

        fmpz_clear(fdeg);
        fmpz_clear(gdeg);
        fmpz_clear(hdeg);
        fmpz_mod_mpoly_clear(f, ctx);
        fmpz_mod_mpoly_clear(g, ctx);
        fmpz_mod_mpoly_clear(h, ctx);
        fmpz_mod_mpoly_ctx_clear(ctx);
    }

    /* Check total_degree adds under multiplication */
    for (i = 0; i < 40 * flint_test_multiplier(); i++)
    {
        int ok;
        fmpz_mod_mpoly_ctx_t ctx;
        fmpz_mod_mpoly_t f, g, h;
        fmpz_t fdeg, gdeg, hdeg;
        slong len1, len2;
        flint_bitcnt_t exp_bits1, exp_bits2;

        fmpz_mod_mpoly_ctx_init_rand_bits(ctx, state, 20, 200);

        fmpz_mod_mpoly_init(f, ctx);
        fmpz_mod_mpoly_init(g, ctx);
        fmpz_mod_mpoly_init(h, ctx);
        fmpz_init(fdeg);
        fmpz_init(gdeg);
        fmpz_init(hdeg);

        len1 = n_randint(state, 10);
        len2 = n_randint(state, 10);

        exp_bits1 = n_randint(state, 100) + 2;
        exp_bits2 = n_randint(state, 100) + 2;

        for (j = 0; j < 4; j++)
        {
            fmpz_mod_mpoly_randtest_bits(f, state, len1, exp_bits1, ctx);
            fmpz_mod_mpoly_randtest_bits(g, state, len2, exp_bits2, ctx);
            fmpz_mod_mpoly_mul(h, f, g, ctx);

            fmpz_mod_mpoly_total_degree_fmpz(hdeg, h, ctx);
            fmpz_mod_mpoly_total_degree_fmpz(fdeg, f, ctx);
            fmpz_mod_mpoly_total_degree_fmpz(gdeg, g, ctx);
            _check_total_degree(hdeg, h, ctx);
            _check_total_degree(fdeg, f, ctx);
            _check_total_degree(gdeg, g, ctx);

            if (fmpz_mod_mpoly_is_zero(f, ctx) || fmpz_mod_mpoly_is_zero(g, ctx))
            {
                ok = fmpz_equal_si(hdeg, -WORD(1))
                     && (fmpz_equal_si(fdeg, -WORD(1))
                         || fmpz_equal_si(gdeg, -WORD(1)));
            }
            else
            {
                fmpz_add(gdeg, gdeg, fdeg);
                ok = (fmpz_cmp(hdeg, gdeg) <= 0);
            }

            if (!ok)
            {
                flint_printf("FAIL: Check degree adds under multiplication\n");
                flint_printf("i: %wd  j: %wd\n", i, j);
                fflush(stdout);
                flint_abort();
            }
        }

        fmpz_clear(fdeg);
        fmpz_clear(gdeg);
        fmpz_clear(hdeg);
        fmpz_mod_mpoly_clear(f, ctx);
        fmpz_mod_mpoly_clear(g, ctx);
        fmpz_mod_mpoly_clear(h, ctx);
        fmpz_mod_mpoly_ctx_clear(ctx);
    }

    TEST_FUNCTION_END(state);
}
