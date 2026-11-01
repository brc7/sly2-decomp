struct S { char pad[0x38]; float g; };
extern float D_002C3C58;
void PostLoLoad(struct S *);
/** @sly2-annot addr=0x1FF3E8
 * Purpose: vt15: PostLoLoad; if radius-like field still 1e10 use default D_002C3C58
 * Info: proposed name PostCid39Load (T2); class CID39 slot15; TU tu_1FF3B8
 * Notes: Cid27->Cid39 (CID 0x27=39, vtable)
 * Confidence: low (draft) [T2 0.55] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/gameplay-objects.md#tu_1ff3b8-0x1ff3b8-0x1ff690
 * @end-sly2-annot */
void func_001FF3E8(struct S *p) { PostLoLoad(p); if (p->g == 1e10f) p->g = D_002C3C58; }
