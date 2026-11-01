typedef int qword __attribute__((mode(TI)));
struct E;
struct O { char pad[0x1A0]; union { qword q; float g[4]; } a; union { qword q; float g[4]; } b; char p1[0x1DC - 0x1C0]; struct S *x1DC; };
struct S { char pad[0xF8]; int xF8; int xFC; int x100; struct E *x104; struct E *x108; struct O **x10C; int x110; int x114; };
struct M { struct O *po; void *pv; };
struct G { char pad[0x3D70]; int x3D70; };
extern struct G D_003DD490;
int func_001C1700(struct S *p, int z, qword a, qword b, int c, void **ap);
int func_001BD8A8(struct O *po, void *pv);
void func_001BDE80(int n, struct M *pm);
void func_001C1D20(struct S *p);
void func_001C1A38(int c, struct E *ae, int bit, float g0, float g1);
/** @sly2-annot addr=0x1C1BA0
 * Purpose: Register obstacle in a zone unless it overlaps an existing one (then message 2), growing arrays
 * Info: proposed name AddPathzoneObstacle (T2); TU path
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C1BA0(struct S *p, struct O *po)
{
    void *ap[0x20];
    int n, i;
    n = func_001C1700(p, 0, po->a.q, po->b.q, 0x20, ap);
    for (i = 0; i < n; i++) {
        if (func_001BD8A8(po, ap[i])) {
            struct M m;
            m.po = po;
            m.pv = ap[i];
            func_001BDE80(2, &m);
            return;
        }
    }
    if (p->xF8 >= p->xFC)
        func_001C1D20(p);
    {
        int j;
        p->xF8++;
        for (j = 0; j < p->xF8; j++)
            if (!p->x10C[j]) break;
        p->x10C[j] = po;
        func_001C1A38(p->x100, p->x104, j, po->a.g[0], po->b.g[0]);
        func_001C1A38(p->x100, p->x108, j, po->a.g[1], po->b.g[1]);
    }
    po->x1DC = p;
    p->x100 += 2;
    p->x114 = ++D_003DD490.x3D70;
}
