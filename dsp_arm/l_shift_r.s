.text
.align 4
.code 32
.global L_shift_r
.type L_shift_r, %function

L_shift_r:
	MOV		r2,r0
	CMP		r0,#0
	BEQ		END
	CMP		r1,#0
	BEQ		END
	BGT		L_SHIFT

R_SHIFT:
	SUB		r1,#1
	EOR		r1,r1,r1, ASR # 31

	CMP		r1,#31
	MOVGT	r0,#0
	BGT		END

	ASRS	r0,r0,r1
	ADDCS	r0,r0,#1
	B		END

L_SHIFT:
	CMP		r1,#31
	BGT		OVERFLOW

	LSL		r0,r0,r1
	CMP		r2,r0,ASR r1
	BEQ		END
	B		OVERFLOW
	
OVERFLOW:
	CMP		r2,#0
	MOVGT	r0,#0x7FFFFFFF
	MOVLT	r0,#0x80000000
	B		END

END:
	BX		lr

