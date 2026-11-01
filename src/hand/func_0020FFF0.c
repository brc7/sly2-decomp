typedef struct { short d; short pad; void *pfn; } VTE;
struct Y { char pad[0xC]; VTE *vt; };
struct D { int a; int b; int c; int d; };
int func_0015E650(void *p, int k, int a, int b, int c, void **ppo);
int func_00132B10(void *o, int n);
struct Y *func_001F67E0(void *o, int b);
void func_001333C0(struct Y *y, int x);
void func_001F0948(struct Y *y, int id);
void func_001F0660(struct Y *y, int id);
/** @sly2-annot addr=0x20FFF0
 * Purpose: Find root object, its SM by oid, then SeekSma or SetSmaGoal to trigger/untrigger goal; optional
 *   returned SMA ref
 * Info: proposed name TriggerRsmgOne (T2); TU sw
 * Notes: names_v2 v1 proposed TriggerRsmg__FP2SWiP4RSMGP2LOi (0.012)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r7_1F8DD8
 * See: docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void func_0020FFF0(void *p, struct D *pd, int b, int fl, int x, struct Y **ppy)
{
    void *o;
    struct Y *y;
    int id;
    if (ppy)
        *ppy = 0;
    if (func_0015E650(p, 0x204, pd->a, b, 1, &o) != 1)
        return;
    if (!func_00132B10(o, 0x30))
        return;
    y = func_001F67E0(o, pd->b);
    if (!y)
        return;
    if (fl & 1)
        id = pd->c;
    else
        id = pd->d;
    if (id == -1)
        return;
    if (x) {
        *ppy = y;
        ((void (*)(void *))y->vt[1].pfn)((char *)y + y->vt[1].d);
        func_001333C0(y, x);
    }
    if (fl & 2)
        func_001F0948(y, id);
    else
        func_001F0660(y, id);
}
