struct DL { void *pvFirst; void *pvLast; int ibDle; };
extern "C" void func_00234700(int n, int (*pfn)(), int m);
extern "C" void func_00235178(int n);
extern "C" void InitDl__FP2DLi(DL *pdl, int ib);
extern "C" void func_002355E0(int n, int m);
extern "C" int func_00167F78();
int SemaCreate(int n, int m);
extern int D_002DEB18;
extern int D_002DEB1C;
extern DL D_002DEAF0;
extern DL D_002DEB00;
extern int D_004425D4;
/** @sly2-annot addr=0x1607A0
 * Purpose: Render-thread startup: installs INTC 5 (VIF1) handler 0x167F78, creates the two frame semaphores
 *   (0,2)/(2,2), clears the two deferred-release DLs, starts the render thread
 * Info: proposed name StartupFrame (T1); TU frm
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/object-model.md#8-source-files, docs/engine/overview.md#1-boot,
 *   docs/engine/rendering.md#12-source-files
 * @end-sly2-annot */
extern "C" void func_001607A0()
{
    func_00234700(5, func_00167F78, 0);
    func_00235178(5);
    D_002DEB18 = SemaCreate(0, 2);
    D_002DEB1C = SemaCreate(2, 2);
    InitDl__FP2DLi(&D_002DEAF0, 0x454);
    InitDl__FP2DLi(&D_002DEB00, 0x1090);
    func_002355E0(D_004425D4, 0);
}
