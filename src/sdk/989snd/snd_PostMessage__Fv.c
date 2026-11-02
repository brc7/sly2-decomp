// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int *D_002C4660[]; extern int D_002C4680; void snd_FlushSoundCommands(void);
void snd_PostMessage__Fv(void) { (*D_002C4660[D_002C4680])++; snd_FlushSoundCommands(); }
