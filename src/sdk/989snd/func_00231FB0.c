// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern int D_002C4648, D_002C4688, D_002C46B8;
extern char D_002AA250[], D_002AA280[], D_002AA1C8[], D_002AA2A0[];
extern unsigned int D_00448480[];
extern int D_004484C0[];
extern char D_00448440[];
int func_00232DA8(int);
void func_0021EC18(const char *, ...);
void func_00234F48(void *, void *);
void func_00231C48(void);
void func_00234CA0(int);
int func_00237488(void *);
int func_00237288(void *, int, int, void *, int, void *, int, void *, void *);

int func_00231FB0(int a, int b)
{
    D_002C4648 = 0;
    if (D_002C4688) {
        if (!D_002C46B8)
            func_0021EC18(D_002AA250);
        return 0;
    }
    if (func_00232DA8(1) == 1) {
        if (!D_002C46B8)
            func_0021EC18(D_002AA280);
        return 0;
    }
    D_00448480[0] = 0xFFFFFFFF;
    func_00234F48(D_00448480, D_00448480 + 1);
    D_004484C0[0] = a;
    D_004484C0[1] = b;
    while (func_00237488(D_00448440)) {
        if (!D_002C46B8)
            func_0021EC18(D_002AA1C8);
        func_00231C48();
        func_00234CA0(0);
    }
    if (func_00237288(D_00448440, 3, 1, D_004484C0, 8, D_00448480, 4, 0, 0) < 0) {
        if (!D_002C46B8)
            func_0021EC18(D_002AA2A0);
        D_002C4648 = 0x106;
        return 0;
    }
    while (D_00448480[0] == 0xFFFFFFFF)
        func_00234CA0(0);
    return D_00448480[0];
}
