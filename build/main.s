	.file	"main.c"
	.text
#APP
	jmp main
	
#NO_APP
	.type	_Z5sleepj.part.0, @function
_Z5sleepj.part.0:
.LFB19:
	.cfi_startproc
.L2:
	testl	%eax, %eax
	je	.L5
#APP
# 106 "../include/lib/asm.h" 1
	hlt
# 0 "" 2
#NO_APP
	decl	%eax
	jmp	.L2
.L5:
	ret
	.cfi_endproc
.LFE19:
	.size	_Z5sleepj.part.0, .-_Z5sleepj.part.0
	.globl	_Z4insbtPhj
	.type	_Z4insbtPhj, @function
_Z4insbtPhj:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	8(%ebp), %edx
	movl	12(%ebp), %ecx
	movl	16(%ebp), %ebx
	addl	%ecx, %ebx
.L7:
	cmpl	%ebx, %ecx
	je	.L10
#APP
# 39 "../include/lib/asm.h" 1
	inb %dx, %al
# 0 "" 2
#NO_APP
	movb	%al, (%ecx)
	incl	%ecx
	jmp	.L7
.L10:
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	_Z4insbtPhj, .-_Z4insbtPhj
	.globl	_Z4inswtPtj
	.type	_Z4inswtPtj, @function
_Z4inswtPtj:
.LFB7:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	8(%ebp), %edx
	xorl	%ecx, %ecx
.L12:
	cmpl	16(%ebp), %ecx
	je	.L15
#APP
# 51 "../include/lib/asm.h" 1
	inw %dx, %ax
# 0 "" 2
#NO_APP
	movl	12(%ebp), %ebx
	movw	%ax, (%ebx,%ecx,2)
	incl	%ecx
	jmp	.L12
.L15:
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	_Z4inswtPtj, .-_Z4inswtPtj
	.globl	_Z4insltPjj
	.type	_Z4insltPjj, @function
_Z4insltPjj:
.LFB8:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	8(%ebp), %edx
	xorl	%ecx, %ecx
.L17:
	cmpl	16(%ebp), %ecx
	je	.L20
#APP
# 63 "../include/lib/asm.h" 1
	inl %dx, %eax
# 0 "" 2
#NO_APP
	movl	12(%ebp), %ebx
	movl	%eax, (%ebx,%ecx,4)
	incl	%ecx
	jmp	.L17
.L20:
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	_Z4insltPjj, .-_Z4insltPjj
	.globl	_Z5sleepj
	.type	_Z5sleepj, @function
_Z5sleepj:
.LFB9:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	8(%ebp), %eax
	testl	%eax, %eax
	je	.L21
	popl	%ebp
	.cfi_remember_state
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	jmp	_Z5sleepj.part.0
.L21:
	.cfi_restore_state
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	_Z5sleepj, .-_Z5sleepj
	.globl	_Z9ide_writehhh
	.type	_Z9ide_writehhh, @function
_Z9ide_writehhh:
.LFB10:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movl	8(%ebp), %edx
	movl	12(%ebp), %ebx
	movb	16(%ebp), %al
	leal	-8(%ebx), %ecx
	cmpb	$3, %cl
	ja	.L25
	movb	%al, -9(%ebp)
	movzbl	%dl, %esi
	pushl	%eax
	movb	channels+6(,%esi,8), %dl
	orl	$-128, %edx
	movzbl	%dl, %edx
	pushl	%edx
	pushl	$12
	pushl	%esi
	call	_Z9ide_writehhh
	movl	channels(,%esi,8), %edx
	movzbl	%bl, %ebx
	leal	-6(%edx,%ebx), %edx
	movb	-9(%ebp), %al
#APP
# 7 "../include/lib/asm.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
	addl	$16, %esp
	movzbl	channels+6(,%esi,8), %eax
	movl	%eax, 16(%ebp)
	movl	$12, 12(%ebp)
	movl	%esi, 8(%ebp)
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	jmp	_Z9ide_writehhh
.L25:
	.cfi_restore_state
	cmpb	$7, %bl
	ja	.L26
	movzbl	%dl, %ecx
	movzbl	%bl, %edx
	addl	channels(,%ecx,8), %edx
	jmp	.L30
