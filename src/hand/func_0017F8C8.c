struct S { char pad[0x28]; unsigned long fl; };
void func_001C3FF0(struct S *p, int f);
void func_0017FD08(struct S *p, int f);
/** @sly2-annot addr=0x17F8C8
 * Purpose: vt78: base then SetCid48State
 * Info: proposed name OnHackshipActive (T2); class HACKSHIP slot78; TU tu_17F090
 * Notes: CID48->CID72 (CID 0x48=72, vtable) | class identity from whole_game WE names/object strings:
 *   OnCid72Active -> OnHackshipActive
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F8C8(struct S *p, int f) { func_001C3FF0(p, f); if (f) func_0017FD08(p, 1); else if ((int)(p->fl >> 60) & 1) func_0017FD08(p, 0); }
