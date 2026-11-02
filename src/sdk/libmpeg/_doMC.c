// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    char d[0x1C];
} MCB;
typedef struct {
    int base;
    int x4;
    char pad8[0x28 - 0x8];
    void (*fnA[4])(MCB *);
    void (*fnB[4])(MCB *);
    MCB a[4];
    MCB b[4];
    int x128;
    int n;
    int flag;
    int pad134;
    int enable;
    int flag2;
} REFS;
typedef struct {
    char pad0[0x5A0];
    REFS refs[2];
    char pad820[0x830 - 0x820];
    int x830;
} MP;
extern char D_002AC598[];
void _sceMpegErr(MP *, char *);
void func_00252548(int, int);
void func_00252458(int, int, int);
void _doMC(MP *mp, int idx)
{
    int i;
    if (mp->refs[idx].enable) {
        for (i = 0; i < mp->refs[idx].n; i++) {
            mp->refs[idx].fnA[i](&mp->refs[idx].a[i]);
            mp->refs[idx].fnB[i](&mp->refs[idx].b[i]);
        }
    }
    if (mp->refs[idx].flag && mp->refs[idx].flag2)
        _sceMpegErr(mp, D_002AC598);
    if (mp->refs[idx].flag)
        func_00252548(mp->refs[idx].x128, mp->refs[idx].x4);
    else if (mp->refs[idx].flag2)
        func_00252548(mp->refs[idx].x128, mp->x830);
    else
        func_00252458(mp->refs[idx].x128, mp->x830, mp->refs[idx].x4);
}