.L26:
	cmpb	$13, %bl
	ja	.L28
	movzbl	%dl, %edx
	movw	channels+2(,%edx,8), %dx
	movzbl	%bl, %ebx
	leal	-10(%edx,%ebx), %edx
	jmp	.L30
.L28:
	cmpb	$21, %bl
	ja	.L24
	movzbl	%dl, %edx
	movl	channels+4(,%edx,8), %edx
	movzbl	%bl, %ebx
	leal	-14(%edx,%ebx), %edx
.L30:
#APP
# 7 "../include/lib/asm.h" 1
	outb %al, %dx
# 0 "" 2
#NO_APP
.L24:
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE10:
	.size	_Z9ide_writehhh, .-_Z9ide_writehhh
	.globl	_Z8ide_readhh
	.type	_Z8ide_readhh, @function
_Z8ide_readhh:
.LFB11:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	subl	$16, %esp
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movl	8(%ebp), %edx
	movl	12(%ebp), %ebx
	leal	-8(%ebx), %ecx
	cmpb	$3, %cl
	ja	.L32
	movzbl	%dl, %esi
	pushl	%eax
	movb	channels+6(,%esi,8), %al
	orl	$-128, %eax
	movzbl	%al, %eax
	pushl	%eax
	pushl	$12
	pushl	%esi
	call	_Z9ide_writehhh
	movl	channels(,%esi,8), %edx
	movzbl	%bl, %ebx
	leal	-6(%edx,%ebx), %edx
#APP
# 39 "../include/lib/asm.h" 1
	inb %dx, %al
# 0 "" 2
#NO_APP
	movb	%al, -9(%ebp)
	addl	$12, %esp
	movzbl	channels+6(,%esi,8), %edx
	pushl	%edx
	pushl	$12
	pushl	%esi
	call	_Z9ide_writehhh
	addl	$16, %esp
	movb	-9(%ebp), %al
	jmp	.L31
.L32:
	cmpb	$7, %bl
	ja	.L34
	movzbl	%dl, %eax
	movzbl	%bl, %edx
	addl	channels(,%eax,8), %edx
	jmp	.L38
.L34:
	cmpb	$13, %bl
	ja	.L35
	movzbl	%dl, %edx
	movw	channels+2(,%edx,8), %dx
	movzbl	%bl, %ebx
	leal	-10(%edx,%ebx), %edx
	jmp	.L38
.L35:
	cmpb	$21, %bl
	ja	.L31
	movzbl	%dl, %edx
	movl	channels+4(,%edx,8), %edx
	movzbl	%bl, %ebx
	leal	-14(%edx,%ebx), %edx
.L38:
#APP
# 39 "../include/lib/asm.h" 1
	inb %dx, %al
# 0 "" 2
#NO_APP
.L31:
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE11:
	.size	_Z8ide_readhh, .-_Z8ide_readhh
	.globl	_Z15ide_read_bufferhhPjj
	.type	_Z15ide_read_bufferhhPjj, @function
_Z15ide_read_bufferhhPjj:
.LFB12:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$28, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	movl	8(%ebp), %ebx
	movl	12(%ebp), %eax
	movl	16(%ebp), %esi
	movl	20(%ebp), %edi
	leal	-8(%eax), %ecx
	movb	%cl, -25(%ebp)
	cmpb	$3, %cl
	ja	.L40
	movl	%eax, -32(%ebp)
	movzbl	%bl, %ecx
	pushl	%eax
	movb	channels+6(,%ecx,8), %dl
	orl	$-128, %edx
	movzbl	%dl, %edx
	pushl	%edx
	pushl	$12
	pushl	%ecx
	call	_Z9ide_writehhh
	addl	$16, %esp
	movl	-32(%ebp), %eax
