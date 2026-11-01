struct VTE { short d; short pad; void (*pfn)(); };
struct O { int a[3]; struct VTE *vt; char pad[0x40]; int n50; float g54; int pad58[2]; float g60; int n64; };
struct S { char pad[0x4020]; struct O *ao[7]; };
extern float D_002BDD00, D_002BDD04, D_002BDD08, D_002BDD0C, D_002BDD10, D_002BDD14, D_002BDD18;
struct O *PloNew__F3CIDP2SO3OIDi(int a, int b, int c, int d);
static inline void Init(struct O *o) { struct VTE *e = &o->vt[9]; e->pfn((char *)o + e->d); }
void func_001569B8(struct S *p)
{
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C2, -1);
        o->n50 = 0;
        p->ao[0] = o;
        Init(o);
    }
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C3, -1);
        o->n50 = 1;
        o->g60 = D_002BDD00;
        p->ao[1] = o;
        Init(o);
    }
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C4, -1);
        o->n50 = 0;
        o->g54 = D_002BDD04;
        p->ao[2] = o;
        Init(o);
    }
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C5, -1);
        o->g54 = D_002BDD08;
        o->g60 = D_002BDD0C;
        o->n50 = 0;
        p->ao[3] = o;
        Init(o);
    }
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C7, -1);
        o->n64 = 3;
        o->g60 = -1.0f;
        o->n50 = 0;
        p->ao[5] = o;
        Init(o);
    }
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C6, -1);
        o->g54 = D_002BDD10;
        o->g60 = D_002BDD14;
        o->n50 = 0;
        p->ao[4] = o;
        Init(o);
    }
    {
        struct O *o = PloNew__F3CIDP2SO3OIDi(0x75, 0, 0x2C8, -1);
        o->n50 = 0;
        o->g54 = D_002BDD18;
        p->ao[6] = o;
        Init(o);
    }
}
