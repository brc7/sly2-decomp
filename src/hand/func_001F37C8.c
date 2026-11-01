struct Q { char pad[0x118]; char dl[1]; };
struct O { char pad[0x10]; struct Q *pq; char pad2[0x20 - 0x14]; char sub[1]; };
struct E { char pad[0xDC]; struct O *po; };
struct P { char pad[0xE8]; struct E *pe; };
int FFindDlEntry__FP2DLPv(void *, void *);
void RemoveDlEntry__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void func_001C1440(struct Q *, void *);
void func_001F37C8(struct P *p, struct Q *q)
{
    struct O *o = p->pe->po;
    struct Q *old = o->pq;
    if (old == q) return;
    if (old) {
        if (FFindDlEntry__FP2DLPv(old->dl, o)) RemoveDlEntry__FP2DLPv(old->dl, o);
    }
    if (q) {
        if (!FFindDlEntry__FP2DLPv(q->dl, o)) {
            AppendDlEntry__FP2DLPv(q->dl, o);
            func_001C1440(q, o->sub);
        }
    }
    o->pq = q;
}
