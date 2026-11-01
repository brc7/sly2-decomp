struct G;
extern struct G D_003DD490;
struct O { char pad[0x58]; int n58; char p2[0xEC]; unsigned long fl; };
struct S { char pad[0x34]; struct O *o; char p2[0x28]; int f60; int p64; char x68[4]; };
void func_0012E460(void *pv);
void func_002303A0(struct O *o, int n, int m);
void func_00134320(struct G *pg, struct O *o);
/** @sly2-annot addr=0x20C760
 * Purpose: drop constraint, clear LO flag bit 26
 * Info: proposed name ReleaseLeapConstraint (T2); TU tu_20B840
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
void func_0020C760(struct S *p)
{
    if (p->f60) {
        p->f60 = 0;
        if (*(int *)p->x68) {
            func_0012E460(p->x68);
            func_002303A0(p->o, p->o->n58, 2);
            p->o->fl &= ~(1L << 26);
            func_00134320(&D_003DD490, p->o);
        }
    }
}
