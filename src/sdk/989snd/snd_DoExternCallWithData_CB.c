// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: extern-procedure call carrying a data block (proc id, function index, size, data; command 0x68, size + 12
// bytes) with a completion callback; PlayStreamsnd sends its start message this way. Name inferred (989snd style).
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_DoExternCallWithData_CB(int a, int b, int c, int d, int e, int f) { int buf[4]; buf[0] = a; buf[1] = b; buf[2] = c; buf[3] = d; snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(0x68, c + 0xC, buf, e, f); }
