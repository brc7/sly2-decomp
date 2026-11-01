typedef int qword __attribute__((mode(TI)));
struct S { int a[0x30]; };
void *func_001C0960(void *p, qword a, qword b, int c, int d, int e);
int func_001C1700(void *p, int z, qword a, qword b, int c, void **ap);
void *func_001BC688(void *p, qword a, qword b, int n, struct S *ps);
void *func_0014C9F8(void *p, int d, void *r, struct S *ps, int c, int d2, int one);
/** @sly2-annot addr=0x1C0A40
 * Purpose: Clip segment to pathzone then against obstacles (0x1BC688), returns blocker
 * Info: proposed name PloClipPathzoneSegment (T2); TU path
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void *func_001C0A40(void *p, qword a, qword b, int c, int d)
{
    void *ap[0x80];
    struct S s;
    void *pv;
    int n, i;
    pv = func_001C0960(p, a, b, c, d, 2);
    if (!pv) return 0;
    n = func_001C1700(p, 0, a, b, 0x80, ap);
    for (i = 0; i < n; i++) {
        void *r = func_001BC688(ap[i], a, b, 2, &s);
        pv = func_0014C9F8(pv, d, r, &s, c, d, 1);
        if (!pv) goto out;
    }
out:
    return pv;
}
