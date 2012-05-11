	.section .data
	.global filter
	.type filter @function
	#atention:
	#(A) last [(width*height) mod 8] bytes are not negated
filter:
	pushq %rbp
	mov %rsp, %rbp
	mov %esi, %eax
	mov %edx, %ebx
	imul %ebx #result in eax OR rax

	#for (int i=0		; i<iterations; i+=step_size)
	#<iterations dest=%rcx>
	mov %rcx, %r9 
	idiv %r9 #iterations in c register
	mov %rax, %rcx #ready for looping
	shr $3, %rax
	shl $3, %rax #2 lines doing mod 3 due to (A) restriction
			#addr buf is in %rdi
	#</iterations>
	#<step size dest=%r10>
	mov %r9, %rax
	mov $8, %r12
	imul %r12
	mov %rax, %r10
	#</step size>
	#<loop_body>
	mov $0, %r11
proceed_8_color_bytes:
	movq (%rdi, %r11, 8), %mm0

	mov $0, %r12
	not %r12
	movq %r12, %mm1

	pandn %mm1, %mm0 
	movq %mm0, (%rdi, %r11, 8)
	add $1, %r11
loop proceed_8_color_bytes
	#</loop_body>
	nop
	emms
	leaveq #==movq %rbp, %rsp popq %rbp
	ret

#pandn #blum page 494 ; dest := dest and (not source); http://webster.cs.ucr.edu/AoA/Windows/HTML/TheMMXInstructionSeta2.html
