typedef int qword __attribute__((mode(TI)));
struct S { char pad[0x1AC]; int n1ac; char pad2[0x10]; qword q1c0; int n1d0; int n1d4; };
extern qword D_002A2DA0;
void InitShape(struct S *p);
void func_00182C70(void *p);
void func_001C3390(struct S *p)
{
    InitShape(p);
    func_00182C70((char *)p + 0x190);
    /*B*/p->q1c0 = D_002A2DA0;
    p->n1d4 = 9;
    p->n1d0 = 8;
    p->n1ac = 1;/*E*/
}
