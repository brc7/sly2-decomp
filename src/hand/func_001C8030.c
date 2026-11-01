struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { int a[3]; struct VTE *vt; char pad[0xF0]; int n100; };
struct G { char pad[0x4760]; int n; };
extern struct G D_003DD490;
void PostLoLoad(struct O *p);
int func_001C7BB8(int n);
/** @sly2-annot addr=0x1C8030
 * Purpose: CID 7 (PROXY child) vt15: base + hide if clue already collected
 * Info: proposed name PostClueproxyLoad (T2); class CLUEPROXY slot15; TU prize
 * Notes: v3 class identity: PostClueProxyLoad -> PostClueproxyLoad | T2 forced: v3: coined class word
 *   (Clueproxy) without descriptor/string backing
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources:
 *   r5_1AE130+class_review(master_names_v2_1)
 * See: docs/reference/functions/game-progression.md#prize-0x1c4e50-0x1c8cc0
 * @end-sly2-annot */
void func_001C8030(struct O *p)
{
    PostLoLoad(p);
    p->n100 = D_003DD490.n++;
    if (func_001C7BB8(p->n100)) { struct VTE *e = &p->vt[9]; e->pfn((char *)p + e->d); }
}
