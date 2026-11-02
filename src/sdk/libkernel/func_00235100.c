// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int func_00235100(void) { int s; __asm__ volatile("mfc0 %0, $12" : "=r"(s)); return (s ^ 1) & 1; }
