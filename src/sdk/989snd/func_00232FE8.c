// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int snd_SendIOPCommandAndWait__FiiPc(int, int, void *);
void func_00232FE8(int a, int b) { int buf[2]; buf[0] = a; buf[1] = b; snd_SendIOPCommandAndWait__FiiPc(0x5A, 8, buf); }
