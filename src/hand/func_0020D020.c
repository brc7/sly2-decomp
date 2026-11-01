typedef int qword __attribute__((mode(TI)));
extern qword D_002C3E00[5];
extern float D_002C3E50;
extern float D_002C3E54;
int func_0020CFC0(void);
struct O { char pad[0x208]; int n208; char pad2[0x4A0 - 0x20C]; int n4A0; char pad3[0xF80 - 0x4A4]; int nF80; };
int func_00209A68(struct O *o, int n);
qword ConvertAloPos(struct O *o, int a, qword v);
int func_00173610(qword *pv, float f, float g, int a, int (*pfn)(void));
int func_0020D020(struct O **pp)
{
    struct O *o = *pp;
    unsigned int i;
    if (o->n208 || o->nF80 != -1 || !o->n4A0 || func_00209A68(o, 0x13))
        return 0;
    i = 0;
    do {
        qword v = ConvertAloPos(*pp, 0, D_002C3E00[i]);
        if (func_00173610(&v, D_002C3E50, D_002C3E54, 1, func_0020CFC0))
            break;
        i++;
    } while (i < 5);
    return !(i < 5);
}
