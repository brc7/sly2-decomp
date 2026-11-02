typedef int qword __attribute__((mode(TI)));
struct AL { int cb; int cbAlign; AL(int c, int a) { cb = c; cbAlign = a; } };
struct CBinaryInputStream;
struct TRI { int aipos[3]; float sArea; };
struct EMITMESH { int cpos; qword *apos; int ctri; TRI *atri; qword *anormal; float sArea; int pad18[2]; qword v20; };
extern "C" {
int U16Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void ReadVector__18CBinaryInputStreamP6VECTOR(CBinaryInputStream *pbis, qword *pv);
float F32Read__18CBinaryInputStream(CBinaryInputStream *pbis);
void *func_001AE1D0(AL *pal);
void *func_001AE2A0(AL *pal);
}
extern qword D_002A2DC0;
static inline void *PvAlloc(int cb, int cbAlign) { AL al(cb, cbAlign); return func_001AE1D0(&al); }
static inline void *PvAllocClear(int cb, int cbAlign) { AL al(cb, cbAlign); return func_001AE2A0(&al); }
static inline qword Sub(qword a, qword b) { qword d; __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Add(qword a, qword b) { qword d; __asm__("vadd.xyzw %0, %1, %2" : "=j"(d) : "j"(a), "j"(b)); return d; }
static inline qword Cross(qword a, qword b)
{
    qword d;
    __asm__("vopmula.xyz ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(d) : "j"(a), "j"(b));
    return d;
}
static __inline__ qword VLen(qword c)
{
    qword l, one;
    __asm__("vmul.xyz %0, %2, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=&j"(l), "=&j"(one) : "j"(c));
    {
    qword r;
    __asm__("vsqrt Q, %1x\n\tvwaitq\n\tvaddq.x %0, $vf0, Q" : "=j"(r) : "j"(l));
    return r;
    }
}
static __inline__ void VNormalize(qword *pr, qword c, qword k)
{
    qword l;
    register qword t __asm__("vf1");
    float gl;
    l = VLen(c);
    __asm__("qmfc2.ni %0, %1" : "=r"(gl) : "j"(l));
    if (gl < 1e-4f) {
        __asm__("" : "+r"(k));
        t = k;
    } else {
        __asm__ __volatile__("vdiv Q, $vf0w, %2x\n\tvwaitq\n\tvmulq.xyzw %0, %1, Q" : "=j"(t) : "j"(c), "j"(l));
    }
    *pr = t;
}
/** @sly2-annot addr=0x159240
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name LoadEmitmeshFromBrx__FP8EMITMESHP18CBinaryInputStream (EXISTING); TU emitter
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: name high [EXISTING 0.75]; purpose not reviewed - evidence: pre-existing symbol in
 *   symbol_addrs.txt
 * See: docs/engine/particles-effects.md#6-key-functions,
 *   docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void LoadEmitmeshFromBrx(EMITMESH *pemitmesh, CBinaryInputStream *pbis)
{
    int i;
    pemitmesh->cpos = U16Read__18CBinaryInputStream(pbis);
    pemitmesh->apos = (qword *)PvAlloc(pemitmesh->cpos * 16, 16);
    pemitmesh->anormal = (qword *)PvAllocClear(pemitmesh->cpos * 16, 16);
    for (i = 0; i < pemitmesh->cpos; i++)
        ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pemitmesh->apos[i]);
    pemitmesh->ctri = U16Read__18CBinaryInputStream(pbis);
    pemitmesh->atri = (TRI *)PvAlloc(pemitmesh->ctri * 16, 16);
    pemitmesh->sArea = 0.0f;
    for (int itri = 0; itri < pemitmesh->ctri; itri++) {
        TRI *ptri = &pemitmesh->atri[itri];
        for (i = 0; i < 3; i++)
            ptri->aipos[i] = U16Read__18CBinaryInputStream(pbis);
        ptri->sArea = F32Read__18CBinaryInputStream(pbis);
        pemitmesh->sArea += ptri->sArea;
        qword *apos = pemitmesh->apos;
        qword p0 = apos[ptri->aipos[0]];
        qword normal = Cross(Sub(apos[ptri->aipos[1]], p0), Sub(apos[ptri->aipos[2]], p0));
        for (i = 0; i < 3; i++) {
            qword *pn = &pemitmesh->anormal[ptri->aipos[i]];
            *pn = Add(*pn, normal);
        }
    }
    ReadVector__18CBinaryInputStreamP6VECTOR(pbis, &pemitmesh->v20);
    for (i = 0; i < pemitmesh->cpos; i++)
        VNormalize(&pemitmesh->anormal[i], pemitmesh->anormal[i], D_002A2DC0);
}
