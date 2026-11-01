struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a, b, c; struct VTE *vt; };
struct S { char pad[0x1050]; struct O *po; };
void func_00206518(struct S *p);
/** @sly2-annot addr=0x1BA0B8
 * Purpose: CID 60 vt26: ResetStepguardToHome then vt9 on attached prop
 * Info: proposed name Neela_VUnk26Proxy (T2); class NEELA slot26; TU tu_1B9F80
 * Notes: class identity from whole_game WE names/object strings: Cid60Guard_VUnk26Proxy -> Neela_VUnk26Proxy
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_1b9f80-0x1b9f80-0x1bab50
 * @end-sly2-annot */
void func_001BA0B8(struct S *p)
{
    struct O *po;
    func_00206518(p);
    po = p->po;
    if (po) {
        struct VTE *e = &po->vt[9];
        ((void (*)(void *))e->pfn)((char *)po + e->d);
    }
}
