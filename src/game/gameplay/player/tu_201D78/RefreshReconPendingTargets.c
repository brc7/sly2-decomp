struct Q { char pad[0x54]; int n; };
struct O { char pad[0xEC]; struct Q *q; char p2[8]; int nF8; };
struct S { char pad[0x30]; int c; struct O *a[16]; unsigned int c74; struct O *a78[5]; };
/** @sly2-annot addr=0x202030
 * Purpose: pending list (+0x74/+0x78, max 5): targets with name id and task state 1/2
 * Info: proposed name RefreshReconPendingTargets (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
void RefreshReconPendingTargets(struct S *p)
{
    int i;
    p->c74 = 0;
    for (i = 0; i < p->c; i++) {
        struct O *o = p->a[i];
        if (o->nF8 != -1) {
            struct Q *q = o->q;
            if (!q || q->n == 1 || q->n == 2) {
                if (p->c74 < 5) p->a78[p->c74++] = o;
            }
        }
    }
}
