/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "ulong_extras.h"
#include "fmpz.h"

TEST_FUNCTION_START(fmpz_fdiv_qr_preinvn, state)
{
    int i, result;

    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpz_t a, b, c, r;
        mpz_t d, e, f, g, h, s;
        fmpz_preinvn_t inv;
        int aliasing;

        fmpz_init(a);
        fmpz_init(b);
        fmpz_init(c);
        fmpz_init(r);

        mpz_init(d);
        mpz_init(e);
        mpz_init(f);
        mpz_init(g);
        mpz_init(h);
        mpz_init(s);

        fmpz_randtest(a, state, 400);
        fmpz_randtest_not_zero(b, state, 400);

        fmpz_get_mpz(d, a);
        fmpz_get_mpz(e, b);

        fmpz_preinvn_init(inv, b);

        aliasing = n_randint(state, 5);

        if (aliasing == 0)
        {
            fmpz_fdiv_qr_preinvn(c, r, a, b, inv);
        }
        else if (aliasing == 1)
        {
            fmpz_set(c, a);
            fmpz_fdiv_qr_preinvn(c, r, c, b, inv);
        }
        else if (aliasing == 2)
        {
            fmpz_set(c, b);
            fmpz_fdiv_qr_preinvn(c, r, a, c, inv);
        }
        else if (aliasing == 3)
        {
            fmpz_set(r, a);
            fmpz_fdiv_qr_preinvn(c, r, r, b, inv);
        }
        else
        {
            fmpz_set(r, b);
            fmpz_fdiv_qr_preinvn(c, r, a, r, inv);
        }

        mpz_fdiv_qr(f, s, d, e);

        fmpz_get_mpz(g, c);
        fmpz_get_mpz(h, r);

        result = (mpz_cmp(f, g) == 0 && mpz_cmp(h, s) == 0);
        if (!result)
        {
            flint_printf("FAIL:\n");
            gmp_printf
                ("d = %Zd, e = %Zd, f = %Zd, g = %Zd, h = %Zd, s = %Zd\n", d,
                 e, f, g, h, s);
            fflush(stdout);
            flint_abort();
        }

        fmpz_preinvn_clear(inv);
        fmpz_clear(a);
        fmpz_clear(b);
        fmpz_clear(c);
        fmpz_clear(r);

        mpz_clear(d);
        mpz_clear(e);
        mpz_clear(f);
        mpz_clear(g);
        mpz_clear(h);
        mpz_clear(s);
    }

    TEST_FUNCTION_END(state);
}
