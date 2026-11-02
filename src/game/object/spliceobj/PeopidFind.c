struct KV { int k; int v; };
struct T { int c; struct KV *a; };
extern struct T D_00279AD0[];
struct R { int n0; int n4; };
struct VTE { short d; short p; struct R *(*pfn)(void *); };
struct VT { struct VTE a[7]; };
struct X { char pad[0xC]; struct VT *pvt; };
/** @sly2-annot addr=0x1FFCF8
 * Purpose: Binary-search the class option table (by class info vt6 index into D_00279AD0) for option id;
 *   return its record
 * Info: proposed name PeopidFind (T1); TU spliceobj
 * Notes: names_v2 B proposed RefGetObjectList__FP5BASIC4OTYPPFPv_v (0.885)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7_1F8DD8+verify_side
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/splice-scripting.md#6-source-files,
 *   docs/reference/functions/splice-scripting.md#spliceobj-0x1ffcf8-0x201620
 * @end-sly2-annot */
int PeopidFind(struct X *px, int k)
{
    struct R *r = px->pvt->a[6].pfn((char *)px + px->pvt->a[6].d);
    struct T *t = &D_00279AD0[r->n4];
    int lo = 0, hi = t->c - 1;
    struct KV *a = t->a;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (a[mid].k == k) return a[mid].v;
        if (k < a[mid].k) hi = mid - 1;
        else lo = mid + 1;
    }
    return 0;
}
