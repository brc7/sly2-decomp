struct S { char pad[0x184]; int n; char p1[0x38]; int x1c0; };
extern char D_002DEB30[];
int func_0017F1D0(struct S *p, void *message);
void BroadcastGameEvent(void *g, int n, struct S *p);
void func_00181268(struct S *p, int n);
int func_001811B8(struct S *p, void *message)
{
    register int four __asm__("$2") = 4; if (p->x1c0 == four) return 0;
    if (!func_0017F1D0(p, message)) return 0;
    if (p->n < 0) return 1;
    if (p->n <= 0) BroadcastGameEvent(D_002DEB30, 0x18, p);
    func_00181268(p, 4);
    return 1;
}
