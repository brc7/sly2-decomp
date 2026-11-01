struct S { char pad[0x3B18]; int h3B18; };
extern char D_002DEB30[], D_003D9E70[], D_002DDF40[], D_003DD040[], D_002E0CCC[];
extern int D_002E1E40, D_002F7900, D_002DD5BC, D_002DD648, D_003D4A54, D_002DE570, D_002DFB80, D_002DFBC4;
extern int D_003E1C40, D_003E1C44, D_003E1C48, D_003E1C4C, D_003E1C50, D_003DD48C;
void func_001619A0(void *p, int a, int b);
void func_00162EA0(int h);
void func_001D9408(void *p);
void func_001FF830(struct S *p);
void func_00161458(void *p);
void func_0014D650(void *p);
void func_001FD700(void *p);
void func_001EC7E8(void);
void func_001D53C0(void *p, int n);
void func_001AE300(void);

/** @sly2-annot addr=0x20E068
 * Purpose: Tear down the current world before loading: game callback(1), free view/joy state, ResetSndmgr,
 *   clear ~15 global object pointers, FreeSw
 * Info: proposed name DeleteSw (T1); TU sw
 * Notes: names_v2 v1 proposed LoadChkpntFromBrx__FP6CHKPNTP18CBinaryInputStream (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/engine/overview.md#3-world--level-lifecycle, docs/engine/world-sw.md#2-loading-a-level,
 *   docs/engine/world-sw.md#10-key-functions
 * @end-sly2-annot */
void func_0020E068(struct S *p)
{
    void *pv = D_002DEB30;
    func_001619A0(pv, 1, 0);
    if (p->h3B18) {
        func_00162EA0(p->h3B18);
        p->h3B18 = 0;
    }
    func_001D9408(D_003D9E70);
    func_001FF830(p);
    func_00161458(pv);
    func_0014D650(D_002DDF40);
    func_001FD700(D_003DD040);
    func_001EC7E8();
    func_001D53C0(D_002E0CCC, 3);
    D_002E1E40 = 0;
    D_002F7900 = 0;
    D_002DD5BC = 0;
    D_002DD648 = 0;
    D_003D4A54 = 0;
    D_002DE570 = 0;
    D_002DFB80 = 0;
    D_002DFBC4 = 0;
    D_003E1C40 = 0;
    D_003E1C44 = 0;
    D_003E1C48 = 0;
    D_003E1C4C = 0;
    D_003E1C50 = 0;
    D_003DD48C = 0;
    func_001AE300();
}