.L40:
#APP
# 59 "../include/lib/rom/ide.h" 1
	pushw %es       
	movw  %ds, %ax 
	movw  %ax, %es 
# 0 "" 2
#NO_APP
	cmpb	$7, %al
	ja	.L41
	pushl	%ecx
	pushl	%edi
	pushl	%esi
	movzbl	%bl, %edx
	movzbl	%al, %eax
	addl	channels(,%edx,8), %eax
	jmp	.L47
.L41:
	cmpb	$11, %al
	ja	.L43
	pushl	%edx
	pushl	%edi
	pushl	%esi
	movzbl	%bl, %edx
	movl	channels(,%edx,8), %edx
	movzbl	%al, %eax
	leal	-6(%edx,%eax), %eax
	jmp	.L47
.L43:
	cmpb	$13, %al
	ja	.L44
	pushl	%ecx
	pushl	%edi
	pushl	%esi
	movzbl	%bl, %edx
	movw	channels+2(,%edx,8), %dx
	movzbl	%al, %eax
	leal	-10(%edx,%eax), %eax
	jmp	.L47
.L44:
	cmpb	$21, %al
	ja	.L42
	pushl	%edx
	pushl	%edi
	pushl	%esi
	movzbl	%bl, %edx
	movl	channels+4(,%edx,8), %edx
	movzbl	%al, %eax
	leal	-14(%edx,%eax), %eax
.L47:
	movzwl	%ax, %eax
	pushl	%eax
	call	_Z4insltPjj
	addl	$16, %esp
.L42:
#APP
# 72 "../include/lib/rom/ide.h" 1
	popw %es
# 0 "" 2
#NO_APP
	cmpb	$3, -25(%ebp)
	ja	.L39
	movzbl	%bl, %ebx
	movzbl	channels+6(,%ebx,8), %eax
	movl	%eax, 16(%ebp)
	movl	$12, 12(%ebp)
	movl	%ebx, 8(%ebp)
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_remember_state
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	jmp	_Z9ide_writehhh
.L39:
	.cfi_restore_state
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE12:
	.size	_Z15ide_read_bufferhhPjj, .-_Z15ide_read_bufferhhPjj
	.globl	_Z11ide_pollinghj
	.type	_Z11ide_pollinghj, @function
_Z11ide_pollinghj:
.LFB13:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movl	$4, %esi
	movzbl	8(%ebp), %ebx
.L49:
	pushl	%ecx
	pushl	%ecx
	pushl	$12
	pushl	%ebx
	call	_Z8ide_readhh
	addl	$16, %esp
	decl	%esi
	jne	.L49
.L50:
	pushl	%edx
	pushl	%edx
	pushl	$7
	pushl	%ebx
	call	_Z8ide_readhh
	addl	$16, %esp
	testb	%al, %al
	js	.L50
	cmpl	$0, 12(%ebp)
	jne	.L51
.L53:
	xorl	%edx, %edx
	jmp	.L48
.L51:
	pushl	%eax
	pushl	%eax
	pushl	$7
	pushl	%ebx
	call	_Z8ide_readhh
	addl	$16, %esp
	movb	$2, %dl
	testb	$1, %al
	jne	.L48
	movb	$1, %dl
	testb	$32, %al
	jne	.L48
	testb	$8, %al
	jne	.L53
	movb	$3, %dl
.L48:
	movb	%dl, %al
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE13:
	.size	_Z11ide_pollinghj, .-_Z11ide_pollinghj
	.globl	_Z15ide_print_errorjh
	.type	_Z15ide_print_errorjh, @function
_Z15ide_print_errorjh:
.LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movb	12(%ebp), %al
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE14:
	.size	_Z15ide_print_errorjh, .-_Z15ide_print_errorjh
	.globl	_Z14ide_initializejjjjj
	.type	_Z14ide_initializejjjjj, @function
_Z14ide_initializejjjjj:
.LFB15:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	subl	$28, %esp
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	movl	8(%ebp), %edi
	movl	12(%ebp), %ebx
	movl	16(%ebp), %ecx
	movl	20(%ebp), %edx
	movl	%edi, %eax
	andl	$-4, %eax
	xorl	%esi, %esi
	testl	%edi, %edi
	jne	.L62
	movl	$496, %esi
