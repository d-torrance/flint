/*
    Copyright (C) 2013 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "acb_poly.h"
#include "acb_dirichlet.h"

TEST_FUNCTION_START(acb_dirichlet_l_series, state)
{
    slong iter;

    for (iter = 0; iter < 100 * 0.1 * flint_test_multiplier(); iter++)
    {
        slong m, n1, n2, bits1, bits2, bits3;
        int deflate;
        acb_poly_t S, A, B, C;
        dirichlet_group_t G;
        dirichlet_char_t chi;

        bits1 = 2 + n_randint(state, 200);
        bits2 = 2 + n_randint(state, 200);
        bits3 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 8);
        n1 = 1 + n_randint(state, 8);
        n2 = 1 + n_randint(state, 8);

        dirichlet_group_init(G, 1 + n_randint(state, 12));
        dirichlet_char_init(chi, G);
        dirichlet_char_index(chi, G, n_randint(state, G->phi_q));

        acb_poly_init(S);
        acb_poly_init(A);
        acb_poly_init(B);
        acb_poly_init(C);

        deflate = n_randint(state, 2);
        acb_poly_randtest(S, state, m, bits1, 3);

        acb_dirichlet_l_series(A, S, G, chi, deflate, n1, bits2);
        acb_poly_set(B, S);  /* aliasing */
        acb_dirichlet_l_series(B, B, G, chi, deflate, n2, bits3);

        acb_poly_set(C, A);
        acb_poly_truncate(C, FLINT_MIN(n1, n2));
        acb_poly_truncate(B, FLINT_MIN(n1, n2));

        if (!acb_poly_overlaps(B, C))
        {
            flint_printf("FAIL\n\n");
            flint_printf("S = "); acb_poly_printd(S, 15); flint_printf("\n\n");
            flint_printf("A = "); acb_poly_printd(A, 15); flint_printf("\n\n");
            flint_printf("B = "); acb_poly_printd(B, 15); flint_printf("\n\n");
            flint_abort();
        }

        acb_poly_clear(S);
        acb_poly_clear(A);
        acb_poly_clear(B);
        acb_poly_clear(C);

        dirichlet_char_clear(chi);
        dirichlet_group_clear(G);
    }

    TEST_FUNCTION_END(state);
}
