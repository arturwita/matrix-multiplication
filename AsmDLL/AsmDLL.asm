.data
	permmask2 dword 0, 4, 0, 0,      0, 0, 0, 0
.code

DllMain PROC		; ENTRY POINT FOR DLL
	mov rax, 1
	ret
DllMain ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;									INPUT DATA									;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	LEFT_MATRIX			- contains: matrix alignment, height, width and data	;
;	RIGHT_MATRIX		- contains: matrix alignment, height, width and data	;
;	RESULTANT_MATRIX	- contains: matrix alignment, height, width and data	;
;	LINE_OFFSET			- offset in both left and resultant matrix				;
;	ROWS				- number of left matrix rows to multiply				;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;							INITIAL DATA LOCALIZATION							;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	leftMatrix			rightMatrix			resultantMatrix			offset		;
;	width  = [RCX]		width  = [RDX]		width  = [R8]			R9			;
;	height = [RCX+4]	height = [RDX+4]	height = [R8+4]						;
;	data   = [RCX+8]	data   = [RDX+8]	data   = [R8+8]			rows		;
;	_align = [RCX+12]	_align = [RDX+12]	_align = [R8+12]		[RSP+40]	;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;									USED REGISTERS								;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; RSI - procedure iteration counter (esi)										;
; RBX - matrix width as counter (ebx)											;
; R9 - offset in assembler size (offset * matrix_width * size_of_float)			;
; R8 - pointer to resultant matrix data											;
; R12 - left array index														;
; R13 - right matrix index														;
; R14 - pointer to left matrix data												;
; R15 - pointer to right matrix data											;
; R10 - right matrix height as counter (R10d)									;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


multiplyAsm PROC
	
	; push values on stack to save them
	push rbx
	push rsi
	push r12
	push r13
	push r14
	push r15

	mov EAX, dword ptr [R8]					; move resultant matrix width to acumulator
	imul EAX, EAX, 4						; multiply matrix width by 4 (size of float), save to EAX
	mov R13, RAX							; move resultant matrix width * 4 to R13 
	imul R13, R9							; multiply it by offset
	mov R8, qword ptr [R8+8]				; move pointer to resultant matrix data to R8
	add R8, R13								; move index in resultant matrix to first float to save
	
	mov R14, qword ptr [RCX+8]				; move pointer to left matrix data to R14
	mov R15, qword ptr [RDX+8]				; move pointer to right matrix data to R15

	mov EAX, dword ptr [RCX]				; move matrix width to acumulator
	imul EAX, EAX, 4						; multiply matrix width by 4 (size of float), save to EAX
	imul R9D, EAX							; multiply line offset by EAX

	mov ESI, dword ptr [RSP+88]				; move ProcedureLoop counter to ESI
	
	mov R10D, dword ptr [RDX+4]				; save RightMatrixLoop counter value to R10 
	mov EDI, dword ptr [RDX+4]				; move RightMatrixLoop counter to EDI 

	xor R13, R13 							; clear R13 (index in right matrix data)
	vmovdqa YMM2, ymmword ptr permmask2		; move permutation mask 2 to YMM2

RightMatrixLoop:
	mov R12, R9								; copy initial offset in left matrix to R12
	mov EBX, dword ptr [RCX]				; move LineLoop counter to EBX

LineLoop:
	vmovdqu YMM6, ymmword ptr [R14+R12]		; move 8 floats from left matrix to YMM6
	vmovdqu YMM7, ymmword ptr [R15+R13]		; move 8 floats from right matrix to YMM7
	vmulps YMM8, YMM6, YMM7					; multiply YMM6 by YMM7, save to YMM8
	
	vhaddps YMM8, YMM8, YMM8				; horizontally add YMM8+YMM8 and save to YMM8
	vhaddps YMM8, YMM8, YMM8				; horizontally add YMM8+YMM8 and save to YMM8
	vpermd YMM8, YMM2, YMM8					; permute YMM8 using permutation mask 2
	vhaddps YMM8, YMM8, YMM8				; horizontally add YMM8+YMM8 and save to YMM8
	vaddps YMM9, YMM9, YMM8					; add YMM9+YMM8 and save to YMM9
	
	add R12, 32								; increment left array index by 8 floats
	add R13, 32								; increment right array index by 8 floats
	sub EBX, 8								; decrement LineLoop counter
	jnz LineLoop							; jump if not zero

	vmovd EDX, XMM9							; move result of whole row multiplication to EDX
	mov dword ptr [R8], EDX					; save the result to resultant matrix
	vxorps YMM9, YMM9, YMM9					; clear YMM9 register

	add R8, 4								; move index in resultant matrix to next float
	dec EDI									; decrement RightMatrixLoop counter
	jnz RightMatrixLoop						; jump if not zero

	add R9, RAX								; move offset in left matrix to next row
	mov EDI, R10D							; move RightMatrixLoop counter to EDI
	xor R13, R13 							; clear R13 (index in right matrix data)
	mov EBX, dword ptr [RCX]				; move LineLoop counter to EBX
	dec ESI									; decrement ProcedureLoop counter
	jnz LineLoop							; jump if not zero

	; pop values from stack
	pop r15
	pop r14
	pop r13
	pop r12
	pop rsi
	pop rbx

	ret										; return from procedure
multiplyAsm ENDP
end
