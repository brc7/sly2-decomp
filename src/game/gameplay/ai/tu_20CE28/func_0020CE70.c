struct VE { short d; short pad; int (*pf)(void *, int); };
struct O { char pad[0xC]; struct VE *vt; };
struct S { struct O *po; int k; float t; int n; };
extern struct { float t; float dt; } D_002DDED0;
extern float D_002C3DF0;
int func_0020D020(struct S *p);
void func_0020D108(struct S *p, int k);

/** @sly2-annot addr=0x20CE70
 * Purpose: if all 5 probe points blocked for > D_002C3DF0 s -> InflictZap kind 0x4000 on self
 * Info: proposed name UpdateStuckWatch (T2); TU tu_20CE28
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/guards-ai.md#tu_20ce28-0x20ce28-0x20d5d0
 * @end-sly2-annot */
void func_0020CE70(struct S *p)
{
    int f, k;
    if (!p->n)
        return;
    f = func_0020D020(p);
    do {
        k = p->k;
        switch (k) {
        case 0:
            if (f)
                func_0020D108(p, 1);
            break;
        case 1:
            if (!f)
                func_0020D108(p, 0);
            else if (D_002DDED0.t - p->t > D_002C3DF0 && !p->po->vt[76].pf((char *)p->po + p->po->vt[76].d, 0x4000))
                func_0020D108(p, 2);
            break;
        case 2:
            if (!f)
                func_0020D108(p, 0);
            break;
        }
    } while (p->k != k);
}
