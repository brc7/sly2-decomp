struct S { char pad[0x188]; int n; };
void func_0017F090(struct S *p);
/** @sly2-annot addr=0x180188
 * Purpose: CID 0x4A vt7: InitCid47, field = 10
 * Info: proposed name InitHackbullet (T2); class HACKBULLET slot7; TU tu_17F090
 * Notes: Cid4A->Cid74 (CID 0x4A=74, hex-literal) | verifier PLAUSIBLE: Placeholder class. CID 0x4A vtable
 *   slot 7: InitCid47 (0x17F090) then +0x188 = 10. | class identity from whole_game WE names/object strings:
 *   InitCid74 -> InitHackbullet | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources:
 *   r4b_rest+verdicts_followups+whole_game(class identity)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00180188(struct S *p)
{
    func_0017F090(p);
    p->n = 10;
}
