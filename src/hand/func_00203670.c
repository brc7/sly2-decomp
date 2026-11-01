struct G;
extern struct G D_003DD490;
struct O { char pad[0xF38]; void *p; };
void HandleBasicMessage(void *p, int msg, void *pm);
struct O *PpoHero(struct G *pg);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *o, int n);
void func_00202030(void *p);
void func_002021F8(void *p);
void func_00203670(void *p, int msg, void *pm)
{
    struct O *o;
    HandleBasicMessage(p, msg, pm);
    if (msg != 0x11) return;
    o = PpoHero(&D_003DD490);
    if (o && FIsBasicDerivedFrom__FP5BASIC3CID(o, 0x34) && o->p) {
        func_00202030(o->p);
        func_002021F8(o->p);
    }
}
