struct S { char pad[0x80]; float g80; int n84; int n88; int n8C; char pad2[0x10]; float gA0; float gA4; };
void InitLo(struct S *p);
/** @sly2-annot addr=0x173158
 * Purpose: cid 0x77 (LO child, 0xB0) Init: LO Init, defaults 5500/1/1/1, respawn delays 30 and 50 s
 * Info: proposed name InitCid119 (T2); class CID119 slot7; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, vtable) | verifier PLAUSIBLE: Placeholder class (CID 0x77 vtable slot
 *   7). Body InitLo, +0x80=5500, +0x84/88/8C=1, +0xA0=30, +0xA4=50: matches; calling 30/50 'respawn delays'
 *   is not supported by this body. | T2 forced: verifier: placeholder name
 * Confidence: low (draft) [T2 0.75] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_00173158(struct S *p) { InitLo(p); p->g80 = 5500.0f; p->n84 = p->n8C = p->n88 = 1; p->gA0 = 30.0f; p->gA4 = 50.0f; }
