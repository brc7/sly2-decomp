// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: calls a function of a registered IOP extern procedure (proc id, function index, five arguments), IOP command
// 0x4C AndWait; the game's SUKR stream-sound module is reached this way (SUKR_INIT, SUKR_STOP_ALL). Name inferred.
int snd_SendIOPCommandAndWait__FiiPc(int, int, void *);
void snd_DoExternCall(int a, int b, int c, int d, int e, int f, int g) { int buf[7]; buf[0] = a; buf[1] = b; buf[2] = c; buf[3] = d; buf[4] = e; buf[5] = f; buf[6] = g; snd_SendIOPCommandAndWait__FiiPc(0x4C, 0x1C, buf); }
