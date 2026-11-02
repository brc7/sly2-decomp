struct X { char pad[0x54]; int n54; };
struct O { char pad[0x230]; struct X *px; };
struct Ent { int a0; int a4; int a8; int aC; };
struct P { char pad[0x404]; struct Ent a[8]; int n484; int n488; int n48C; int n490; int n494; };
struct G;
extern struct G D_003DD490;
struct O *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, int);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *, int);
int func_001C2DC8(int, struct O *, int, int);
void UpdateStepguardPickpocket(struct P *);
/** @sly2-annot addr=0x1F1CF8
 * Purpose: From the next loot entry on, find its template object (PloFindSwObject oid) and load it into the
 *   pouch launcher (0x1C2DC8, kind 0x2B0, or 0x2B1 for cid-0x5C objects); then refresh pickpocket state
 * Info: proposed name StageStepguardLoot (T1); TU pickpocket
 * Notes: runtime: pickpocket pouch is CID99 (SO child), not a PO
 * Confidence: mid (draft) [T1 0.50] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime-corrected
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void StageStepguardLoot(struct P *p)
{
    p->n48C = 0;
    for (; p->n488 < p->n484; p->n488++) {
        struct Ent *pe = &p->a[p->n488];
        struct O *po = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, pe->a0, p->n494);
        int a8, k;
        if (!po) continue;
        a8 = pe->a8;
        k = 0x2B0;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(po, 0x5C)) {
            struct X *px = po->px;
            if (px) {
                int n = px->n54;
                if (n == -1 || n == 0) { p->n48C = 1; break; }
                if (n != 1) continue;
            }
            k = 0x2B1;
        }
        if (func_001C2DC8(p->n494, po, a8, k) == 1) p->n48C = 1;
        break;
    }
    UpdateStepguardPickpocket(p);
}
