struct R { int id; int res; int x; };
struct E { int a; struct R ar[3]; };
struct S { char pad[0x58]; void *p58; void *p5C; char pad2[0x150 - 0x60]; void *p150; char pad3[0x2C4 - 0x154]; int c; struct E a[8]; };
int func_001C4700(void *p, int a);
int func_00152520(struct S *p, int id);
void func_002303A0(struct S *p, void *q, int n);
void func_001524A0(struct S *p, int a, int b);
/** @sly2-annot addr=0x152390
 * Purpose: Find move entry by id (or, id==-1, the one matching the current aseg state 0x1C4700), optionally
 *   rebind result, stop asegs (0x2303A0 x2) and apply entry's aseg j
 * Info: proposed name PlayDancerMove (T2); TU rhythm
 * Confidence: low (draft) [T2 0.35] - evidence: semantic review; sources: r2_131F70
 * See: docs/reference/functions/gameplay-objects.md#rhythm-0x1520c8-0x1539a8
 * @end-sly2-annot */
void func_00152390(struct S *p, int id, int idNew, int j)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->a[i];
        if ((id == -1 && func_001C4700(p->p150, pe->a)) || pe->a == id) {
            if (idNew != -1) {
                int r = func_00152520(p, idNew);
                if (r)
                    pe->ar[0].res = r;
            }
            func_002303A0(p, p->p58, 2);
            func_002303A0(p, p->p5C, 2);
            func_001524A0(p, pe->ar[j].res, pe->ar[j].x);
            return;
        }
    }
}
