// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: snd_DoExternCall without waiting for the reply (command 0x4C, NoWait); stream-sound stop / pause / continue /
// set-params. Name inferred (989snd style).
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_DoExternCallNoWait(int a, int b, int c, int d, int e, int f, int g) { int buf[7]; buf[0] = a; buf[1] = b; buf[2] = c; buf[3] = d; buf[4] = e; buf[5] = f; buf[6] = g; snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(0x4C, 0x1C, buf, 0, 0); }
