typedef int qword __attribute__((mode(TI)));
struct K { int a, b; };
extern qword D_002BE7D0;
extern struct K D_002BE7E0;
struct S { char pad[0x40]; int fl40; char pad2[0x13C]; int n180; char pad3[0x1C]; qword q1A0; struct K k1B0; };
void func_001F2108(struct S *p);
/** @sly2-annot addr=0x17F090
 * Purpose: CID 0x47 vt7: InitSo, max health x180=10, two default vectors, grf
 * Info: proposed name InitHack (T2); class HACK slot7; TU tu_17F090
 * Notes: Cid47->Cid71 (CID 0x47=71, vtable) | v3 class identity: InitCid71 -> InitHack | T2 forced: v3:
 *   coined class word (Hack) without descriptor/string backing
 * Confidence: low (draft) [T2 0.60] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/engine/gameplay-objects.md#source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F090(struct S *p) {
    int v;
    func_001F2108(p);
    p->n180 = 10;
    p->q1A0 = D_002BE7D0;
    p->k1B0 = D_002BE7E0;
    v = p->fl40;
    if ((v ^ 1) & 1) *(int *)v |= 2; else p->fl40 = v | 2;
}
