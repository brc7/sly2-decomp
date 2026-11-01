struct E { int id; char pad[0x18]; int c; void *a[3]; };
struct S { char pad[0x23C]; int c; struct E *ae; struct E *pe; };
void func_001F6A68(struct S *p);
void ShowSoGlobsById(struct S *p, void *pv);
void func_001C7A58(struct S *p, int id)
{
    int i, n;
    {
    int j; struct E *pe;
    n = p->c;
    for (j = 0; ; j++) {
        if (j >= n) {
            p->pe = p->ae;
            break;
        }
        pe = &p->ae[j];
        if (id == pe->id) {
            p->pe = pe;
            break;
        }
    }
    }
    func_001F6A68(p);
    for (i = 0; i < p->pe->c; i++) ShowSoGlobsById(p, p->pe->a[i]);
}