.L62:
	addl	%esi, %eax
	movw	%ax, channels
	movl	%ebx, %eax
	andl	$-4, %eax
	movl	$1014, %esi
	testl	%ebx, %ebx
	je	.L63
	xorl	%esi, %esi
.L63:
	addl	%esi, %eax
	movw	%ax, channels+2
	movl	%ecx, %eax
	andl	$-4, %eax
	movl	$368, %ebx
	testl	%ecx, %ecx
	je	.L64
	xorl	%ebx, %ebx
.L64:
	addl	%ebx, %eax
	movw	%ax, channels+8
	movl	%edx, %eax
	andl	$-4, %eax
	movl	$886, %ecx
	testl	%edx, %edx
	je	.L65
	xorl	%ecx, %ecx
.L65:
	addl	%ecx, %eax
	movw	%ax, channels+10
	movl	24(%ebp), %eax
	andl	$-4, %eax
	movw	%ax, channels+4
	addl	$8, %eax
	movw	%ax, channels+12
	pushl	%ecx
	pushl	$2
	pushl	$12
	pushl	$0
	call	_Z9ide_writehhh
	addl	$12, %esp
	pushl	$2
	pushl	$12
	pushl	$1
	call	_Z9ide_writehhh
	addl	$16, %esp
	xorl	%esi, %esi
	xorl	%ebx, %ebx
.L66:
	xorl	%edi, %edi
.L75:
	movl	%esi, %eax
	sall	$6, %eax
	movb	$0, ide_devices(%eax)
	movl	%edi, %eax
	movb	%al, -26(%ebp)
	pushl	%eax
	movl	%edi, %eax
	sall	$4, %eax
	orb	$-96, %al
	pushl	%eax
	pushl	$6
	pushl	%ebx
	call	_Z9ide_writehhh
	movl	$1, %eax
	call	_Z5sleepj.part.0
	addl	$12, %esp
	pushl	$236
	pushl	$7
	pushl	%ebx
	call	_Z9ide_writehhh
	movl	$1, %eax
	call	_Z5sleepj.part.0
	popl	%eax
	popl	%edx
	pushl	$7
	pushl	%ebx
	call	_Z8ide_readhh
	addl	$16, %esp
	testb	%al, %al
	je	.L67
.L93:
	pushl	%eax
	pushl	%eax
	pushl	$7
	pushl	%ebx
	call	_Z8ide_readhh
	addl	$16, %esp
	movb	%al, %cl
	andb	$1, %cl
	movb	%cl, -25(%ebp)
	jne	.L68
	testb	%al, %al
	js	.L93
	testb	$8, %al
	je	.L93
	jmp	.L70
.L68:
	pushl	%eax
	pushl	%eax
	pushl	$4
	pushl	%ebx
	call	_Z8ide_readhh
	movb	%al, -27(%ebp)
	popl	%edx
	popl	%ecx
	pushl	$5
	pushl	%ebx
	call	_Z8ide_readhh
	addl	$16, %esp
	movb	-27(%ebp), %dl
	cmpb	$20, %dl
	jne	.L103
	cmpb	$-21, %al
.L107:
	jne	.L67
	pushl	%eax
	pushl	$161
	pushl	$7
	pushl	%ebx
	call	_Z9ide_writehhh
	movl	$1, %eax
	call	_Z5sleepj.part.0
	addl	$16, %esp
.L70:
	pushl	$128
	pushl	$ide_buf
	pushl	$0
	pushl	%ebx
	call	_Z15ide_read_bufferhhPjj
	movl	%esi, %edx
	sall	$6, %edx
	leal	ide_devices(%edx), %eax
	movb	$1, ide_devices(%edx)
	movzbl	-25(%ebp), %edx
	movw	%dx, 4(%eax)
	movb	%bl, 1(%eax)
	movb	-26(%ebp), %cl
	movb	%cl, 2(%eax)
	movl	ide_buf, %edx
	movw	%dx, 6(%eax)
	movw	ide_buf+98, %dx
	movw	%dx, 8(%eax)
	movl	ide_buf+164, %ecx
	movl	%ecx, 12(%eax)
	addl	$16, %esp
	movl	ide_buf+120, %edx
	btl	$26, %ecx
	jnc	.L73
	movl	ide_buf+200, %edx
