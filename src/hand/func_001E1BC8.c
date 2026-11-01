typedef struct T { char pad[0x18]; int x18; } T;
typedef struct M { int a; int b; int c; } M;
typedef struct S { char pad[0x30]; T *pt; char pad2[0x24]; int x58; int x5C; } S;
void HandleBasicMessage(S *p, int a, M *m);
void func_001E1BC8(S *p, int a, M *m)
{
    HandleBasicMessage(p, a, m);
    if (a == 10 && m->a == p->x58 && m->c == p->pt->x18) p->x5C = 1;
}
