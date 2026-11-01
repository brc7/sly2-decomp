// Complete WIP; true source-object lookup and sound-reference contracts.
static inline void Clamp(float *f,float *b) {float lo=b[0],hi=b[1]; if(*f<lo)*f=lo; else if(hi<*f)*f=hi;}
struct VT { char pad[0x40]; short d; short pd; int (*kind)(void *); };
struct M { char pad[0xC]; struct VT *vt; };
struct C { char pad[0x20]; float value; };
struct O { char pad[0xFC4]; struct C *curve; };
struct G { char pad[0x475C]; float offset; };
extern float D_002C3EA0[3];
extern float D_002C3EB0[2];
struct O *PloFindSwObject__FP2SWi3OIDP2LO(struct G *, int, int, void *);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct O *, int);
struct M *func_001FBB90(int);
void func_001FB2F8(struct M *, struct O *);
void func_001FB4F0(struct M *, float, float);
void func_001FB1F8(struct M *, float);
void func_001FB2C0(struct M *, float);
void func_001FB520(struct M *, float);
void func_00258040(struct M **, struct M *);
static inline struct O *SelectedValue(struct O *p){struct O *value;__asm__("daddu %0,%1,$zero":"=r"(value):"r"(p));return value;}
void func_00210740(float offset, float fade, float scale, struct G *p, int id, int oid, int x, int y, struct O *fallback, void *owner, void *unused, struct M **out)
{
    struct O *o;
    struct M *m;
    if (id == -1)
        return;
    __asm__ volatile("" ::: "$4");
    if(oid!=-1)fallback=PloFindSwObject__FP2SWi3OIDP2LO(p,4,oid,owner);
    o=SelectedValue(fallback);
    if (o && FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x38) && o->curve) {
        float v = o->curve->value;
        float f = D_002C3EA0[0] + v * (D_002C3EA0[1] + v * D_002C3EA0[2]);
        Clamp(&f, D_002C3EB0);
        offset += f;
    }
    offset += p->offset;
    m = func_001FBB90(id);
    func_001FB2F8(m, o);
    func_001FB4F0(m, (float)x, (float)y);
    func_001FB1F8(m, scale);
    func_001FB2C0(m, offset);
    func_001FB520(m, fade);
    if (m->vt->kind((char *)m + m->vt->d) == 2)
        func_00258040(out, m);
}
