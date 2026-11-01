struct T { char pad[0xFE8]; int nFE8; };
struct S { char pad[0x10]; struct T *pt; int pad2; void *p18; char pad3[0x14]; int a30[4]; };
void UpdateBhv(struct S *p);
int func_00174FB8(void *p);
int func_001C1FA0(void *pe, void *p, float g);
void func_0017E9B0(struct S *p);
void func_0017E918(struct S *p) {
    UpdateBhv(p);
    if (p->pt->nFE8 & 1) {
        if (func_00174FB8(p->p18) && !func_001C1FA0(p->a30, p->pt, 25.0f)) func_0017E9B0(p);
    } else if (!func_001C1FA0(p->a30, p->pt, 25.0f)) func_0017E9B0(p);
}
