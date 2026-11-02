// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern void *(*get_eh_context)(void);
void *__get_dynamic_handler_chain(void) { return (char *)(*get_eh_context)() + 4; }
