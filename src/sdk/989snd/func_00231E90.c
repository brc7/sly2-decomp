// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
extern unsigned int *D_002C4640; extern int D_002C4644; extern int D_002C46B8; extern char D_00443800[]; extern char D_002AA140[];
void func_00234CA0(int); int func_00237488(void *); void func_0021EC18(void *);
int func_00231E90(void)
{
    func_00234CA0(0);
    if (D_002C4640 == 0) return 1;
    if (func_00237488(D_00443800)) return 0;
    if (D_002C4640[0] == 0xFFFFFFFF && D_002C4640[D_002C4644 + 1] == D_002C4640[0]) {
        D_002C4640 = 0;
        return 1;
    }
    if (D_002C46B8 == 0) func_0021EC18(D_002AA140);
    return 0;
}
