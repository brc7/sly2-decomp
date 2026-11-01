struct Q { char pad[0x24]; int n; char p1[0x14]; float g3c; int x40; char p2[8]; int x4c; char p3[0x10]; struct Q *next; };
struct E { int a; int b; int c; int d; struct Q *pqFirst; void *pqLast; int ib; int x1c; float g; int x24; int x28; float h; };
struct S;
void func_00181B60(struct S *p, struct E *pe);
float SMaxShape(int n);
void func_00181EB0(struct S *p, struct Q *q, int n);
void func_00181A90(struct S *p, struct E *pe)
{
    int i = 0;
    float g;
    struct Q *q;
    func_00181B60(p, pe);
    g = SMaxShape(pe->x24);
    for (q = pe->pqFirst; q; q = q->next) {
        q->n = -1;
        func_00181EB0(p, q, 0);
        q->g3c = g * ((float)i + 0.5f) / (float)pe->d;
        q->x40 = 0;
        q->x4c = pe->x1c;
        i++;
    }
}
