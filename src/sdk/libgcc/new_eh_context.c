// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
struct eh_context { void *handler_label; void **dynamic_handler_chain; void *info; void *table_index; };
void *malloc(unsigned long);
void *func_002333B4(void *, int, unsigned long);
void __terminate(void);
struct eh_context *new_eh_context(void)
{
    struct eh_context *ehc;
    struct eh_full_context { struct eh_context c; void *top_elt[2]; } *ehfc =
        (struct eh_full_context *)malloc(sizeof *ehfc);
    if (!ehfc)
        __terminate();
    func_002333B4(ehfc, 0, sizeof *ehfc);
    ehc = (struct eh_context *)ehfc;
    if ((void *)(&ehc->dynamic_handler_chain) != (void *)ehc)
        ;
    ehc->dynamic_handler_chain = (void **)ehfc->top_elt;
    return ehc;
}
