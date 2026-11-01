struct VE { short d; short pad; void (*pf)(void *); };
struct O {
    char pad0[0xC]; struct VE *vt; char pad1[0x28 - 0x10]; unsigned long fl28;
    char pad2[0xF64 - 0x30]; void *pF64; char pad3[0xFF8 - 0xF68]; float tFF8; int pad4; int k1000;
};
struct S { char pad[0x50]; int c; struct O **apo; };
struct L { int k; int rest[27]; };
extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3DBC;
extern const float D_002A88CC;
void func_001A5E28(struct S *p);
void *func_00205458(struct O *po, int n);
void *func_002333B4(void *p, int c, int n);
void func_00176750(void *p, struct L *pl);
void func_00153FF8(void *p);

/** @sly2-annot addr=0x20B590
 * Purpose: cid 0x7D vt16: guards out of the list for > 0.25 s (+0xFF8): mode +0x1000 1 = RemoveLo, 2 =
 *   restart BHV156 and reset detector
 * Info: proposed name UpdateCid125GuardMgr (T2); class CID125 slot16; TU stepguard
 * Notes: Cid7D->Cid125 (CID 0x7D=125, hex-literal) | T2 forced: verifier: coined class word without
 *   descriptor
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#1-stepguard-stepguardcpp-0x205e70-0x20b840-and-its-subclasses,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B590(struct S *p)
{
    int i, c;
    struct O **apo;
    func_001A5E28(p);
    c = p->c;
    apo = p->apo;
    for (i = 0; i < c; i++) {
        struct O *o = apo[i];
        if (((int)(o->fl28 >> 60) & 1) && D_002DDED0.t - o->tFF8 > D_002C3DBC) {
            switch (o->k1000) {
            case 0:
                break;
            case 1:
                o->vt[9].pf((char *)o + o->vt[9].d);
                break;
            case 2: {
                void *x = func_00205458(o, 0x9C);
                if (x) {
                    struct L l;
                    func_002333B4(&l, 0, sizeof(l));
                    l.k = 1;
                    func_00176750(x, &l);
                }
                if (o->pF64)
                    func_00153FF8(o->pF64);
                break;
            }
            }
            o->tFF8 = D_002A88CC;
        }
    }
}
