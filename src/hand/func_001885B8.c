typedef struct { float x, y, z; } VEC3;
struct S { char pad[0x240]; VEC3 v; };
extern char D_002BE990[];
void func_002039C0(struct S *p);
void BindLoOidTable(struct S *p, int n, void *pv);
void func_001885B8(struct S *p)
{
    VEC3 *pv;
    func_002039C0(p);
    pv = &p->v;
    pv->x = 0.0f;
    pv->y = 0.0f;
    pv->z = -75.0f;
    BindLoOidTable(p, 0xAD, D_002BE990);
}
