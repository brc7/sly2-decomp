struct Q { void *p0; int pad; float g8; float gC; };
struct E { char pad[0xDC]; struct Q *pq; };
struct P { char pad[0xE8]; struct E *pe; };
struct CB { int cb; int n; };
void *func_001AE2A0(struct CB *);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *, int);
static inline void *PvAllocMsg(struct CB *pcb, int cb, int n) { pcb->cb = cb; pcb->n = n; return func_001AE2A0(pcb); }
struct Q *func_001F8300(struct P *p)
{
    if (!p->pe) return 0;
    if (!p->pe->pq) {
        struct CB cb;
        struct Q *q = PvAllocMsg(&cb, 0x70, 0x10);
        q->p0 = p;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(p, 0x34)) q->g8 = 150.0f;
        else q->g8 = 75.0f;
        q->gC = 1.0f;
        p->pe->pq = q;
    }
    return p->pe->pq;
}
