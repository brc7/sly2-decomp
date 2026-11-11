// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// 989snd: plays a sound from a 0x30-byte parameter block (mask, bank, sound index, volume, pan, pitch), IOP command 0x5C, no reply.
// Name inferred (989snd style; the Sony name is not known): PlayBanksnd fills the block for one-shot bank sounds.
void snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(int, int, void *, int, int);
void snd_PlaySoundEx(void *a) { snd_SendIOPCommandNoWait__FiiPcPFiUl_vUl(0x5C, 0x30, a, 0, 0); }
