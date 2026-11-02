// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern void *(*get_eh_context)(void);
void *eh_context_static(void);
void *eh_context_initialize(void)
{
    get_eh_context = eh_context_static;
    return (*get_eh_context)();
}
