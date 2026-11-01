extern char D_003DD490[];
struct S { char pad[0xEB8]; int a[8]; int n; int b[8]; int c; };
int func_0015E938(void *p, int k, int x, struct S *ps);
/** @sly2-annot addr=0x20AAA8
 * Purpose: Resolve attack oids into objects +0xEDC
 * Info: proposed name ResolveStepguardAttacks (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AAA8(struct S *p)
{
    int i;
    p->c = 0;
    for (i = 0; i < p->n; i++) {
        int r = func_0015E938(D_003DD490, 4, p->a[i], p);
        if (r)
            p->b[p->c++] = r;
    }
}
