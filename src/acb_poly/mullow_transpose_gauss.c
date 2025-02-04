/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "arb_poly.h"
#include "acb_poly.h"

void
_acb_poly_mullow_transpose_gauss(acb_ptr res,
    acb_srcptr poly1, slong len1,
    acb_srcptr poly2, slong len2, slong n, slong prec)
{
    arb_ptr a, b, c, d, e, f, w;
    arb_ptr t, u, v;
    slong i;

    len1 = FLINT_MIN(len1, n);
    len2 = FLINT_MIN(len2, n);

    w = flint_malloc(sizeof(arb_struct) * (2 * (len1 + len2 + n)));
    a = w;
    b = a + len1;
    c = b + len1;
    d = c + len2;
    e = d + len2;
    f = e + n;

    t = _arb_vec_init(n);
    u = _arb_vec_init(n);
    v = _arb_vec_init(n);

    for (i = 0; i < len1; i++)
    {
        a[i] = *acb_realref(poly1 + i);
        b[i] = *acb_imagref(poly1 + i);
    }

    for (i = 0; i < len2; i++)
    {
        c[i] = *acb_realref(poly2 + i);
        d[i] = *acb_imagref(poly2 + i);
    }

    for (i = 0; i < n; i++)
    {
        e[i] = *acb_realref(res + i);
        f[i] = *acb_imagref(res + i);
    }

    _arb_vec_add(t, a, b, len1, prec);
    _arb_vec_add(u, c, d, len2, prec);

    _arb_poly_mullow(v, t, len1, u, len2, n, prec);
    _arb_poly_mullow(t, a, len1, c, len2, n, prec);
    _arb_poly_mullow(u, b, len1, d, len2, n, prec);

    _arb_vec_sub(e, t, u, n, prec);
    _arb_vec_sub(f, v, t, n, prec);
    _arb_vec_sub(f, f, u, n, prec);

    for (i = 0; i < n; i++)
    {
        *acb_realref(res + i) = e[i];
        *acb_imagref(res + i) = f[i];
    }

    _arb_vec_clear(t, n);
    _arb_vec_clear(u, n);
    _arb_vec_clear(v, n);

    flint_free(w);
}

void
acb_poly_mullow_transpose_gauss(acb_poly_t res, const acb_poly_t poly1,
                                            const acb_poly_t poly2,
                                                slong n, slong prec)
{
    slong len1, len2;

    len1 = poly1->length;
    len2 = poly2->length;

    if (len1 == 0 || len2 == 0 || n == 0)
    {
        acb_poly_zero(res);
        return;
    }

    n = FLINT_MIN((len1 + len2 - 1), n);
    len1 = FLINT_MIN(len1, n);
    len2 = FLINT_MIN(len2, n);

    if (res == poly1 || res == poly2)
    {
        acb_poly_t t;
        acb_poly_init2(t, n);
        _acb_poly_mullow_transpose_gauss(t->coeffs, poly1->coeffs, len1,
                                poly2->coeffs, len2, n, prec);
        acb_poly_swap(res, t);
        acb_poly_clear(t);
    }
    else
    {
        acb_poly_fit_length(res, n);
        _acb_poly_mullow_transpose_gauss(res->coeffs, poly1->coeffs, len1,
                                poly2->coeffs, len2, n, prec);
    }

    _acb_poly_set_length(res, n);
    _acb_poly_normalise(res);
}

