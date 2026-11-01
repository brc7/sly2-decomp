struct VTE { short d; short pad; void (*pfn)(void *); };
struct S { char pad[0xC]; struct VTE *vt; };
void func_001F36A8(struct S *p);
/** @sly2-annot addr=0x17F138
 * Purpose: vt15: PostSoLoad then own vt83
 * Info: proposed name PostHackLoad (T2); class HACK slot15; TU tu_17F090
 * Notes: names_v2 v1 proposed SetStepguardSgsExternal__FP9STEPGUARD3SGSP4ASEG (0.036) | Cid47->Cid71 (CID
 *   0x47=71, vtable) | v3 class identity: PostCid71Load -> PostHackLoad | T2 forced: v3: coined class word
 *   (Hack) without descriptor/string backing
 * Confidence: low (draft) [T2 0.55] - evidence: static naming pass; sources:
 *   r4b_rest+class_review(master_names_v2_1)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F138(struct S *p) { struct VTE *e; func_001F36A8(p); e = &p->vt[0x53]; e->pfn((char *)p + e->d); }
