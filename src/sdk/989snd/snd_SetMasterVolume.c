// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_SetMasterVolume(int a, int b) { int buf[2]; buf[0] = a; buf[1] = b; snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(9, 8, buf, 0, 0); }
