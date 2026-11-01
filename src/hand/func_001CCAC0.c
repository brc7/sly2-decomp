typedef int qword __attribute__((mode(TI)));
struct P8 { int a, b; };
struct W { char pad[0x1000]; int x1000; };
struct O { char pad[0xE0]; int xE0; };
struct S {
    struct W *x0; int x4; float x8; char p0[0x1C - 0xC]; float x1C;
    char p1[0x30 - 0x20]; void *x30; int x34; void *x38;
    char p2[0x80 - 0x3C]; int x80; struct O *x84; int x88; struct O *x8C; int x90;
    int x94; int x98; int x9C; int xA0; int xA4; float xA8;
};
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern struct P8 D_002A77C8;
extern char D_003DD490[];
void func_0012E460(int *p);
int func_00132B10(struct O *p, int n);
qword func_001A62E8(struct W *p);
void func_001E6F60(struct O *p, qword q, int a, int b, int c, int d, int *pn);
struct O *func_001CCA40(struct W *pw, struct O *po);
qword func_0022CA18(void *a, void *b);
int func_0012DC18(struct W *p, void *q, int a, int b, int c, qword d);
void func_001384C0(void *p);
void func_00149AB8(void *pg, struct W *pw, struct P8 p8, struct S *p);
/** @sly2-annot addr=0x1CCAC0
 * Purpose: Choose grab target (shape closest point or class-0x14 object), new position act, reblend, schedule
 *   UpdateJtHandsCallback
 * Info: proposed name SetJtHandsTarget (T2); TU jthands
 * Notes: names_v2 C proposed RevokeCmPolicy__FP2CMi3CPPP5CPLCYP2SOPv (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
void func_001CCAC0(struct S *p, int n)
{
    if (n == p->xA4) return;
    if (p->xA4 == 1) {
        func_0012E460(&p->x94);
        func_0012E460(&p->x9C);
    }
    p->xA4 = n;
    p->xA8 = D_002DDED0.t;
    switch (n) {
    case 0:
    case 1:
        p->x8C = p->x84;
        p->x90 = 1;
        if (func_00132B10(p->x84, 10)) {
            struct O *po = p->x84;
            func_001E6F60(po, func_001A62E8(p->x0), 0, 0, 0, 0, &p->x88);
            p->x90 = 0;
        } else if (func_00132B10(p->x84, 0x14) && (p->x84->xE0 & p->x0->x1000)) {
            p->x90 = 2;
        } else if (func_00132B10(p->x84, 0x30)) {
            struct O *po = func_001CCA40(p->x0, p->x84);
            if (po) {
                p->x8C = po;
                p->x90 = 2;
            }
        }
        if (p->x90 == 2)
            p->x80 = func_0012DC18(p->x0, p->x38, 5, 7, 3, func_0022CA18(p->x38, p->x38));
        p->x1C = p->x8 = 0.5f;
        func_001384C0(p->x30);
        func_00149AB8(D_003DD490, p->x0, D_002A77C8, p);
        break;
    case -1:
        func_0012E460(&p->x80);
        p->x90 = n;
        p->x84 = 0;
        p->x88 = 0;
        p->x8C = 0;
        break;
    }
}
