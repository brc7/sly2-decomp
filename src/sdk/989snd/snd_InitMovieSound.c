// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int snd_SendIOPCommandAndWait__FiiPc(int, int, void *);
void snd_InitMovieSound(int a, int b, int c, int d, int e, int f) { int buf[6]; buf[0] = a; buf[1] = b; buf[2] = c; buf[3] = d; buf[4] = e; buf[5] = f; snd_SendIOPCommandAndWait__FiiPc(0x3B, 0x18, buf); }
