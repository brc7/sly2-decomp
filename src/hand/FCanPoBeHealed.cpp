extern "C" void func_00129EC8(struct CRef *pref);
struct CRef {
    int tagk; int tag;
};
class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void Msg(int, void *, int, void **, CRef *); };
extern "C" int FCanPoBeHealed(OBase *p);
int FCanPoBeHealed(OBase *p)
{
    CRef ref;
    int f = 1;
    ref.tagk = 0x17;
    ref.tag = f;
    p->Msg(0xD, 0, 0, 0, &ref);
    if (ref.tagk == 0x17) f = ref.tag;
    func_00129EC8(&ref);
    return f;
}
