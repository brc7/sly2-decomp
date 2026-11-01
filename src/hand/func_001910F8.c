struct S { char *p0; char pad[0x64]; void *x68; };
void *PloNew__F3CIDP2SO3OIDi(int, int, int, int);
void func_00221268(void *, int);
void func_00194118(struct S *);
void func_001A0300(char *, void *, void *);
void func_0020B868(void *);
void func_00137A30(char *, int, int, void *, int, int, int, void *);
void func_001CC5E8(void *, char *, char *);
void func_001910F8(struct S *p) {
    char *pb = (char *)p;
    p->x68 = PloNew__F3CIDP2SO3OIDi(0xF, 0, 0x148, -1);
    func_00221268(p->x68, 0);
    func_00194118(p);
    func_001A0300(p->p0, pb + 0x70, pb + 0x170);
    func_001A0300(p->p0, pb + 0x330, pb + 0x430);
    func_0020B868(pb + 0x180);
    func_0020B868(pb + 0x240);
    func_00137A30(p->p0, 0xC, 3, pb + 0x2F0, 0, 0, 0, pb + 0x314);
    func_001CC5E8(pb + 0x440, p->p0, p->p0 + 0x1030);
}
