typedef int qword __attribute__((mode(TI)));
struct O { char pad[0x98]; short k; char pad2[0xA0 - 0x9A]; int nA0; char pad3[0xF0 - 0xA4]; qword pos; };
struct P { char pad[0x5C]; struct O *po; };
int func_00230380(int);
qword ConvertXfVec(int, int, qword);
void func_001FAC98(struct P *p, qword v)
{
    if (func_00230380(p->po->k)) {
        struct O *po = p->po;
        qword a = po->pos;
        qword r = ConvertXfVec(0, po->nA0, v);
        qword s;
        __asm__("vadd.xyzw %0, %1, %2" : "=j"(s) : "j"(a), "j"(r));
        po->pos = s;
    }
}
