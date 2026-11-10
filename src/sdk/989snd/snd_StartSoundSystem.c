// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct CB { void (*fn)(int, long); int pad; long arg; } CB;
typedef struct RPC { int x[16]; } RPC;
extern int *gSndCommandBuffePtr[2];
extern int gCommandBuffeBytesAvail[2];
extern CB *gSndCommandReturnDefPtr[2];
extern int *gReturnValuesPtr[2];
extern int gLoadBusy, D_002C46B8;
extern CB gLoadReturnDef;
extern int D_00443B80[], D_00444B80[], D_00447B80[], D_00447FC0[];
extern CB D_00445B80[], D_00446B80[];
extern RPC D_00443800, D_00448440;
extern int gStats[];
extern unsigned int gLoadReturnValue;
extern char D_002AA100[], D_002AA128[];
void printf(const char *, ...);
void func_00236A60(int) __asm__("sceSifInitRpc");
int func_002370A8(void *, int, int) __asm__("sceSifBindRpc");
int snd_SendIOPCommandAndWait__FiiPc(int, int, void *);

void snd_StartSoundSystem(int flags)
{
    int i;
    int buf[2];

    gSndCommandBuffePtr[0] = D_00443B80;
    gSndCommandBuffePtr[1] = D_00444B80;
    gReturnValuesPtr[0] = D_00447B80;
    gReturnValuesPtr[1] = D_00447FC0;
    gSndCommandReturnDefPtr[0] = D_00445B80;
    gSndCommandReturnDefPtr[1] = D_00446B80;
    if (flags & 2)
        D_002C46B8 = 1;
    else
        D_002C46B8 = 0;
    func_00236A60(0);
    do {
        if (func_002370A8(&D_00443800, 0x123456, 0) < 0) {
            printf(D_002AA100, D_002AA128, 0xB5);
            while (1)
                ;
        }
        i = 9999;
        while (i--)
            ;
    } while (!D_00443800.x[9]);
    gLoadBusy = 0;
    gLoadReturnDef.fn = 0;
    gLoadReturnDef.arg = 0;
    gLoadReturnValue = 0;
    do {
        if (func_002370A8(&D_00448440, 0x123457, 0) < 0) {
            printf(D_002AA100, D_002AA128, 0xCA);
            while (1)
                ;
        }
        i = 9999;
        while (i--)
            ;
    } while (!D_00448440.x[9]);
    ((volatile int *)gStats)[0] = 0;
    ((volatile int *)gStats)[4] = 0;
    *D_00443B80 = 0;
    *D_00444B80 = 0;
    gCommandBuffeBytesAvail[0] = 0xFFC;
    buf[0] = (int)gStats;
    buf[1] = flags;
    gCommandBuffeBytesAvail[1] = 0xFFC;
    snd_SendIOPCommandAndWait__FiiPc(0, 8, buf);
}

// ---- Common symbols defined here (docs/guides/tu-merging.md "Data", address names): g++ 2.95 emits a class or
// template static data member as COMMON (C: a tentative definition), and the linker allocates commons at the end
// of .bss (small ones in .sbss), which is why these addresses are not in TU order. Sizes are the gaps to the next
// name in the ELF; a 16-aligned start with a 16-multiple size is a qword array (commons take the type's alignment).
typedef int qword __attribute__((mode(TI)));
qword c_00443800[4] __asm__("D_00443800");   /* .bss 0x40 */
qword c_00443B80[122] __asm__("D_00443B80");   /* .bss 0x7A0 */
qword c_00444B80[256] __asm__("D_00444B80");   /* .bss 0x1000 */
qword c_00445B80[256] __asm__("D_00445B80");   /* .bss 0x1000 */
qword c_00446B80[256] __asm__("D_00446B80");   /* .bss 0x1000 */
qword c_00447B80[68] __asm__("D_00447B80");   /* .bss 0x440 */
qword c_00447FC0[68] __asm__("D_00447FC0");   /* .bss 0x440 */
char c_00448400[63] __asm__("gStats");   /* .bss 0x3F */
qword c_00448440[4] __asm__("D_00448440");   /* .bss 0x40 */
qword c_00448480[4] __asm__("gLoadReturnValue");   /* .bss 0x40 */
