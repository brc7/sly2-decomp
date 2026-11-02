struct S { int k; char pad[0x50]; void *o; };
int func_002121E8(void *o, int a);
void func_0022B090(struct S *p, int k);
/** @sly2-annot addr=0x22B198
 * Purpose: advances wipe state machine (1->3, 2->0 when sprite done)
 * Info: proposed name UpdateWipe (T1); TU wipe
 * Notes: runtime5b: WIPE k 0..3 state machine observed (S56)
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r8_21BB88+runtime
 * See: docs/reference/functions/world-sw.md#wipe-0x22b040-0x22b720
 * @end-sly2-annot */
void func_0022B198(struct S *p)
{
    int k = p->k;
    switch (k) {
    case 0: break;
    case 1: if (!func_002121E8(p->o, 0x314)) k = 3; break;
    case 2: if (!func_002121E8(p->o, 0x314)) k = 0; break;
    }
    func_0022B090(p, k);
}
