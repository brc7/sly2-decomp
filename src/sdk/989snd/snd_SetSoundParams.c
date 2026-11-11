// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: changes a playing voice's volume / pan / pitch (handle, change mask, volume, pan, pitch, bend); forwards to
// snd_SetSoundParams_CB without a callback. Name inferred (989snd style; ApplyBanksndParams is the caller).
void snd_SetSoundParams_CB(int, int, int, int, int, int, int, int);
void snd_SetSoundParams(int a, int b, int c, int d, int e, int f) { snd_SetSoundParams_CB(a, b, c, d, e, f, 0, 0); }
