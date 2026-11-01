struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct E { union { int oid; struct O *po; } u; int pad; };
struct L { int c; struct E ae[8]; };
extern char D_003DD490[];
struct O *func_0015E938(void *p, int k, int oid, void *q);
/** @sly2-annot addr=0x148668
 * Purpose: Resolve each ASH oid to an object (PloFindSwObject kind 4) and keep the asegs
 * Info: proposed name PostAshLoad (T1); TU button
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void func_00148668(struct L *l, void *q)
{
    int i;
    for (i = 0; i < l->c; i++) {
        struct O *o = func_0015E938(D_003DD490, 4, l->ae[i].u.oid, q);
        if (o) {
            struct VTE *e;
            l->ae[i].u.po = o;
            e = &o->vt[9];
            ((void (*)(void *))e->pfn)((char *)o + e->d);
        } else {
            l->ae[i].u.po = 0;
        }
    }
}
