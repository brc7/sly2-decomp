typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x98]; int c; qword *a; };
/** @sly2-annot addr=0x1733C8
 * Purpose: Appends a test offset vector (max 16) at +0x98/+0x9C
 * Info: proposed name AddCid119Offset (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex)) | verifier PLAUSIBLE: Placeholder class. Appends a
 *   qword to array +0x9C (count +0x98, max 16); caller LoadCid119FromBrx. 'test offset' not supported. | T2
 *   forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_001733C8(struct S *p, qword x) { register int n __asm__("$3"); register int i __asm__("$6"); i = p->c; if (i < 16) { register qword *a __asm__("$4"); register int off __asm__("$2"); a = p->a; off = i * 16; n = i + 1; p->c = n; *(qword *)(off + (int)a) = x; } }
