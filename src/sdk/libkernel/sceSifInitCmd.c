// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* sceSifInitCmd (libkernel sifcmd). Handler func field must be a function pointer type (Claude agent). */
typedef struct { void (*func)(void *, void *); void *data; int pad; } HANDLER;
typedef struct {
    void *pktbuf;
    void *unused;
    void *iopbuf;
    HANDLER *sys;
    int nsys;
    HANDLER *usr;
    int nusr;
    int *sreg;
} CMDDATA;
typedef struct { unsigned int size; void *dest; int cid; unsigned int opt; void *buf; } CAPKT;

extern int D_002C5A60;
extern unsigned char D_0044ED00[0x80];
extern unsigned char D_0044ED80[0x40];
extern CAPKT D_0044EDC0;
extern int D_0044EDD4;
extern CMDDATA D_0044EDD8;
extern HANDLER D_0044EE00[32];
extern int D_0044EF80[32];

int func_00239450(void);
void func_002394A0(void);
void func_00234CA0(int);
void func_00234E00(void);
int func_00234730(int, void *, int);
int func_00235248(int);
unsigned int func_00234E30(unsigned int);
unsigned int func_00234E20(unsigned int, unsigned int);
void func_0023AFA8(unsigned int, void *, unsigned int, void *, void *, unsigned int);
void func_0023AAB8(void *, void *);
void func_0023AA98(void *, void *);
void func_0023B028(void);

void sceSifInitCmd(void)
{
    int i;

    func_00239450();
    if (D_002C5A60) {
        func_002394A0();
        return;
    }
    D_002C5A60 = 1;
    D_0044EDD8.pktbuf = (void *)((unsigned int)D_0044ED00 | 0x20000000);
    D_0044EDD8.unused = (void *)((unsigned int)D_0044ED80 | 0x20000000);
    D_0044EDD8.iopbuf = 0;
    D_0044EDD8.sys = D_0044EE00;
    D_0044EDD8.nsys = 32;
    D_0044EDD8.usr = 0;
    D_0044EDD8.nusr = 0;
    D_0044EDD8.sreg = D_0044EF80;
    for (i = 0; i < 32; i++) {
        D_0044EE00[i].func = 0;
        D_0044EE00[i].data = 0;
    }
    for (i = 0; i < 32; i++)
        D_0044EF80[i] = 0;
    D_0044EE00[0].func = func_0023AAB8;
    D_0044EE00[0].data = &D_0044EDD8;
    D_0044EE00[1].func = func_0023AA98;
    D_0044EE00[1].data = &D_0044EDD8;
    func_002394A0();
    func_00234CA0(0);
    if (*(volatile unsigned int *)0x1000E010 & 0x20)
        *(volatile unsigned int *)0x1000E010 = 0x20;
    if (!(*(volatile unsigned int *)0x1000C000 & 0x100))
        func_00234E00();
    D_0044EDD4 = func_00234730(5, (void *)func_0023B028, 0);
    func_00235248(5);
    D_0044EDD8.iopbuf = (void *)func_00234E30(0x80000000);
    if (D_0044EDD8.iopbuf) {
        D_0044EDC0.buf = D_0044ED00;
        func_0023AFA8(0x80000000, &D_0044EDC0, 0x14, 0, 0, 0);
        return;
    }
    {
        while (!(func_00234E30(4) & 0x20000))
            ;
        D_0044EDD8.iopbuf = (void *)func_00234E30(2);
        func_00234E20(0x80000000, (unsigned int)D_0044EDD8.iopbuf);
        func_00234E20(0x80000001, (unsigned int)&D_0044EDD8);
        D_0044EDC0.buf = D_0044ED00;
        D_0044EDC0.opt = 0;
        func_0023AFA8(0x80000002, &D_0044EDC0, 0x14, 0, 0, 0);
    }
}
