struct LM { float lo; float hi; };
struct S {
    char pad[0x1000]; int x1000; int x1004; int x1008; int a; int b; float t; int x1018; char p0[8];
    int x1024; int x1028; char p1[0x10]; float x103c; float x1040; float x1044; char p2[4]; int x104c;
    struct LM alm1[4]; float x1070; float x1074; float x1078; char p4[4]; int x1080;
    struct LM alm2[4]; float x10a4; float x10a8; float x10ac; float x10b0; int x10b4; int x10b8;
    int x10bc; char x10c0[0xC]; char x10cc[4];
};
struct CLOCK { char pad[0x3C]; float g3c; };
extern struct CLOCK D_002DDED0;
void func_001D5150(void *p, int a, int b);
void func_002333B4(void *pv, int b, int cb);
/** @sly2-annot addr=0x1875B8
 * Purpose: Change pad state/kind: InitRumble when kind becomes >=3 (dualshock), clear analog block on kind 4
 *   changes, reset button state when state changes to/from 3
 * Info: proposed name SetJoyJoys (T1); TU joy
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_A1+names_v2-signature(verified)
 * See: docs/engine/main-loop-startup.md#8-key-functions,
 *   docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void SetJoyJoys__FP3JOY4JOYS4JOYK(struct S *p, int a, int b)
{
    unsigned int i;
    if (a == p->a && b == p->b) return;
    if (b == 0) {
        p->x1008 = 0;
        a = 1;
    }
    if (p->b < 3 && b >= 3) func_001D5150(p->x10cc, p->x1000, p->x1004);
    if (p->b == 4 ? b != 4 : b == 4) func_002333B4(p->x10c0, 0, 0xC);
    if (p->a == 3 ? a != 3 : a == 3) {
        p->x10bc = 0xFFFF;
        p->x1018 = 0;
        p->x1080 = 0;
        p->x104c = 0;
        p->x10b8 = 0;
        p->x10b4 = 0;
        func_002333B4(p->x10c0, 0, 0xC);
        p->x1028 = 0;
        p->x103c = p->x1040 = p->x1044 = 0.0f;
        p->x1070 = p->x1074 = p->x1078 = 0.0f;
        p->x10a4 = p->x10a8 = 0.0f;
        p->x10b0 = 0.0f;
        p->x10ac = 0.0f;
        p->x1024 = 0;
        for (i = 0; i < 4; i++) {
            p->alm2[i].lo = -0.75f;
            p->alm1[i].lo = -0.75f;
            p->alm2[i].hi = 0.75f;
            p->alm1[i].hi = 0.75f;
        }
    }
    p->a = a;
    p->b = b;
    p->t = D_002DDED0.g3c;
}
