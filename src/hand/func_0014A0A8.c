typedef int qword __attribute__((mode(TI)));
struct E { int a; int oid; void *p; };
struct Q { char pad[0xE8]; int c; char pad2[0xF4 - 0xEC]; qword *aq; };
struct P { char pad[0x54]; int c; struct E *ae; struct Q *q; };
extern char D_003DD490[];
void func_001A5C48(struct P *p);
struct Q *func_001E94B0(int k, int c);
void *func_0015E938(void *p, int k, int oid, void *q);
qword func_001A62E8(void *p);
void func_001E6180(struct Q *q);
/** @sly2-annot addr=0x14A0A8
 * Purpose: vt15: PostLoLoad, build a path/curve object (0x1E94B0 kind 0x62C) from the points
 * Info: proposed name PostRouteLoad (T2); class CID123 slot15; TU tu_14A008
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/guards-ai.md#tu_14a008-0x14a008-0x14b660
 * @end-sly2-annot */
void func_0014A0A8(struct P *p)
{
    int i;
    func_001A5C48(p);
    p->q = func_001E94B0(0x62C, p->c);
    p->q->c = p->c;
    for (i = 0; i < p->c; i++) {
        struct E *e = &p->ae[i];
        void *o = func_0015E938(D_003DD490, 4, e->oid, p);
        e->p = o;
        if (o) {
            qword *pq = (qword *)((i << 4) + (int)p->q->aq);
            *pq = func_001A62E8(o);
        }
    }
    func_001E6180(p->q);
}
