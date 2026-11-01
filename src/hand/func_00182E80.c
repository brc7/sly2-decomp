struct S { char pad[0x190]; char sub[0x2C]; int n; };
void InitShape(struct S *p);
void InitGrabspot(void *p);
void func_00182E80(struct S *p)
{
    InitShape(p);
    InitGrabspot(p->sub);
    p->n = 3;
}
