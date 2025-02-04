/*
    Copyright (C) 2021 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpz_poly.h"
#include "fq_default.h"

TEST_FUNCTION_START(fq_default_get_set_fmpz_poly, state)
{
    int i, result;

    /* fq_zech range */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fq_default_ctx_t ctx;
        fq_default_t fq1, fq2;
        fmpz_poly_t f;
        fmpz_t p;

        fmpz_init(p);

        fmpz_set_ui(p, 3);

        fq_default_ctx_init(ctx, p, 3, "x");

        fq_default_init(fq1, ctx);
        fq_default_init(fq2, ctx);

        fmpz_poly_init(f);

        fq_default_randtest(fq1, state, ctx);

        fq_default_get_fmpz_poly(f, fq1, ctx);
        fq_default_set_fmpz_poly(fq2, f, ctx);

        result = (fq_default_equal(fq1, fq2, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fq_default_print(fq1, ctx); flint_printf("\n\n");
            fq_default_print(fq2, ctx); flint_printf("\n\n");
            fmpz_poly_print(f); flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(f);

        fq_default_clear(fq1, ctx);
        fq_default_clear(fq2, ctx);

        fq_default_ctx_clear(ctx);
    }

    /* fq_nmod range */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fq_default_ctx_t ctx;
        fq_default_t fq1, fq2;
        fmpz_poly_t f;
        fmpz_t p;

        fmpz_init(p);

        fmpz_set_ui(p, 3);

        fq_default_ctx_init(ctx, p, 16, "x");

        fq_default_init(fq1, ctx);
        fq_default_init(fq2, ctx);

        fmpz_poly_init(f);

        fq_default_randtest(fq1, state, ctx);

        fq_default_get_fmpz_poly(f, fq1, ctx);
        fq_default_set_fmpz_poly(fq2, f, ctx);

        result = (fq_default_equal(fq1, fq2, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fq_default_print(fq1, ctx); flint_printf("\n\n");
            fq_default_print(fq2, ctx); flint_printf("\n\n");
            fmpz_poly_print(f); flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(f);

        fq_default_clear(fq1, ctx);
        fq_default_clear(fq2, ctx);

        fq_default_ctx_clear(ctx);

        fmpz_clear(p);
    }

    /* fq range */
    for (i = 0; i < 100 * flint_test_multiplier(); i++)
    {
        fq_default_ctx_t ctx;
        fq_default_t fq1, fq2;
        fmpz_poly_t f;
        fmpz_t p;

        fmpz_init(p);

        fmpz_set_str(p, "73786976294838206473", 10);

        fq_default_ctx_init(ctx, p, 1, "x");

        fq_default_init(fq1, ctx);
        fq_default_init(fq2, ctx);

        fmpz_poly_init(f);

        fq_default_randtest(fq1, state, ctx);

        fq_default_get_fmpz_poly(f, fq1, ctx);
        fq_default_set_fmpz_poly(fq2, f, ctx);

        result = (fq_default_equal(fq1, fq2, ctx));
        if (!result)
        {
            flint_printf("FAIL:\n");
            fq_default_print(fq1, ctx); flint_printf("\n\n");
            fq_default_print(fq2, ctx); flint_printf("\n\n");
            fmpz_poly_print(f); flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_poly_clear(f);

        fq_default_clear(fq1, ctx);
        fq_default_clear(fq2, ctx);

        fq_default_ctx_clear(ctx);

        fmpz_clear(p);
    }

    TEST_FUNCTION_END(state);
}
