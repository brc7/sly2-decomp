struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct S { char pad[0xC]; struct VTE *vt; char pad2[0x48]; int n58; int n5C; char pad3[0x168]; int a1C8[1]; };
void func_0017F138(struct S *p);
void func_002303A0(struct S *p, int a, int b);
void func_0017F770(struct S *p);
void func_0017F408(struct S *p, int n, void *pv);
/** @sly2-annot addr=0x17F850
 * Purpose: vt15: PostCid47Load, bind entries and parts
 * Info: proposed name PostHackshipLoad (T2); class HACKSHIP slot15; TU tu_17F090
 * Notes: names_v2 v1 proposed UseStepguardAnimationImmediate__FP9STEPGUARD3SGS3OID (0.067) | Cid48->Cid72
 *   (CID 0x48=72, vtable) | class identity from whole_game WE names/object strings: PostCid72Load ->
 *   PostHackshipLoad
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F850(struct S *p) { struct VTE *e; func_0017F138(p); e = &p->vt[0x4F]; e->pfn((char *)p + e->d, 0); func_002303A0(p, p->n58, 2); func_002303A0(p, p->n5C, 2); func_0017F770(p); func_0017F408(p, 5, p->a1C8); }
