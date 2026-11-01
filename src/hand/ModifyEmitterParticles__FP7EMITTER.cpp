typedef int qword __attribute__((mode(TI)));
union V { qword q; struct { float x, y, z, w; } v; float a[4]; };
struct BLK { qword apos[2][0x84]; int ibuf; int cpart; int pad[2]; BLK *pblkNext; };
struct PRG { char pad[0x448]; BLK *pblkFirst; };
struct ALO;
struct EMITO { ALO *palo; };
struct EMITB { int pad0; int f4; EMITO *pemito; };
struct EMITTER { char pad[0xE4]; EMITB *pemitb; char pad1[0x140 - 0xE8]; float sBox; char pad2[0x150 - 0x144]; PRG *pprg; };
extern "C" {
qword func_001A62E8(ALO *palo);
float GModPositive__Fff(float g, float gMod);
}
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline void Wrap(float *pg, float &gMin, float &gMax, float &gBox, float *pdg)
{
    if (*pg < gMin || gMax < *pg) {
        *pdg = gMin + GModPositive__Fff(*pg - gMin, gBox) - *pg;
        *pg += *pdg;
    }
}
/** @sly2-annot addr=0x159A20
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void ModifyEmitterParticles(EMITTER *pemitter)
{
    float sBox = pemitter->sBox;
    if (sBox > 0.0001f) {
        V vecBox, posMin, posMax, dpos;
        volatile int unused;
        vecBox.v.z = sBox; vecBox.v.y = sBox; vecBox.v.x = sBox;
        if (pemitter->pemitb->pemito->palo) {
            posMin.q = Sub(func_001A62E8(pemitter->pemitb->pemito->palo), vecBox.q);
            posMax.q = Add(func_001A62E8(pemitter->pemitb->pemito->palo), vecBox.q);
        } else {
            qword m, k;
            __asm__("qmtc2.ni %1, %0" : "=j"(k) : "r"(-1.0f));
            dpos.q = k;
            __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(m) : "j"(vecBox.q), "j"(k));
            posMin.q = m;
            posMax.q = vecBox.q;
        }
        float sBox2 = pemitter->sBox + pemitter->sBox;
        vecBox.v.z = sBox2; vecBox.v.y = sBox2; vecBox.v.x = sBox2;
        if (pemitter->pemitb->f4 == 0 && pemitter->pprg) {
            for (BLK *pblk = pemitter->pprg->pblkFirst; pblk; pblk = pblk->pblkNext) {
                qword *apos = pblk->apos[pblk->ibuf];
                for (int i = 0; i < pblk->cpart; i++) {
                    V *ppos = (V *)&apos[i];
                    Wrap(&ppos->v.x, posMin.v.x, posMax.v.x, vecBox.v.x, &dpos.v.x);
                    Wrap(&ppos->v.y, posMin.v.y, posMax.v.y, vecBox.v.y, &dpos.v.y);
                    Wrap(&ppos->v.z, posMin.v.z, posMax.v.z, vecBox.v.z, &dpos.v.z);
                }
            }
        }
    }
}
