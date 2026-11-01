struct M { int n0; int n4; int n8; };
struct T { char pad[0xF3C]; int nF3C; };
struct P { char pad[8]; int n8; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; struct P *p40; char pad3[0xC]; int n50; };
int func_00209518(void *p);
int BHVANIM_FHandleMessage(struct S *p, int msg, struct M *pm);
void func_0020C760(int n);
int BHVSTRIKE_FHandleMessage(struct S *p, int msg, struct M *pm) {
    if (msg == 9 && p->p40 && func_00209518(p->p40) == pm->n0) {
        switch (pm->n8) {
        case 0x2D: p->pt->nF3C = 1; return 1;
        case 0x2E: p->pt->nF3C = 0; return 1;
        case 0x30: p->n50 = 0; func_0020C760(p->p40->n8); return 1;
        }
    }
    return BHVANIM_FHandleMessage(p, msg, pm);
}
