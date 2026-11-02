void InitBhv(void *p);
struct X { char pad[0x60]; int n; int pad2; float a, b, c, d, e; };
/** @sly2-annot addr=0x145010
 * Purpose: BHV194 slot 7: base, state -1, follow distances 0.25/250/350/1.0/1.2
 * Info: proposed name InitBhvfollow (T1); class BHVFOLLOW slot7; TU tu_144B28
 * Notes: superseded: OnBhv194Push (r2b_rest) | BHV vtable slot 7 role per r9 (CONFLICTS #15); was
 *   OnBhv194Push | v3 BHV class word (runtime6 P2: BHV194 = follow, BHV168 = walk-to-goal child): InitBhv194
 *   -> InitBhvfollow | runtime6 P2: InitBhv194 defaults 0.25/250/350/1.0/1.2 seen at +0x68 on live Murray
 *   (S62)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r2b_rest+bhv-slot-canon(r9)+runtime(class role)+runtime
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
void InitBhvfollow(struct X *p)
{
    InitBhv(p);
    p->n = -1;
    p->a = 0.25f;
    p->b = 250.0f;
    p->c = 350.0f;
    p->d = 1.0f;
    p->e = 1.2f;
}
