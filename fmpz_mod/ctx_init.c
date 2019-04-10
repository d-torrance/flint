/*
    Copyright (C) 2019 Daniel Schultz

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "fmpz_mod.h"

void fmpz_mod_ctx_init(fmpz_mod_ctx_t ctx, const fmpz_t n)
{
    mp_bitcnt_t bits;

    if  (fmpz_sgn(n) <= 0)
    {
        flint_throw(FLINT_ERROR, "Nonpositive modulus in fmpz_mod_ctx_init");
    }

    fmpz_init_set(ctx->n, n);
    fmpz_preinvn_init(ctx->ninv, n);

    bits = fmpz_bits(n);

    if (bits <= FLINT_BITS)
    {
        ctx->add_fxn = fmpz_mod_add1;
        ctx->sub_fxn = fmpz_mod_sub1;
        ctx->mul_fxn = fmpz_mod_mul1;
        nmod_init(&ctx->mod, fmpz_get_ui(n));
    }
    else if (bits <= 2*FLINT_BITS)
    {
        fmpz_get_uiui(ctx->n_limbs + 1, ctx->n_limbs + 0, n);
        ctx->n_limbs[2] = 0;

        /* n = 2^FLINT_BITS must be special case */
        if (ctx->n_limbs[1] == 1 && ctx->n_limbs[0] == 0)
        {
            ctx->add_fxn = fmpz_mod_add2s;
            ctx->sub_fxn = fmpz_mod_sub2s;
            ctx->mul_fxn = fmpz_mod_mul2s;
        }
        else
        {
            fmpz_t t;
            fmpz_init(t);
            fmpz_one(t);
            fmpz_mul_2exp(t, t, 4*FLINT_BITS);
            fmpz_tdiv_q(t, t, n);
            fmpz_get_uiuiui(ctx->ninv_limbs + 2, ctx->ninv_limbs + 1, ctx->ninv_limbs + 0, t);
            fmpz_clear(t);
            FLINT_ASSERT(ctx->ninv_limbs[2] != 0);
            ctx->add_fxn = fmpz_mod_add2;
            ctx->sub_fxn = fmpz_mod_sub2;
            ctx->mul_fxn = fmpz_mod_mul2;
        }
    }
    else
    {
        ctx->n_limbs[0] = 0;
        ctx->n_limbs[1] = 0;
        ctx->n_limbs[2] = 0;
        ctx->add_fxn = fmpz_mod_addN;
        ctx->sub_fxn = fmpz_mod_subN;
        ctx->mul_fxn = fmpz_mod_mulN;
    }
}
