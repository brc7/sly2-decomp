struct A { char pad[0x184]; int n184; };
struct B { char pad[0xC]; struct A *pa; };
struct Q { struct A *pa; void *p; };
struct S { char pad[0x26C]; char x26C[0x49C - 0x26C]; struct B *pb; };
void HandlePoMessage(struct S *p, int k, struct Q *q);
void func_0020D460(struct B *pb, struct A *pa);
void func_001EED78(void *p, int k, struct Q *q);
void func_002075D0(struct S *p, int k, struct Q *q)
{
    HandlePoMessage(p, k, q);
    switch (k) {
    case 4:
        if (q->p == p && q->pa->n184)
            func_0020D460(p->pb, q->pa);
        break;
    case 5:
        if (q->p == p && q->pa == p->pb->pa)
            func_0020D460(p->pb, 0);
        break;
    }
    func_001EED78(p->x26C, k, q);
}
