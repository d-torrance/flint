/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "acb_mat.h"

void
acb_mat_sub(acb_mat_t res,
        const acb_mat_t mat1, const acb_mat_t mat2, slong prec)
{
    slong i, j;

    for (i = 0; i < acb_mat_nrows(mat1); i++)
        for (j = 0; j < acb_mat_ncols(mat1); j++)
            acb_sub(acb_mat_entry(res, i, j),
                acb_mat_entry(mat1, i, j),
                acb_mat_entry(mat2, i, j), prec);
}
