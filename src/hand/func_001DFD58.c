typedef struct DL { void *pvFirst; void *pvLast; int ibDle; } DL;
typedef struct OBJ OBJ;
typedef struct VT { struct { short d; short pad; void (*pfn)(void *, int, int, int, int, int); } a[8]; } VT;
struct OBJ {
    char pad[0xC];
    VT *vt;
    char pad10[0x40 - 0x10];
    int x40;
    char pad44[0x54 - 0x44];
    void *x54;
    int pad58;
    DL dl;
    char pad68[0x6C - 0x68];
    int x6C;
    int x70;
    int x74;
    int x78;
    int x7C;
};
void func_001E02C0(OBJ *);
void *func_001E5AE0(OBJ *, void *, int);
void func_001E1C40(void *);
int func_0011A3B0(DL *);
void func_001DFEB8(OBJ *);
void func_001A6CA8(OBJ *, int);
/** @sly2-annot addr=0x1DFD58
 * Purpose: Start the relay (if not already firing): ploFirer=arg, fFiring=1, send itself msg 5, append
 *   BlockEnd if fAppendEnd, spawn a JSG for the top block (PjsgNew) and run it; if still running mark the LO
 *   busy (0x1A6CA8(p,1), grf bit 2, fBusy)
 * Info: proposed name FireRelay (T1); TU relay
 * Notes: runtime5b/runtime6: charsel entries fire RELAYs; JSG spawned per fired RELAY (S57-S61)
 * Confidence: high [T1 0.80] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void func_001DFD58(OBJ *p, int n)
{
    int fl;
    if (p->x74)
        return;
    p->x6C = n;
    p->x74 = 1;
    p->x78 = 0;
    p->vt->a[3].pfn((char *)p + p->vt->a[3].d, 5, 0, 0, 0, 0);
    if (!p->x74)
        return;
    if (p->x7C) {
        func_001E02C0(p);
        p->x7C = 0;
    }
    func_001E1C40(func_001E5AE0(p, p->x54, 0));
    if (!p->x74)
        return;
    if (func_0011A3B0(&p->dl)) {
        func_001DFEB8(p);
        return;
    }
    fl = p->x40;
    if ((fl ^ 1) & 1)
        fl = *(int *)fl;
    if (fl & 2)
        return;
    func_001A6CA8(p, 1);
    if ((p->x40 ^ 1) & 1)
        *(int *)p->x40 |= 2;
    else
        p->x40 |= 2;
    p->x78 = 1;
}
