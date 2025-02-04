/*
    Copyright (C) 2020 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "ca.h"

TEST_FUNCTION_START(ca_add, state)
{
    slong iter;

    /* check special values */
    {
        ca_ctx_t ctx;
        ca_t x, y, z;

        ca_ctx_init(ctx);
        ca_init(x, ctx);
        ca_init(y, ctx);
        ca_init(z, ctx);

        ca_set_si(x, 2, ctx);
        ca_uinf(y, ctx);
        ca_add(z, x, y, ctx);

        if (ca_check_is_uinf(z, ctx) != T_TRUE)
        {
            flint_printf("FAIL: 2 + uinf\n");
            flint_abort();
        }

        ca_pos_inf(x, ctx);
        ca_uinf(y, ctx);
        ca_add(z, x, y, ctx);

        if (ca_check_is_undefined(z, ctx) != T_TRUE)
        {
            flint_printf("FAIL: +inf + uinf\n");
            flint_abort();
        }

        ca_neg_inf(x, ctx);
        ca_neg_inf(y, ctx);
        ca_add(z, x, y, ctx);

        if (ca_check_is_neg_inf(z, ctx) != T_TRUE)
        {
            flint_printf("FAIL: -inf + (-inf)\n");
            flint_abort();
        }

        ca_pos_inf(x, ctx);
        ca_neg_inf(y, ctx);
        ca_add(z, x, y, ctx);

        if (ca_check_is_undefined(z, ctx) != T_TRUE)
        {
            flint_printf("FAIL: +inf + (-inf)\n");
            flint_abort();
        }

        ca_clear(x, ctx);
        ca_clear(y, ctx);
        ca_clear(z, ctx);
        ca_ctx_clear(ctx);
    }

    for (iter = 0; iter < 1000 * 0.1 * flint_test_multiplier(); iter++)
    {
        ca_ctx_t ctx;
        ca_t x, y, z, a, b;
        truth_t equal;

        ca_ctx_init(ctx);
        ca_init(x, ctx);
        ca_init(y, ctx);
        ca_init(z, ctx);
        ca_init(a, ctx);
        ca_init(b, ctx);

        /* test (x + y) + z = x + (y + z) */
        ca_randtest_special(x, state, 5, 5, ctx);
        ca_randtest_special(y, state, 5, 5, ctx);
        ca_randtest_special(z, state, 5, 5, ctx);
        ca_randtest_special(a, state, 5, 5, ctx);
        ca_randtest_special(b, state, 5, 5, ctx);

        ca_add(a, x, y, ctx);
        ca_add(a, a, z, ctx);

        ca_add(b, y, z, ctx);
        ca_add(b, x, b, ctx);

        equal = ca_check_equal(a, b, ctx);

        if (equal == T_FALSE)
        {
            flint_printf("FAIL\n\n");
            flint_printf("x = "); ca_print(x, ctx); flint_printf("\n\n");
            flint_printf("y = "); ca_print(y, ctx); flint_printf("\n\n");
            flint_printf("z = "); ca_print(z, ctx); flint_printf("\n\n");
            flint_printf("a = "); ca_print(a, ctx); flint_printf("\n\n");
            flint_printf("b = "); ca_print(b, ctx); flint_printf("\n\n");
            flint_abort();
        }

        ca_clear(x, ctx);
        ca_clear(y, ctx);
        ca_clear(z, ctx);
        ca_clear(a, ctx);
        ca_clear(b, ctx);
        ca_ctx_clear(ctx);
    }

    TEST_FUNCTION_END(state);
}
