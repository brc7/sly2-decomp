// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_AutoVol(int a, int b, int c, int d) { int buf[4]; buf[0] = a; buf[1] = b; buf[2] = c; buf[3] = d; snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(0x22, 0x10, buf, 0, 0); }
