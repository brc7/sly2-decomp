struct S { char pad[0x40]; int x40; char pad2[0x13C]; int n180; char pad3[0x10]; int n194; char pad4[0x58]; int n1f0, n1f4, n1f8, n1fc, n200; };
void func_001C3D70(struct S *p);
static inline int FPtr(int v) { return !(v & 1); }
/** @sly2-annot addr=0x1C8280
 * Purpose: CID 78 (PO child, 0x230) Init: InitPo + fields
 * Info: proposed name InitVault (T1); class VAULT slot7; TU prize
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C8280(struct S *p)
{
    int v;
    func_001C3D70(p);
    v = p->x40;
    p->n1f0 = 0x7B;
    p->n1fc = -1;
    p->n1f8 = -1;
    p->n1f4 = -1;
    p->n200 = -1;
    p->n180 = -1;
    p->n194 = -1;
    if (FPtr(v)) *(int *)v |= 2; else p->x40 = v | 2;
}
