/*
    Copyright (C) 2010 Sebastian Pancratz
    Copyright (C) 2010 William Hart
    Copyright (C) 2011 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "ulong_extras.h"
#include "nmod_vec.h"
#include "nmod_poly.h"

void
_nmod_poly_powmod_ui_binexp(mp_ptr res, mp_srcptr poly,
                                ulong e, mp_srcptr f, slong lenf, nmod_t mod)
{
    mp_ptr T, Q;
    slong lenT, lenQ;
    int i;

    if (lenf == 2)
    {
        res[0] = n_powmod2_ui_preinv(poly[0], e, mod.n, mod.ninv);
        return;
    }

    lenT = 2 * lenf - 3;
    lenQ = FLINT_MAX(lenT - lenf + 1, 1);

    T = _nmod_vec_init(lenT + lenQ);
    Q = T + lenT;

    _nmod_vec_set(res, poly, lenf - 1);

    for (i = ((int) FLINT_BIT_COUNT(e) - 2); i >= 0; i--)
    {
        _nmod_poly_mul(T, res, lenf - 1, res, lenf - 1, mod);
        _nmod_poly_divrem(Q, res, T, 2 * lenf - 3, f, lenf, mod);

        if (e & (UWORD(1) << i))
        {
            _nmod_poly_mul(T, res, lenf - 1, poly, lenf - 1, mod);
            _nmod_poly_divrem(Q, res, T, 2 * lenf - 3, f, lenf, mod);
        }
    }

    _nmod_vec_clear(T);
}


void
nmod_poly_powmod_ui_binexp(nmod_poly_t res,
                           const nmod_poly_t poly, ulong e,
                           const nmod_poly_t f)
{
    mp_ptr p;
    slong len = poly->length;
    slong lenf = f->length;
    slong trunc = lenf - 1;
    int pcopy = 0;

    if (lenf == 0)
    {
        flint_throw(FLINT_ERROR, "Exception (nmod_poly_powmod_ui_binexp). Divide by zero.\n");
    }

    if (lenf == 1)
    {                                                                                             nmod_poly_zero(res);
        return;
    }

    if (len >= lenf)
    {
        nmod_poly_t t, r;
        nmod_poly_init_preinv(t, res->mod.n, res->mod.ninv);
        nmod_poly_init_preinv(r, res->mod.n, res->mod.ninv);
        nmod_poly_divrem(t, r, poly, f);
        nmod_poly_powmod_ui_binexp(res, r, e, f);
        nmod_poly_clear(t);
        nmod_poly_clear(r);
        return;
    }

    if (e <= 2)
    {
        if (e == UWORD(0))
        {
            nmod_poly_fit_length(res, 1);
            res->coeffs[0] = UWORD(1);
            res->length = 1;
        }
        else if (e == UWORD(1))
        {
            nmod_poly_set(res, poly);
        }
        else
            nmod_poly_mulmod(res, poly, poly, f);
        return;
    }

    if (len == 0)
    {
        nmod_poly_zero(res);
        return;
    }

    if (len < trunc)
    {
        p = _nmod_vec_init(trunc);
        flint_mpn_copyi(p, poly->coeffs, len);
        flint_mpn_zero(p + len, trunc - len);
        pcopy = 1;
    } else
        p = poly->coeffs;

    if ((res == poly && !pcopy) || (res == f))
    {
        nmod_poly_t t;
        nmod_poly_init2(t, poly->mod.n, trunc);
        _nmod_poly_powmod_ui_binexp(t->coeffs,
            p, e, f->coeffs, lenf, poly->mod);
        nmod_poly_swap(res, t);
        nmod_poly_clear(t);
    }
    else
    {
        nmod_poly_fit_length(res, trunc);
        _nmod_poly_powmod_ui_binexp(res->coeffs,
            p, e, f->coeffs, lenf, poly->mod);
    }

    if (pcopy)
        _nmod_vec_clear(p);

    res->length = trunc;
    _nmod_poly_normalise(res);
}
