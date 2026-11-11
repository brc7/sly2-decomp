// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void abort(void) __attribute__((noreturn));
// The default terminate handler: abort(). (gcc 2.95 libgcc2.c: __terminate_func starts as __default_terminate,
// which __terminate, the next function, calls through.)
void __default_terminate(void) { abort(); }
