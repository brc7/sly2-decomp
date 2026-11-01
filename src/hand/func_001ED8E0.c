struct Q { int a; int b; unsigned short c; unsigned short d; };
struct S { char pad[0x14]; int a; int b; int d; int c; char pad2[0x30 - 0x24]; int x30; int dl34; };
extern struct DL D_003E159C;
void func_0011A050(void *pdl, int ib);
void PrependSlEntry(struct DL *pdl, struct S *p);
void func_0011A070(struct DL *pdl, struct S *p);
void func_001ED8E0(struct S *p, struct Q *q)
{
    p->a = q->a;
    p->b = q->b;
    p->c = q->c;
    p->d = q->d;
    func_0011A050(&p->dl34, 0x10);
    if (p->x30 == 0) PrependSlEntry(&D_003E159C, p);
    else func_0011A070(&D_003E159C, p);
}
