typedef struct MATRIX4 { float mat[4][4]; } __attribute__((aligned(16))) MATRIX4;
typedef struct K { int a, b, c; } K;
typedef struct KG { K k; float g; } KG;
typedef struct GS { char pad[0x3E38]; int x3E38; } GS;
typedef struct CM { char pad[0x4C]; float rNear; float rFar; char pad2[0xC]; MATRIX4 m60; char pad3[0xA0]; MATRIX4 m140; } CM;
typedef struct P { char pad[0x20]; int x20; } P;
extern GS D_003DD490;
extern void *D_002DEA4C;
extern char D_002DEA50[];
extern char D_002DF940[], D_002DFA80[], D_002DF800[], D_002DF010[];
extern CM D_002DDF40;
extern K D_002C1F00, D_002C1F10;
extern char D_003E12B8[];
extern void func_00157598(P *, void *);
extern void func_001574B8(P *);
extern void func_00226D10(P *);
extern void func_002270C8(P *, int, void *, int);
extern void func_00226CA8(P *, int, int);
extern void func_00226F28(P *, int, int, void *, int);
extern void func_0014F768(CM *, int, int, MATRIX4 *, MATRIX4 *);
extern void func_001576F0(P *);
extern void BeginRpGlob(P *, void *, void *);
/** @sly2-annot addr=0x157CF0
 * Purpose: Pass 10 begin (kind 22): re-CALL frame context (or stereo eye), DIRECT Z-only clear D_002DF010,
 *   re-upload camera/ambient VU globals, BASE/OFFSET 0x32/0x1E7, then BeginRpGlob
 * Info: proposed name BeginRpForeground (T1); TU draw
 * Notes: no usable identifier; purpose only
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8+rendering
 * See: docs/engine/rendering.md#4-the-26-kinds-and-12-passes-cstatic-tables-dumped-from-the-elf-roles-cstatic-unless-marked,
 *   docs/reference/functions/rendering.md#draw-0x157a50-0x1585d8
 * @end-sly2-annot */
void BeginRpForeground(P *p, void *a1, void *a2)
{
    KG akg[2];
    MATRIX4 m1, m2;
    if (D_003DD490.x3E38) {
        if (D_002DEA4C == D_002DEA50)
            func_00157598(p, D_002DF940);
        else
            func_00157598(p, D_002DFA80);
    } else {
        func_00157598(p, D_002DF800);
    }
    p->x20 = 1;
    func_001574B8(p);
    func_00226D10(p);
    func_002270C8(p, 0x25, D_002DF010, 0);
    func_00226CA8(p, 0, 0);
    {
        CM *pcm = &D_002DDF40;
        float rNear = pcm->rNear;
        float rFar = pcm->rFar;
        float r = (rNear + rFar) / (rNear - rFar);
        float s = rNear * (1.0f - r);
        akg[0].k = D_002C1F00;
        akg[0].g = 1.0f / s;
        akg[1].k = D_002C1F10;
        akg[1].g = -r / s;
        func_00226F28(p, 0xC, 2, akg, 0);
        if (D_003DD490.x3E38) {
            if (D_002DEA4C == D_002DEA50)
                func_0014F768(pcm, 1, 0, &m1, &m2);
            else
                func_0014F768(pcm, 2, 0, &m1, &m2);
        } else {
            MATRIX4 *pm1 = &pcm->m140;
            MATRIX4 *pm2 = &pcm->m60;
            m1 = *pm1;
            m2 = *pm2;
        }
    }
    func_00226F28(p, 0xC, 4, &m1, 2);
    func_00226F28(p, 0xC, 4, &m2, 6);
    func_00226F28(p, 0xC, 1, D_003E12B8, 10);
    func_00226CA8(p, 0x32, 0x1E7);
    func_001576F0(p);
    BeginRpGlob(p, a1, a2);
}
