struct S { char pad[0x234]; int x; char pad2[0x268 - 0x238]; char y[4]; };
void RemoveSoHierarchy(struct S *p);
void func_001305B8(int a, struct S *b, int c, void *d, float f, float g);
void ReleaseAct(void *p);
void OnMissileRemove__FP7MISSILE(struct S *p)
{
    RemoveSoHierarchy(p);
    func_001305B8(0, p, 0, &p->x, 0.0f, 1.0f);
    ReleaseAct(p->y);
}
