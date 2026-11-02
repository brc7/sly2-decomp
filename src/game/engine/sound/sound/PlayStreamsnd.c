struct K { int n0; int n4; int n8; char bC; char pad[0x110 - 0xD]; };
struct X { int n0; int n4; char pad[0x34 - 8]; int n34; int n38; int n3C; char pad2[0x4C - 0x40]; int n4C; };
struct VTE { short d; short p; void (*pfn)(void *); };
struct P { char pad[0xC]; struct VTE *pvt; char pad1[0x14 - 0x10]; int n14; char pad2[0x44 - 0x18]; float f44; char pad3[0x52 - 0x48]; short h52; char pad4[0x60 - 0x54]; struct X *p60; char pad5[0x3B8 - 0x64]; int n3B8; };
struct M { int n0; int n4; int n8; int nC; int n10; int n14; };
struct G { char pad[0x10]; int n10; };
extern struct G D_003DD040;
void func_00258248(struct K *);
int func_001FD080(struct G *, struct X *);
void PauseSnd(struct P *);
void SetSndFlags(struct P *, int, int);
void func_001FCBA8(void);
void func_00233130(int, int, int, struct M *, void (*)(void), struct P *);
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
/** @sly2-annot addr=0x1FC510
 * Purpose: vt9: resolve stream file key, send start to SUKR IOP module with vol/pan
 * Info: proposed name PlayStreamsnd (T1); class STREAMSND; TU sound
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7_1F8DD8+verdicts_tu_r7r8
 * See: docs/engine/sound.md#6-key-functions, docs/guides/port-plan.md#64-sound--coreaudio-k12,
 *   docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void PlayStreamsnd(struct P *p)
{
    struct K k;
    struct M m;
    int n;
    k.n8 = 2;
    func_00258248(&k);
    n = func_001FD080(&D_003DD040, p->p60);
    if (FSet(&k, p->p60)) {
        if (k.n8 != 2) {
            SetSndFlags(p, 0x1FFF, 0);
            return;
        }
        m.n0 = k.n0;
        PauseSnd(p);
        m.n4 = (int)(p->f44 * 1024.0f);
        m.n8 = p->n14;
        m.nC = n;
        m.n10 = p->n3B8 | 1;
        {
            int fl = 0;
            if (D_003DD040.n10 && n == 2) fl = p->p60->n4C == 0;
            m.n14 = fl;
        }
        p->h52 = 1;
        p->pvt[1].pfn((char *)p + p->pvt[1].d);
        func_00233130(0x524B5553, 1, 0x18, &m, func_001FCBA8, p);
        SetSndFlags(p, 0x3F, 0);
    } else {
        SetSndFlags(p, 0x1FFF, 0);
    }
}
