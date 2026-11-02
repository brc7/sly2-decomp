struct K { int n0; int n4; int n8; char bC; char pad[0x110 - 0xD]; };
struct X { int n0; int n4; char pad[0x34 - 8]; int n34; int n38; int n3C; };
struct P { char pad[0xA8]; struct X xA8; char pad2[0xE8 - 0xA8 - sizeof(struct X)]; int pE8; char pad3[0xF8 - 0xEC]; int nF8; int *aFC; };
void func_00258248(struct K *);
int snd_BankLoadByLoc(int, int);
void snd_ResolveBankXREFS(void);
int snd_IsSoundALooper(int, int);
static inline int FSet(struct K *pk, struct X *px)
{
    if (!px) return 0;
    {
        int a = px->n34;
        int c = px->n0;
        int d = px->n3C;
        int b = px->n4;
        pk->n8 = 2;
        pk->n4 = b ^ d;
        pk->n0 = (c ^ a) + 0xCA0;
    }
    return pk->n4 != 0;
}
/** @sly2-annot addr=0x1FD5E8
 * Purpose: Load bank if needed, snd_ResolveBankXREFS, mark looping sounds (kind 2) via snd_IsSoundALooper
 * Info: proposed name PostSoundLoad (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/open-questions.md#modding-live-test-2026-11-01,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PostSoundLoad(struct P *p)
{
    int i;
    if (!p->pE8) {
        struct K k;
        k.n8 = 2;
        func_00258248(&k);
        if (FSet(&k, &p->xA8) && k.n8 == 2) p->pE8 = snd_BankLoadByLoc(k.n0, 0);
        snd_ResolveBankXREFS();
    }
    for (i = 0; i < p->nF8; i++) {
        if (p->aFC[i] == 2) continue;
        if (snd_IsSoundALooper(p->pE8, i)) p->aFC[i] = 2;
    }
}
