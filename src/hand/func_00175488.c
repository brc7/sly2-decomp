typedef int qword __attribute__((mode(TI)));
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern qword D_002A2DD0;
struct VTE { short d; short pad; int (*pfn)(void *, void *); };
struct Q { char pad[0xC]; struct VTE *vt; };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; struct Q *q38; int n3C; int n40; };
struct P { char pad[0xA0]; int nA0; char pad2[0x1C]; int nC0; };
struct S { char pad[0x10]; void *p10; int pad2; char a18[0x18]; struct Q *q30; int n34; float g38; struct P *p3C; };
void func_00133448(struct Q *q, void *p);
void func_001333C0(struct Q *q, void *p);
void func_0020AC28(void *p, void *po, int n, qword v);
void func_001EF0D8(void *p);
void func_001EF038(struct S *p);
struct O *func_00205438(void *p, int n);
/** @sly2-annot addr=0x175488
 * Purpose: BHVATTACK state switch: 0 = subscribe to victim, push BHVSTRIKE with the chosen attack entry and
 *   aim; 1 = push BHVANIM flag 4 (follow-through after a confirmed hit); -1 = clear busy
 * Info: proposed name SetBhvattackState (T1); TU guardbhv
 * Notes: runtime2/4 confirmed the BHVATTACK role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00175488(struct S *p, int n) {
    if (n == p->n34) return;
    if (p->n34 == 0) {
        func_00133448(p->q30, p->p10);
        p->q30 = 0;
        func_0020AC28(p->p10, 0, 0, D_002A2DD0);
    }
    func_001EF0D8(p->a18);
    p->n34 = n;
    p->g38 = D_002DDED0.t;
    switch (n) {
    case 0:
        {
            struct O *o;
            struct VTE *e;
            func_001333C0(p->q30, p->p10);
            o = func_00205438(p->p10, 0x9F);
            o->q38 = p->q30;
            o->n40 = p->p3C->nC0;
            if (p->p3C->nA0) {
                struct B { char pad[0x30]; qword v; } b;
                struct Q *q = p->q30;
                struct VTE *f = &q->vt[0x20];
                func_0020AC28(p->p10, p->p3C, f->pfn((char *)q + f->d, &b), b.v);
            }
            e = &o->vt[8]; ((void (*)(void *))e->pfn)((char *)o + e->d);
        }
        break;
    case 1:
        {
            struct O *o = func_00205438(p->p10, 0x9D);
            struct VTE *e;
            o->n3C = 4;
            o->q38 = p->q30;
            e = &o->vt[8]; ((void (*)(void *))e->pfn)((char *)o + e->d);
        }
        break;
    case -1:
        func_001EF038(p);
        break;
    }
}
