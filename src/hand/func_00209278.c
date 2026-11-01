typedef struct { short d; short pad; void *pfn; } VTE;
struct O { char pad[0xC]; VTE *vt; };
struct E { int k; int h; struct O *po; };
void func_0012F9B8(int h, char *p, int fl, struct O **ppo, float g, float f);
void PsmaApplySmOut(int h, char *p, int a, int fl, struct O **ppo);
void func_0020BF80(int h, char *p, int fl, struct O *po);
void func_0020C948(int h, char *p, int fl, struct O *po);
#define VC1(o) ((void (*)(void *))(o)->vt[1].pfn)((char *)(o) + (o)->vt[1].d)
void func_00209278(struct E *pe, char *p, int fl, float g)
{
    switch (pe->k) {
    case -1:
        break;
    case 0:
        func_0012F9B8(pe->h, p, fl, &pe->po, g, 1.0f);
        VC1(pe->po);
        break;
    case 1:
        PsmaApplySmOut(pe->h, p, -1, fl, &pe->po);
        VC1(pe->po);
        break;
    case 2:
        pe->po = (struct O *)(p + 0xDB0);
        func_0020BF80(pe->h, p, fl, pe->po);
        VC1(pe->po);
        break;
    case 3:
        pe->po = (struct O *)(p + 0xE20);
        func_0020C948(pe->h, p, fl, pe->po);
        VC1(pe->po);
        break;
    }
}
