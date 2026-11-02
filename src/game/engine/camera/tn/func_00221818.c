typedef int qword __attribute__((mode(TI)));
struct E { int id; int pad[3]; qword q[8]; };
extern struct E D_002C4120;
struct S { char pad[0x110]; int c; struct E *a; struct E *cur; int x11C; };
void func_00221940(struct S *p, int id);
/** @sly2-annot addr=0x221818
 * Purpose: chooses/creates the TNFN record (max 4) for an id; resets 0x118
 * Info: proposed name SelectTnTnfn (T2); TU tn
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void func_00221818(struct S *p, int id)
{
    int i;
    struct E *pe;
    func_00221940(p, id);
    if (p->c >= 4) return;
    for (i = 0; i < p->c; i++) {
        if (p->a[i].id == id) {
            p->cur = &p->a[i];
            return;
        }
    }
    if (p->x11C) {
        p->cur = p->a;
        return;
    }
    pe = &p->a[p->c++];
    p->cur = pe;
    if (p->c >= 2)
        *pe = p->a[p->c - 2];
    else
        *pe = D_002C4120;
    p->cur->id = id;
}
