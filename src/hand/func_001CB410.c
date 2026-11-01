struct T { int n; int *ap; int c; int i; };
struct S { char pad[0xE0]; struct T *pt; };
int PspareFromIcopy(struct S *p, int i);
/** @sly2-annot addr=0x1CB410
 * Purpose: find spare whose object is plo
 * Info: proposed name PspareFromPlo (T2); TU proxy
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
int func_001CB410(struct S *p, int x)
{
    /*B*/int i; int n; if (p->pt->n != 1) return 0; n = 0; for (i = p->pt->i; i < p->pt->c; i++) { if (x == p->pt->ap[i]) { n = PspareFromIcopy(p, i); break; } } return n;/*E*/
}
