/*
    Copyright (C) 2020 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "fq_nmod_mpoly_factor.h"

/*
    either give a non-trivial split a = f*g
    or establish that a is primitive wrt all variables

    return:
        1: split a = f*g
        0: a is primitive wrt all variables (f & g undefined)
       -1: failed
*/
static int _split(
    fq_nmod_mpoly_t f,
    fq_nmod_mpoly_t g,
    fq_nmod_mpoly_t a, fmpz_t a_vars_left,
    const fq_nmod_mpoly_ctx_t ctx,
    fq_nmod_mpoly_univar_struct * u,   /* temp */
    slong * vars)
{
    slong i, j, v;
    slong nvars = ctx->minfo->nvars;
    slong mvars = 0;

    for (v = 0; v < nvars; v++)
    {
        if (!fmpz_tstbit(a_vars_left, v))
            continue;

        fq_nmod_mpoly_to_univar(u + v, a, v, ctx);
        vars[mvars] = v;
        mvars++;
    }

    if (mvars < 1)
        return 0;

    /* sort vars by decreasing length */
    for (i = 1; i < mvars; i++)
        for (j = i; j > 0 && u[vars[j]].length > u[vars[j - 1]].length; j--)
            FLINT_SWAP(slong, vars[j], vars[j - 1]);

    for (i = 0; i < mvars; i++)
    {
        v = vars[i];

        FLINT_ASSERT(fmpz_tstbit(a_vars_left, v));
        fmpz_clrbit(a_vars_left, v);

        if (u[v].length < 2)
        {
            FLINT_ASSERT(u[v].length == 1);
            FLINT_ASSERT(fmpz_is_zero(u[v].exps + 0));
            continue;
        }

        if (!_fq_nmod_mpoly_vec_content_mpoly(g, u[v].coeffs, u[v].length, ctx))
            return -1;

        if (g->length < 2)
        {
            FLINT_ASSERT(fq_nmod_mpoly_is_one(g, ctx));
            continue;
        }

        fq_nmod_mpoly_divides(f, a, g, ctx);
        FLINT_ASSERT(f->length > 1);

        return 1;
    }

    return 0;
}


/* return factors that are primitive wrt each variable */
int fq_nmod_mpoly_factor_content(
    fq_nmod_mpoly_factor_t f,
    const fq_nmod_mpoly_t A,
    const fq_nmod_mpoly_ctx_t ctx)
{
    int success;
    slong nvars = ctx->minfo->nvars;
    slong v;
    fq_nmod_mpoly_univar_struct * u;
    fq_nmod_mpoly_factor_t g; /* exponents are bitsets */
    slong * vars;

    f->num = 0;
    if (fq_nmod_mpoly_is_fq_nmod(A, ctx))
    {
		fq_nmod_mpoly_get_fq_nmod(f->constant, A, ctx);
        return 1;
    }

    vars = FLINT_ARRAY_ALLOC(nvars, slong);
    fq_nmod_mpoly_factor_init(g, ctx);
    u = FLINT_ARRAY_ALLOC(nvars, fq_nmod_mpoly_univar_struct);
    for (v = 0; v < nvars; v++)
        fq_nmod_mpoly_univar_init(u + v, ctx);

    /* remove leading coefficient */
    FLINT_ASSERT(A->length > 0);
    n_fq_get_fq_nmod(f->constant, A->coeffs + 0, ctx->fqctx);
	fq_nmod_mpoly_factor_fit_length(g, nvars, ctx);
	fq_nmod_mpoly_make_monic(g->poly + 0, A, ctx);

    /* remove monomial divisors */
    mpoly_remove_var_powers(g->exp, g->poly[0].exps, g->poly[0].bits,
                                                g->poly[0].length, ctx->minfo);
    for (v = 0; v < nvars; v++)
    {
        if (fmpz_is_zero(g->exp + v))
            continue;

        fq_nmod_mpoly_factor_fit_length(f, f->num + 1, ctx);
        fq_nmod_mpoly_gen(f->poly + f->num, v, ctx);
        fmpz_swap(f->exp + f->num, g->exp + v);
        f->num++;
    }

    /* done if g->poly[0] is constant */
    if (g->poly[0].length == 1)
    {
        FLINT_ASSERT(fq_nmod_mpoly_is_one(g->poly + 0, ctx));
        success = 1;
        goto cleanup;
    }

    /* g has length one and no variable has been checked yet */
    fmpz_one(g->exp + 0);
    fmpz_mul_2exp(g->exp + 0, g->exp + 0, nvars);
    fmpz_sub_ui(g->exp + 0, g->exp + 0, 1);
	g->num = 1;

    while (g->num > 0)
    {
        slong t = g->num - 1;

        fq_nmod_mpoly_factor_fit_length(g, t + 3, ctx);
        success = _split(g->poly + t + 2, g->poly + t + 1, g->poly + t,
                                                     g->exp + t, ctx, u, vars);
        if (success < 0)
        {
            success = 0;
            goto cleanup;
        }
        else if (success == 0)
        {
            FLINT_ASSERT(!fq_nmod_mpoly_is_fq_nmod(g->poly + t, ctx));

            fq_nmod_mpoly_factor_fit_length(f, f->num + 1, ctx);
            fq_nmod_mpoly_swap(f->poly + f->num, g->poly + t, ctx);
            fmpz_one(f->exp + f->num);
            f->num++;
            g->num = t;
        }
        else
        {
            FLINT_ASSERT(!fq_nmod_mpoly_is_fq_nmod(g->poly + t + 1, ctx));
            FLINT_ASSERT(!fq_nmod_mpoly_is_fq_nmod(g->poly + t + 2, ctx));

            fq_nmod_mpoly_swap(g->poly + t, g->poly + t + 2, ctx);
            fmpz_set(g->exp + t + 1, g->exp + t);
            g->num = t + 2;
        }
    }

    success = 1;

cleanup:

    fq_nmod_mpoly_factor_clear(g, ctx);
    for (v = 0; v < nvars; v++)
        fq_nmod_mpoly_univar_clear(u + v, ctx);
    flint_free(u);
    flint_free(vars);

    FLINT_ASSERT(!success || fq_nmod_mpoly_factor_matches(A, f, ctx));

    return success;
}

