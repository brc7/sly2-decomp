struct VTE { short d; short pad; void (*pfn)(void *, int, int, int, int, int); };
struct S { char pad[0xC]; struct VTE *vt; char p2[0x1B8]; void *x1c8; };
void func_0017FD08(void *p, int n);
/** @sly2-annot addr=0x181728
 * Purpose: SetCid48State(singleton x1C8, 1) and own vt call
 * Info: proposed name TriggerHackshipFromHacklock (T2); class HACKSHIP; TU tu_17F090
 * Notes: Cid48->Cid72 (CID 0x48=72, agent-default(hex)) | Cid4B->Cid75 (CID 0x4B=75, hex-literal) | class
 *   identity from whole_game WE names/object strings: TriggerCid72FromCid75 -> TriggerHackshipFromHacklock
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181728(struct S *p)
{
    struct VTE *e;
    func_0017FD08(p->x1c8, 1);
    p->x1c8 = 0;
    e = &p->vt[3];
    e->pfn((char *)p + e->d, 6, 0, 0, 0, 0);
}
