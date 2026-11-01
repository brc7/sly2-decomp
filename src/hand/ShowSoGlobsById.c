#include "common.h"
struct Q { char pad[0x10]; ulong fl; };
struct E { int k; int pad[2]; int fl; int pad2[5]; struct Q *pq; };
struct P { char pad[0xB8]; int c; int pad2; struct E *ae; };
void ShowSoGlobsById(struct P *p, int k)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->k == k) {
            pe->fl &= ~0x800;
            if (pe->pq) pe->pq->fl &= ~(1L << 50);
        }
    }
}
