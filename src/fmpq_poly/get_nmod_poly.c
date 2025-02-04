/*
    Copyright (C) 2019 Tommy Hofmann

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "ulong_extras.h"
#include "nmod_poly.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpq_poly.h"

void
_fmpq_poly_get_nmod_poly(nmod_poly_t rop, const fmpq_poly_t op)
{
    slong len = op->length;

    if (len == 0)
    {
        nmod_poly_zero(rop);
    }
    else
    {
        nmod_poly_fit_length(rop, len);
        _fmpz_vec_get_nmod_vec(rop->coeffs, op->coeffs, len, rop->mod);
        _nmod_poly_set_length(rop, len);
        _nmod_poly_normalise(rop);
    }
}

void
fmpq_poly_get_nmod_poly_den(nmod_poly_t rop, const fmpq_poly_t op, int den)
{
    _fmpq_poly_get_nmod_poly(rop, op);

    if (den)
    {
        if (!fmpz_is_one(op->den))
        {
            nmod_poly_scalar_mul_nmod(rop, rop, n_invmod(fmpz_get_nmod(op->den, rop->mod), rop->mod.n));
        }
    }
}

void
fmpq_poly_get_nmod_poly(nmod_poly_t rop, const fmpq_poly_t op)
{
    fmpq_poly_get_nmod_poly_den(rop, op, 1);
}
