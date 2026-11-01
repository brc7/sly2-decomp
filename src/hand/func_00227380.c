struct S { char pad[0xE0]; int a; int b; char c[0x10]; char d[4]; };
void func_00231768(struct S *p, void *q);
int func_00117C80(void *q);
void ReadGeom__18CBinaryInputStreamP4GEOM(void *q, void *a);
void CBinaryInputStream_ReadBspc(void *q, void *a, void *b);
void LoadOptionsFromBrx__FPvP18CBinaryInputStream(struct S *p, void *q);
void func_00227380(struct S *p, void *q)
{
    func_00231768(p, q);
    p->a = func_00117C80(q);
    ReadGeom__18CBinaryInputStreamP4GEOM(q, p->d);
    CBinaryInputStream_ReadBspc(q, p->d, p->c);
    LoadOptionsFromBrx__FPvP18CBinaryInputStream(p, q);
}
