/*
    Copyright (C) 2016 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "acb.h"
#include "acb_hypgeom.h"

static void
airy_recurrence(acb_ptr ai, const acb_t z, slong len, slong prec)
{
    slong k;

    if (len >= 3)
    {
        acb_mul(ai + 2, ai, z, prec);
        acb_mul_2exp_si(ai + 2, ai + 2, -1);
    }

    for (k = 3; k < len; k++)
    {
        acb_mul(ai + k, ai + k - 2, z, prec);
        acb_add(ai + k, ai + k, ai + k - 3, prec);
        acb_div_ui(ai + k, ai + k, k * (k - 1), prec);
    }
}

void
acb_hypgeom_airy_jet(acb_ptr ai, acb_ptr bi, const acb_t z, slong len, slong prec)
{
    if (len <= 0)
        return;

    if (len == 1)
    {
        acb_hypgeom_airy(ai, NULL, bi, NULL, z, prec);
        return;
    }

    acb_hypgeom_airy(ai, ai ? (ai + 1) : NULL, bi, bi ? (bi + 1) : NULL, z, prec);

    if (ai != NULL) airy_recurrence(ai, z, len, prec);
    if (bi != NULL) airy_recurrence(bi, z, len, prec);
}

