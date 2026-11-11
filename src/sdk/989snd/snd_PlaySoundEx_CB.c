// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: snd_PlaySoundEx with a completion callback and user data (command 0x5C, NoWait); PlayBanksnd uses it for
// looping sounds, whose callback (OnBankSoundStarted) receives the voice handle. Name inferred (989snd style).
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_PlaySoundEx_CB(void *a, int b, int c) { snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(0x5C, 0x30, a, b, c); }
