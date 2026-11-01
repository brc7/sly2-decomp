struct C { unsigned char a[4]; };
struct S { int k; struct C c; int z; };
void func_00227788(struct S *p);
void RgbaFromSpeakerCid(struct C *pc, int a);
void func_00227728(struct S *p, int k, int b)
{
    struct C c;
    func_00227788(p);
    p->k = k;
    RgbaFromSpeakerCid(&c, b);
    p->c = c;
}
