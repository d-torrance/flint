/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include <mpfr.h>
#include "arb.h"

TEST_FUNCTION_START(arb_zeta_ui_euler_product, state)
{
    slong iter;

    for (iter = 0; iter < 10000 * 0.1 * flint_test_multiplier(); iter++)
    {
        arb_t r;
        ulong n;
        mpfr_t s;
        slong prec, accuracy;

        do { n = n_randint(state, 1 << n_randint(state, 10)); } while (n < 6);

        prec = 2 + n_randint(state, n * FLINT_BIT_COUNT(n));

        arb_init(r);
        mpfr_init2(s, prec + 100);

        arb_zeta_ui_euler_product(r, n, prec);
        mpfr_zeta_ui(s, n, MPFR_RNDN);

        if (!arb_contains_mpfr(r, s))
        {
            flint_printf("FAIL: containment\n\n");
            flint_printf("n = %wu\n\n", n);
            flint_printf("r = "); arb_printd(r, prec / 3.33); flint_printf("\n\n");
            flint_printf("s = "); mpfr_printf("%.275Rf\n", s); flint_printf("\n\n");
            flint_abort();
        }

        accuracy = arb_rel_accuracy_bits(r);

        if (accuracy < prec - 4)
        {
            flint_printf("FAIL: accuracy = %wd, prec = %wd\n\n", accuracy, prec);
            flint_printf("n = %wu\n\n", n);
            flint_printf("r = "); arb_printd(r, prec / 3.33); flint_printf("\n\n");
            flint_abort();
        }

        arb_clear(r);
        mpfr_clear(s);
    }

    TEST_FUNCTION_END(state);
}
