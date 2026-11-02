// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int snd_SendIOPCommandAndWait__FiiPc(int, int, void *);
void snd_GetMasterVolume(int a) { int b[4]; b[0] = a; snd_SendIOPCommandAndWait__FiiPc(10, 4, b); }
