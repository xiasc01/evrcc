/**********************************************************************
Each of the companies; Lucent, Motorola, Nokia, and Qualcomm (hereinafter 
referred to individually as "Source" or collectively as "Sources") do 
hereby state:

To the extent to which the Source(s) may legally and freely do so, the 
Source(s), upon submission of a Contribution, grant(s) a free, 
irrevocable, non-exclusive, license to the Third Generation Partnership 
Project 2 (3GPP2) and its Organizational Partners: ARIB, CCSA, TIA, TTA, 
and TTC, under the Source's copyright or copyright license rights in the 
Contribution, to, in whole or in part, copy, make derivative works, 
perform, display and distribute the Contribution and derivative works 
thereof consistent with 3GPP2's and each Organizational Partner's 
policies and procedures, with the right to (i) sublicense the foregoing 
rights consistent with 3GPP2's and each Organizational Partner's  policies 
and procedures and (ii) copyright and sell, if applicable) in 3GPP2's name 
or each Organizational Partner's name any 3GPP2 or transposed Publication 
even though this Publication may contain the Contribution or a derivative 
work thereof.  The Contribution shall disclose any known limitations on 
the Source's rights to license as herein provided.

When a Contribution is submitted by the Source(s) to assist the 
formulating groups of 3GPP2 or any of its Organizational Partners, it 
is proposed to the Committee as a basis for discussion and is not to 
be construed as a binding proposal on the Source(s).  The Source(s) 
specifically reserve(s) the right to amend or modify the material 
contained in the Contribution. Nothing contained in the Contribution 
shall, except as herein expressly provided, be construed as conferring 
by implication, estoppel or otherwise, any license or right under (i) 
any existing or later issuing patent, whether or not the use of 
information in the document necessarily employs an invention of any 
existing or later issued patent, (ii) any copyright, (iii) any 
trademark, or (iv) any other intellectual property right.

With respect to the Software necessary for the practice of any or 
all Normative portions of the Enhanced Variable Rate Codec (EVRC) as 
it exists on the date of submittal of this form, should the EVRC be 
approved as a Specification or Report by 3GPP2, or as a transposed 
Standard by any of the 3GPP2's Organizational Partners, the Source(s) 
state(s) that a worldwide license to reproduce, use and distribute the 
Software, the license rights to which are held by the Source(s), will 
be made available to applicants under terms and conditions that are 
reasonable and non-discriminatory, which may include monetary compensation, 
and only to the extent necessary for the practice of any or all of the 
Normative portions of the EVRC or the field of use of practice of the 
EVRC Specification, Report, or Standard.  The statement contained above 
is irrevocable and shall be binding upon the Source(s).  In the event 
the rights of the Source(s) in and to copyright or copyright license 
rights subject to such commitment are assigned or transferred, the 
Source(s) shall notify the assignee or transferee of the existence of 
such commitments.
*******************************************************************/
 
/*======================================================================*/
/*     Enhanced Variable Rate Codec - Bit-Exact C Specification         */
/*     Copyright (C) 1997-1998 Telecommunications Industry Association. */
/*     All rights reserved.                                             */
/*----------------------------------------------------------------------*/
/* Note:  Reproduction and use of this software for the design and      */
/*     development of North American Wideband CDMA Digital              */
/*     Cellular Telephony Standards is authorized by the TIA.           */
/*     The TIA does not authorize the use of this software for any      */
/*     other purpose.                                                   */
/*                                                                      */
/*     The availability of this software does not provide any license   */
/*     by implication, estoppel, or otherwise under any patent rights   */
/*     of TIA member companies or others covering any use of the        */
/*     contents herein.                                                 */
/*                                                                      */
/*     Any copies of this software or derivative works must include     */
/*     this and all other proprietary notices.                          */
/*======================================================================*/
/*  Module:     putacbc.c                                                */
/*  Memory Usage:                                                       */
/*      ROM:                0                                           */
/*      Static/Global RAM:  0                                           */
/*      Stack/Local RAM:    6                                           */
/*----------------------------------------------------------------------*/
#include "macro.h"
#include <stdio.h>
#include "mathadv.h"
#include "mathdp31.h"
//#include "mathevrc.h"
#include "dsp_math.h"

/* put acbc()
 * puts unscaled adaptive codebook contribution in place
 * assumes that pitch is known at dpl+extra, and dpl+extra+subframel
 */
void
    putacbc(INT16 *exctation,	/* int ptr */
			INT16 *input,		/* int ptr */
			INT16 dpl,			/* int */
			INT16 subframel,	/* int */
			INT16 extra,		/* int */
			INT32 *delay3,		/* (int + frac) ptr */
			INT16 freq,			/* fractional */
			INT16 prec			/* int */
)
{
	INT32 ldenom;
	INT32 locdelay;
	register int i;
	INT16 dpr;
	INT32 ltemp;
	INT32 invsubframel;
/*
 * subframel should only be 54 or 53.
 */
	if (subframel == 53)
		invsubframel = 1296593899;
	else						/* subframel == 54 */
		invsubframel = 1272582903;

	dpr = dpl + subframel;

	locdelay = L_shl(delay3[0], 8);
	bl_intrp(&exctation[dpl], &input[dpl], locdelay, freq, prec);

	ltemp = L_sub(delay3[1], delay3[0]);
	ltemp = L_shl(ltemp, 3);

	ldenom = L_mpy_ll(invsubframel, ltemp);

	for (i = dpl + 1; i < dpr; i++)
	{
		locdelay = L_add(locdelay, ldenom);

		bl_intrp(&exctation[i], &input[i], locdelay, freq, prec);
	}

	locdelay = L_shl(delay3[1], 8);
	bl_intrp(&exctation[dpr], &input[dpr], locdelay, freq, prec);

	ltemp = L_sub(delay3[2], delay3[1]);
	ltemp = L_shl(ltemp, 3);

	ldenom = L_mpy_ll(invsubframel, ltemp);

	for (i = dpr + 1; i < dpr + extra; i++)
	{
		locdelay = L_add(locdelay, ldenom);

		bl_intrp(&exctation[i], &input[i], locdelay, freq, prec);
	}
}