.L73:
	movl	%esi, %eax
	sall	$6, %eax
	movl	%edx, ide_devices+16(%eax)
	xorl	%edx, %edx
.L74:
	movb	ide_buf+55(%edx), %cl
	movb	%cl, ide_devices+20(%eax)
	movb	ide_buf+54(%edx), %cl
	movb	%cl, ide_devices+21(%eax)
	addl	$2, %edx
	addl	$2, %eax
	cmpl	$40, %edx
	jne	.L74
	movl	%esi, %eax
	sall	$6, %eax
	movb	$0, ide_devices+60(%eax)
	incl	%esi
.L67:
	decl	%edi
	je	.L108
	movl	$1, %edi
	jmp	.L75
.L108:
	decl	%ebx
	je	.L61
	movl	$1, %ebx
	jmp	.L66
.L103:
	cmpb	$-106, %al
	jne	.L67
	cmpb	$105, %dl
	jmp	.L107
.L61:
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE15:
	.size	_Z14ide_initializejjjjj, .-_Z14ide_initializejjjjj
	.globl	_Z10print_charhhhht
	.type	_Z10print_charhhhht, @function
_Z10print_charhhhht:
.LFB16:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	.cfi_offset 7, -12
	movzbl	20(%ebp), %eax
	movzwl	24(%ebp), %edx
	imull	%edx, %eax
	movzbl	16(%ebp), %edx
	leal	376832(%eax,%edx), %edi
	addl	%edi, %edi
	movl	8(%ebp), %eax
	movl	12(%ebp), %ecx
#APP
# 6 "../include/lib/graphics/text/text.h" 1
	mov %cl, %ah
	
# 0 "" 2
#NO_APP
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE16:
	.size	_Z10print_charhhhht, .-_Z10print_charhhhht
	.globl	_Z5printPhhhh
	.type	_Z5printPhhhh, @function
_Z5printPhhhh:
.LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	.cfi_offset 7, -12
	movl	8(%ebp), %edx
	movb	12(%ebp), %cl
	movzbl	16(%ebp), %edi
	movzbl	20(%ebp), %eax
	imull	$80, %eax, %eax
	leal	376832(%edi,%eax), %edi
	addl	%edi, %edi
.L112:
	movb	(%edx), %al
	testb	%al, %al
	je	.L115
#APP
# 6 "../include/lib/graphics/text/text.h" 1
	mov %cl, %ah
	
# 0 "" 2
#NO_APP
	incl	%edx
	jmp	.L112
.L115:
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE17:
	.size	_Z5printPhhhh, .-_Z5printPhhhh
	.section	.text.startup,"ax",@progbits
	.globl	main
	.type	main, @function
main:
.LFB18:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	.cfi_offset 7, -12
	movb	$53, %al
	movb	$2, %cl
	movl	$754960, %edi
#APP
# 6 "../include/lib/graphics/text/text.h" 1
	mov %cl, %ah
	
# 0 "" 2
#NO_APP
	popl	%edi
	.cfi_restore 7
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE18:
	.size	main, .-main
	.local	_ZL15ide_irq_invoked
	.comm	_ZL15ide_irq_invoked,1,1
	.globl	ide_buf
	.bss
	.align 32
	.type	ide_buf, @object
	.size	ide_buf, 2048
ide_buf:
	.zero	2048
	.globl	ide_devices
	.align 32
	.type	ide_devices, @object
	.size	ide_devices, 256
ide_devices:
	.zero	256
	.globl	channels
	.align 4
	.type	channels, @object
	.size	channels, 16
channels:
	.zero	16
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
