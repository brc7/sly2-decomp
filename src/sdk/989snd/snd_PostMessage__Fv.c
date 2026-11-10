// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int *gSndCommandBuffePtr[]; extern int gCommandFillBuffer; void snd_FlushSoundCommands(void);
void snd_PostMessage__Fv(void) { (*gSndCommandBuffePtr[gCommandFillBuffer])++; snd_FlushSoundCommands(); }
