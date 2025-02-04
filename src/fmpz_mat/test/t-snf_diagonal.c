/*
    Copyright (C) 2014 Alex J. Best

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "fmpz.h"
#include "fmpz_mat.h"

TEST_FUNCTION_START(fmpz_mat_snf_diagonal, state)
{
    slong iter;

    for (iter = 0; iter < 10000 * flint_test_multiplier(); iter++)
    {
        fmpz_mat_t A, S, S2;
        slong b, i, m, n;
        int equal;

        m = n_randint(state, 10);
        n = n_randint(state, 10);
        b = 1 + n_randint(state, 10) * n_randint(state, 10);

        fmpz_mat_init(A, m, n);
        fmpz_mat_init(S, m, n);
        fmpz_mat_init(S2, m, n);

        for (i = 0; i < FLINT_MIN(m, n); i++)
        {
            fmpz_randtest_unsigned(fmpz_mat_entry(A, i, i), state, b);
        }

        fmpz_mat_snf_diagonal(S, A);

        if (!fmpz_mat_is_in_snf(S))
        {
            flint_printf("FAIL:\n");
            flint_printf("matrix not in snf!\n");
            fmpz_mat_print_pretty(A); flint_printf("\n\n");
            fmpz_mat_print_pretty(S); flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_mat_snf_diagonal(S2, S);
        equal = fmpz_mat_equal(S, S2);

        if (!equal)
        {
            flint_printf("FAIL:\n");
            flint_printf("snf of a matrix in snf should be the same!\n");
            fmpz_mat_print_pretty(A); flint_printf("\n\n");
            fmpz_mat_print_pretty(S); flint_printf("\n\n");
            fmpz_mat_print_pretty(S2); flint_printf("\n\n");
            fflush(stdout);
            flint_abort();
        }

        fmpz_mat_clear(S2);
        fmpz_mat_clear(S);
        fmpz_mat_clear(A);
    }

    TEST_FUNCTION_END(state);
}
