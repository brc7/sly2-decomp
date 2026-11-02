// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
struct eh_context { void *handler_label; void **dynamic_handler_chain; void *info; void *table_index; };
void *func_002333B4(void *, int, unsigned long);
extern struct eh_context D_00450258;
extern int D_00450268;
extern void *D_00450270[2];
struct eh_context *eh_context_static(void)
{
    if (!D_00450268) {
        D_00450268 = 1;
        func_002333B4(&D_00450258, 0, sizeof D_00450258);
        D_00450258.dynamic_handler_chain = D_00450270;
    }
    return &D_00450258;
}
