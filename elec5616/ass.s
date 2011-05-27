	.file	"assemble.c"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
#APP
# 2 "assemble.c" 1
	jmp    0x1f
	popl   %esi
	movl   %esi,0x8(%esi)
	xorl   %eax,%eax
	movb   %eax,0x7(%esi)
	movl   %eax,0xc(%esi)
	movb   $0xb,%al
	movl   %esi,%ebx
	leal   0x8(%esi),%ecx
	leal   0xc(%esi),%edx
	int    $0x80
	xorl   %ebx,%ebx
	movl   %ebx,%eax
	inc    %eax
	int    $0x80
	call   -0x24
	.string "/bin/sh"
# 0 "" 2
#NO_APP
	popl	%ebp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5"
	.section	.note.GNU-stack,"",@progbits
