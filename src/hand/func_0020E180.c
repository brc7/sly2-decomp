struct MSG { int cb; int nKind; };
struct H { int a; int b; int c; int d; int e; int f; char dl[0x10]; };
struct S {
    char pad0[0x3BF4]; int fl3BF4;
    char pad1[0x3CD4 - 0x3BF8]; void *a3CD4[3];
    char pad2[0x3E38 - 0x3CE0]; int n3E38;
    char pad3[0x3F0C - 0x3E3C]; struct H *ph; char pad4[0x3F20 - 0x3F10]; void *p3F20;
    char pad5[0x45E8 - 0x3F24]; char a45E8[2][0x28];
};
struct ZPR { char ab[0x630]; };
extern char D_002DEB30[];
void *func_001AE1D0(struct MSG *pmsg);
void *func_001AE2A0(struct MSG *pmsg);
void func_0011A050(void *p, int n);
void func_001703F8(struct ZPR *pz, int a, int b);
void func_001BE398(void *p, struct ZPR *pz);
void func_001619A0(void *p, int a, int b);

/** @sly2-annot addr=0x20E180
 * Purpose: Pre-load setup of the fresh SW: 3x0x4800 and 0x8000 work buffers, 0x28 header with DL, InitZpr x2,
 *   game callback(2)
 * Info: proposed name InitSwLoadBuffers (T2); TU sw
 * Notes: names_v2 v1 proposed BindChkpnt__FP6CHKPNT (0.004)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020E180(struct S *p)
{
    struct MSG msg;
    struct ZPR zpr;
    int i;
    unsigned int j;
    for (i = 0; i < 3; i++) {
        msg.cb = 0x4800;
        msg.nKind = 0x10;
        p->a3CD4[i] = func_001AE1D0(&msg);
    }
    msg.cb = 0x8000;
    msg.nKind = 0x10;
    p->p3F20 = func_001AE2A0(&msg);
    msg.cb = 0x28;
    msg.nKind = 0x10;
    p->ph = func_001AE2A0(&msg);
    p->ph->a = -1;
    p->ph->c = -1;
    func_0011A050(p->ph->dl, 0x14);
    func_001703F8(&zpr, 0x1400, 0x1800);
    for (j = 0; j < 2; j++)
        func_001BE398(p->a45E8[j], &zpr);
    if (p->fl3BF4 & 0x20)
        p->n3E38 = 1;
    func_001619A0(D_002DEB30, 2, 0);
}
