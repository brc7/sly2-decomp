struct G { int a; };
extern struct G D_003DD490;
extern int D_002E1E00;
extern char D_002DEB30[];
struct S { char pad[0x184]; int n184; char pad2[0x38]; int n1C0; };
int FSwHandsOff(struct G *g);
int func_0017F1D0(struct S *p, int *pn);
void UpdateHackshipHealthBar(struct S *p);
void BroadcastGameEvent(void *g, int n, int m);
void func_0017FD08(struct S *p, int n);
int HACKSHIP_TakeDamage(struct S *p, int *pn) {
    if (p->n1C0 < 5) { if (p->n1C0 >= 2) return 0; }
    if (FSwHandsOff(&D_003DD490)) return 0;
    if (D_002E1E00 & 1) return 0;
    if (!func_0017F1D0(p, pn)) return 0;
    UpdateHackshipHealthBar(p);
    if (p->n184 > 0) { BroadcastGameEvent(D_002DEB30, 0xF, *pn); func_0017FD08(p, 2); }
    else func_0017FD08(p, 4);
    return 1;
}
