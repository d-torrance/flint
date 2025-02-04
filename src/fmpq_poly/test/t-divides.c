/*
    Copyright (C) 2020 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpq_poly.h"

TEST_FUNCTION_START(fmpq_poly_divides, state)
{
    int i, result, d1, d2;
    ulong cflags = UWORD(0);

    /* Check aliasing of q and a */
    for (i = 0; i < 200 * flint_test_multiplier(); i++)
    {
        fmpq_poly_t a, b, q;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);

	fmpq_poly_randtest(a, state, n_randint(state, 50), 200);
        fmpq_poly_randtest_not_zero(b, state, n_randint(state, 50) + 1, 200);

	if (n_randint(state, 2))
            fmpq_poly_mul(a, a, b);

        d1 = fmpq_poly_divides(q, a, b);
        d2 = fmpq_poly_divides(a, a, b);

        cflags |= fmpq_poly_is_canonical(q) ? 0 : 1;
        cflags |= fmpq_poly_is_canonical(a) ? 0 : 2;
        result = (d1 == d2 && (d1 == 0 || fmpq_poly_equal(q, a)) && !cflags);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("q = "), fmpq_poly_debug(q), flint_printf("\n\n");
            flint_printf("a = "), fmpq_poly_debug(a), flint_printf("\n\n");
            flint_printf("b = "), fmpq_poly_debug(b), flint_printf("\n\n");
            flint_printf("cflags = %wu, d1 = %d, d2 = %d\n\n", cflags, d1, d2);
            fflush(stdout);
            flint_abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
    }

    /* Check aliasing of q and b */
    for (i = 0; i < 200 * flint_test_multiplier(); i++)
    {
        fmpq_poly_t a, b, q;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);

        fmpq_poly_randtest(a, state, n_randint(state, 50), 200);
        fmpq_poly_randtest_not_zero(b, state, n_randint(state, 50) + 1, 200);

        if (n_randint(state, 2))
            fmpq_poly_mul(a, a, b);

	d1 = fmpq_poly_divides(q, a, b);
        d2 = fmpq_poly_divides(b, a, b);

        cflags |= fmpq_poly_is_canonical(q) ? 0 : 1;
        cflags |= fmpq_poly_is_canonical(b) ? 0 : 2;
        result = (d1 == d2 && (d1 == 0 || fmpq_poly_equal(q, b)) && !cflags);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("q = "), fmpq_poly_debug(q), flint_printf("\n\n");
            flint_printf("a = "), fmpq_poly_debug(a), flint_printf("\n\n");
            flint_printf("b = "), fmpq_poly_debug(b), flint_printf("\n\n");
            flint_printf("cflags = %wu, d1 = %d, d2 = %d\n\n", cflags, d1, d2);
            fflush(stdout);
            flint_abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
    }

    /* Compare with divrem */
    for (i = 0; i < 200 * flint_test_multiplier(); i++)
    {
        fmpq_poly_t a, b, q, q2, r;

        fmpq_poly_init(a);
        fmpq_poly_init(b);
        fmpq_poly_init(q);
        fmpq_poly_init(q2);
        fmpq_poly_init(r);
        fmpq_poly_randtest(a, state, n_randint(state, 50), 200);
        fmpq_poly_randtest_not_zero(b, state, n_randint(state, 50) + 1, 200);

        if (n_randint(state, 2))
           fmpq_poly_mul(a, a, b);

	fmpq_poly_divrem(q, r, a, b);
        d1 = fmpq_poly_divides(q2, a, b);

	d2 = fmpq_poly_is_zero(r);

        cflags |= fmpq_poly_is_canonical(q)  ? 0 : 1;
        cflags |= fmpq_poly_is_canonical(q2) ? 0 : 2;
        result = (d1 == d2 && (d1 == 0 || fmpq_poly_equal(q, q2)) && !cflags);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("a  = "), fmpq_poly_debug(a), flint_printf("\n\n");
            flint_printf("b  = "), fmpq_poly_debug(b), flint_printf("\n\n");
            flint_printf("q  = "), fmpq_poly_debug(q), flint_printf("\n\n");
            flint_printf("r  = "), fmpq_poly_debug(r), flint_printf("\n\n");
            flint_printf("q2 = "), fmpq_poly_debug(q2), flint_printf("\n\n");
            flint_printf("cflags = %wu, d1 = %d, d2 = %d\n\n", cflags, d1, d2);
            fflush(stdout);
            flint_abort();
        }

        fmpq_poly_clear(a);
        fmpq_poly_clear(b);
        fmpq_poly_clear(q);
        fmpq_poly_clear(q2);
        fmpq_poly_clear(r);
    }

    TEST_FUNCTION_END(state);
}
