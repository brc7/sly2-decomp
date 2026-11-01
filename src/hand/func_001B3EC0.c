struct S { char pad[0x10C]; int x10C; int x110; int x114; char x118[0x48]; char x160[0x20]; char x180[0x10]; };
extern int D_002E1E54, D_002DEDBC;
void func_0013D558(void *p, int cb, unsigned int a);
void func_001B3040(void *p);
int func_0024B480(void);
void func_0024A9D8(void *p, int a, int b);
void func_0024F790(void *p, int a, int b, void *fn, struct S *s);
void func_0024B628(void *p, int a, void *fn, struct S *s);
int func_001B41D0(struct S *p, int n);
void func_001B3388(void);
void func_001B33C8(void);
void func_001B33F0(void);
void func_001B34D0(void);
void func_001B3508(void);
/** @sly2-annot addr=0x1B3EC0
 * Purpose: init byte queue, CMpegAudio::Init, sceMpegInit/Create, add callbacks
 * Info: proposed name CMpeg_Start (T1); class CMpeg; TU mpeg
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/cutscenes.md#5-key-functions, docs/reference/functions/cutscenes.md#mpeg-0x1b2820-0x1b4438
 * @end-sly2-annot */
void func_001B3EC0(struct S *p, int a)
{
    p->x114 = a;
    p->x10C = 0;
    func_0013D558(p->x160, 0x40000, (p->x110 + 0x245A80) | 0x20000000);
    func_001B3040(p->x180);
    func_0024B480();
    func_0024A9D8(p->x118, p->x110 + 0x98000, 0x1ADA60);
    func_0024F790(p->x118, 0, 0, func_001B3388, p);
    func_0024F790(p->x118, 2, 0, func_001B33C8, p);
    func_0024F790(p->x118, 3, 0, func_001B33C8, p);
    func_0024F790(p->x118, 2, D_002E1E54 + 1, func_001B33C8, p);
    func_0024F790(p->x118, 3, D_002E1E54 + 1, func_001B33C8, p);
    func_0024B628(p->x118, 1, func_001B33F0, p);
    func_0024B628(p->x118, 4, func_001B34D0, p);
    func_0024B628(p->x118, 0, func_001B3508, p);
    while (!func_001B41D0(p, 1)) ;
    D_002DEDBC = 0;
}
