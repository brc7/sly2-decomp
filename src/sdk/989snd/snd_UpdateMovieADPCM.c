// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: tells the IOP movie-sound player that ADPCM data was DMA'd (size, buffer offset), command 0x5A AndWait;
// called by the movie audio queue after each ADPCM transfer, next to snd_*MovieSound (0x3B..0x3E). Name inferred.
int snd_SendIOPCommandAndWait__FiiPc(int, int, void *);
void snd_UpdateMovieADPCM(int a, int b) { int buf[2]; buf[0] = a; buf[1] = b; snd_SendIOPCommandAndWait__FiiPc(0x5A, 8, buf); }
