struct E;
struct S { char pad[0xF8]; int xF8; int xFC; int x100; struct E *x104; struct E *x108; int *x10C; int x110; int x114; };
struct G { char pad[0x3D70]; int x3D70; };
extern struct G D_003DD490;
void func_001C18B8(int n, struct E *a, int key);
/** @sly2-annot addr=0x1C1968
 * Purpose: Unregister obstacle (both axes) and bump nav version
 * Info: proposed name RemovePathzoneObstacle (T2); TU path
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C1968(struct S *p, int key)
{
    int i;
    for (i = 0; i < p->xFC; i++) {
        if (p->x10C[i] == key) break;
    }
    p->x10C[i] = 0;
    func_001C18B8(p->x100, p->x104, i);
    func_001C18B8(p->x100, p->x108, i);
    p->xF8--;
    p->x100 -= 2;
    p->x114 = ++D_003DD490.x3D70;
}
