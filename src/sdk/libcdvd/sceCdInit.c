// sceCdInit (T1576): MATCH. Keys: the bind-ok path sits inside the while(1) loop and stores 6798 = 0 BEFORE the mode
// (its hi register then has a shorter live length, so global-alloc gives hi(6798) s4 and hi(679C) s5); the init-counter
// increment is a volatile store of a plain read (the read must not depend on the volatile 6764 store); init store order
// found by an exhaustive/hill-climb search (sched1 order), checked against the relocation symbols too.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int pad[9]; void *serve; int pad2[6]; } sceSifClientData;
extern int D_002C6750;
extern volatile int D_002C6760;
extern volatile int D_002C6764;
extern volatile int D_002C6768;
extern volatile int D_002C676C;
extern int D_002C6778;
extern int D_002C677C;
extern int D_002C6784;
extern int D_002C6788;
extern int D_002C678C;
extern int D_002C6790;
extern int D_002C6794;
extern int D_002C6798;
extern int D_002C679C;
extern sceSifClientData D_004504E8;
extern int D_00450540[];
extern int D_002C7940[];
extern char D_002AB570[];   // "Libcdvd bind err %d CD_Init %d\n"
extern char D_002AB590[];   // "Libcdvd Exit\n"
int func_00236960(const char *, ...);
int sceCdSyncS(int);
void sceSifInitRpc(unsigned int);
int sceSifBindRpc(sceSifClientData *, unsigned int, unsigned int);
void sceSifWriteBackDCache(void *, int);
int sceSifCallRpc(sceSifClientData *, int, unsigned int, void *, int, void *, int, void *, void *);
void cdvd_exit(void);
void cmd_sem_init(void);
int PowerOffCB(void);

static __inline__ void nopdelay(void)
{
    int i = 0x100000;
    while (i--) ;
}

int sceCdInit(int mode)
{
    int r;
    int *p;
    if (sceCdSyncS(1)) return 0;
    sceSifInitRpc(0);
    D_002C678C = -1;
    D_002C677C = -1;
    D_002C6794 = -1;
    D_002C6790 = -1;
    D_002C6798 = -1;
    D_002C6788 = -1;
    D_002C6764 = 1;
    D_002C6778 = 0;
    *(volatile int *)&D_002C679C = D_002C679C + 1;
    while (1) {
        int b = sceSifBindRpc(&D_004504E8, 0x80000592, 0);
        if (b < 0) {
            if (D_002C6750 > 0) func_00236960(D_002AB570, b, D_002C679C);
            nopdelay();
            continue;
        }
        if (D_004504E8.serve != 0) {
            D_002C6798 = 0;
            D_00450540[0] = mode;
            sceSifWriteBackDCache(D_00450540, 4);
            p = D_002C7940;
            if (sceSifCallRpc(&D_004504E8, 0, 0, D_00450540, 4, p, 0x10, 0, 0) < 0) {
                D_002C6764 = 0;
                return 0;
            }
            break;
        }
        nopdelay();
    }
    {
        // reply words read through the uncached mirror
        int b = *(int *)((unsigned int)&p[1] | 0x20000000);
        int c = *(int *)((unsigned int)&p[2] | 0x20000000);
        int a = *(int *)((unsigned int)&p[3] | 0x20000000);
        r = 1;
        if (a == 0xFF) {
        } else if (a == 0xFE) {
            D_002C6750 = 1;
        } else if (b / 256 < 2 || c / 256 < 2) {
            r = 2;
        }
    }
    D_002C6764 = 0;
    switch (mode) {
    case 5:
        if (D_002C6750 > 0) func_00236960(D_002AB590);
        cdvd_exit();
        D_002C6768 = -1;
        D_002C676C = -1;
        D_002C6760 = -1;
        D_002C6784 = 0;
        break;
    case 0: case 1:
    default:
        cmd_sem_init();
        PowerOffCB();
        D_002C6784 = 1;
        break;
    }
    return r;
}
