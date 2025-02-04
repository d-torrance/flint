/*
    Copyright (C) 2011 William Hart
    Copyright (C) 2012 Sebastian Pancratz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpz_mod.h"
#include "fmpz_mod_poly.h"

int _fmpz_mod_poly_invmod(fmpz *A,
                          const fmpz *B, slong lenB,
                          const fmpz *P, slong lenP, const fmpz_mod_ctx_t ctx)
{
    fmpz *G;
    slong lenG;

    FMPZ_VEC_NORM(B, lenB);

    G = _fmpz_vec_init(lenB);

    lenG = _fmpz_mod_poly_gcdinv(G, A, B, lenB, P, lenP, ctx);

    if (lenG == 1 && !fmpz_is_one(G + 0))
    {
        fmpz_t invG;

        fmpz_init(invG);
        fmpz_invmod(invG, G + 0, fmpz_mod_ctx_modulus(ctx));
        _fmpz_mod_poly_scalar_mul_fmpz(A, A, lenP - 1, invG, ctx);
        fmpz_clear(invG);
    }

    _fmpz_vec_clear(G, lenB);

    return (lenG == 1);
}

int fmpz_mod_poly_invmod(fmpz_mod_poly_t A, const fmpz_mod_poly_t B,
                             const fmpz_mod_poly_t P, const fmpz_mod_ctx_t ctx)
{
    const slong lenB = B->length, lenP = P->length;
    fmpz *t;
    int ans;

    if (lenP < 2)
    {
        flint_throw(FLINT_ERROR, "Exception (fmpz_mod_poly_invmod). lenP < 2.\n");
    }
    if (lenB == 0)
    {
        fmpz_mod_poly_zero(A, ctx);
        return 0;
    }
    if (lenB >= lenP)
    {
        fmpz_mod_poly_t T;

        fmpz_mod_poly_init(T, ctx);
        fmpz_mod_poly_rem(T, B, P, ctx);
        ans = fmpz_mod_poly_invmod(A, T, P, ctx);
        fmpz_mod_poly_clear(T, ctx);
        return ans;
    }

    if (A != B && A != P)
    {
        fmpz_mod_poly_fit_length(A, lenP - 1, ctx);
        t = A->coeffs;
    }
    else
    {
        t = _fmpz_vec_init(lenP);
    }

    ans = _fmpz_mod_poly_invmod(t, B->coeffs, lenB, P->coeffs, lenP, ctx);

    if (A == B || A == P)
    {
        _fmpz_vec_clear(A->coeffs, A->alloc);
        A->coeffs = t;
        A->alloc  = lenP - 1;
    }
    _fmpz_mod_poly_set_length(A, lenP - 1);
    _fmpz_mod_poly_normalise(A);
    return ans;
}

