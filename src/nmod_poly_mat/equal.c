/*
    Copyright (C) 2011 Fredrik Johansson
    Copyright (C) 2023 Vincent Neiger

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "nmod_poly.h"
#include "nmod_mat.h"
#include "nmod_poly_mat.h"

int
nmod_poly_mat_equal(const nmod_poly_mat_t A, const nmod_poly_mat_t B)
{
    slong i, j;

    if (A->r != B->r || A->c != B->c)
        return 0;

    for (i = 0; i < A->r; i++)
        for (j = 0; j < A->c; j++)
            if (!nmod_poly_equal(nmod_poly_mat_entry(A, i, j),
                                 nmod_poly_mat_entry(B, i, j)))
                return 0;
    return 1;
}

int
nmod_poly_mat_equal_nmod_mat(const nmod_poly_mat_t pmat,
                                const nmod_mat_t cmat)
{
    if (pmat->r != cmat->r || pmat->c != cmat->c)
        return 0;

    for (slong i = 0; i < pmat->r; i++)
        for (slong j = 0; j < pmat->c; j++)
            if (! nmod_poly_equal_nmod(nmod_poly_mat_entry(pmat, i, j),
                                       nmod_mat_entry(cmat, i, j)))
                return 0;
    return 1;
}
