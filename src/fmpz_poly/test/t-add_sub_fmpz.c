/*
     Copyright (C) 2020 Vincent Delecroix
     Copyright (C) 2021 Fredrik Johansson

     This file is part of FLINT.

     FLINT is free software: you can redistribute it and/or modify it under
     the terms of the GNU Lesser General Public License (LGPL) as published
     by the Free Software Foundation; either version 2.1 of the License, or
     (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpz.h"
#include "fmpz_poly.h"

TEST_FUNCTION_START(fmpz_poly_add_sub_fmpz, state)
{
    int i;

    for (i = 0; i < 1000 * flint_test_multiplier(); i++)
    {
        fmpz_poly_t a, s, t;
        fmpz_t c;
        int op, alias;

        fmpz_poly_init(a);
        fmpz_poly_init(s);
        fmpz_poly_init(t);
        fmpz_init(c);

        fmpz_poly_randtest(a, state, 1 + n_randint(state, 4), 1 + n_randint(state, 200));
        fmpz_poly_randtest(s, state, 4, 200);
        fmpz_poly_randtest(t, state, 4, 200);
        fmpz_randtest(c, state, 1 + n_randint(state, 200));

        op = n_randint(state, 3);
        alias = n_randint(state, 3);

        fmpz_poly_set_fmpz(s, c);
        if (op == 0)
            fmpz_poly_add(s, a, s);
        else if (op == 1)
            fmpz_poly_sub(s, a, s);
        else
            fmpz_poly_sub(s, s, a);

        if (alias)
        {
            if (op == 0)
                fmpz_poly_add_fmpz(t, a, c);
            else if (op == 1)
                fmpz_poly_sub_fmpz(t, a, c);
            else
                fmpz_poly_fmpz_sub(t, c, a);
        }
        else
        {
            fmpz_poly_set(t, a);

            if (op == 0)
                fmpz_poly_add_fmpz(t, t, c);
            else if (op == 1)
                fmpz_poly_sub_fmpz(t, t, c);
            else
                fmpz_poly_fmpz_sub(t, c, t);
        }

        if (!fmpz_poly_equal(s, t))
        {
           printf("FAIL (op = %d, alias = %d):\n", op, alias);
           printf("a = "); fmpz_poly_print(a); printf("\n");
           printf("s = "); fmpz_poly_print(s); printf("\n");
           printf("t = "); fmpz_poly_print(t); printf("\n");
           printf("c = "); fmpz_print(c); printf("\n");
           fflush(stdout);
           flint_abort();
        }

        fmpz_poly_clear(a);
        fmpz_poly_clear(s);
        fmpz_poly_clear(t);
        fmpz_clear(c);
    }

    TEST_FUNCTION_END(state);
}
