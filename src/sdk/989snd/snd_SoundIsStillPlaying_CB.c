// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_SoundIsStillPlaying_CB(int a, int b, int c) { int buf[1]; buf[0] = a; snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(0x19, 4, buf, b, c); }
