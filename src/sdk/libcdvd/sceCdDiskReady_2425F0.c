// (Claude agent) libcdvd sceCdDiskReady (0x2425F0): both delay loops share one function-scope counter `i` (same reg in both, like the target).
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int pad[9]; void *serve; int pad2[6]; } sceSifClientData;
extern int D_002C6750;
extern int D_002C6780;
extern volatile int D_002C6770;
extern int D_002C6790;
extern sceSifClientData D_00450510;
extern int D_00450550[];
extern int D_002C81C0[];
int printf(const char *fmt, ...);
int func_00239450(void);
int func_002394A0(void);
void func_00241A30(void);
int func_00234A70(int);
void func_00236A60(int);
int func_002370A8(sceSifClientData *, unsigned int, int);
void func_0023B170(void *, int);
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int func_00234A40(int);
int sceCdDiskReady(int mode);

int sceCdDiskReady_2425F0(int mode)
{
    int r;
    int cnt;
    int i;
    if (D_002C6750 > 0) printf("NEW DiskReady Call\n");
    func_00241A30();
    if (D_002C6770 != func_00234A70(D_002C6770)) {
        if (mode == 8) return -1;
        return 6;
    }
    func_00236A60(0);
    cnt = 0;
    if (D_002C6790 < 0) {
        while (1) {
            if (func_002370A8(&D_00450510, 0x8000059C, 0) < 0) {
                if (D_002C6750 > 0) printf("Libcdvd bind err CdDiskReady\n");
                i = 0x100000; while (i--) ;
                continue;
            }
            if (D_00450510.serve != 0) { D_002C6790 = 0; break; }
            if (cnt > 0x10) {
                func_00234A40(D_002C6770);
                return sceCdDiskReady(mode);
            }
            i = 0x100000; while (i--) ;
            cnt++;
        }
    }
    r = func_00239450();
    D_002C6780 = 0;
    if (r) func_002394A0();
    D_00450550[0] = mode;
    func_0023B170(D_00450550, 4);
    if (func_00237288(&D_00450510, 0, 0, D_00450550, 4, D_002C81C0, 4, 0, 0) < 0) {
        func_00234A40(D_002C6770);
        if (mode == 8) return -1;
        return 6;
    }
    if (D_002C6750 > 0) printf("DiskReady ended\n");
    r = *(int *)((unsigned int)D_002C81C0 | 0x20000000);
    func_00234A40(D_002C6770);
    return r;
}
