// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern void (*__terminate_func)(void);
void __terminate(void) { (*__terminate_func)(); }
