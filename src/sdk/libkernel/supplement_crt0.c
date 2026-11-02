// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
extern char D_002AAD00[];
extern char D_002AAD10[];
extern int D_002C4AB8;
extern int D_002C4ABC;
int func_00234A20(struct SemaParam *);

void supplement_crt0(void)
{
    struct SemaParam s1;
    struct SemaParam s2;

    s1.option = (unsigned int)D_002AAD00;
    s1.maxCount = 1;
    s1.initCount = 1;
    s2.maxCount = 1;
    s2.initCount = 1;
    s2.option = (unsigned int)D_002AAD10;
    D_002C4AB8 = func_00234A20(&s1);
    D_002C4ABC = func_00234A20(&s2);
}
