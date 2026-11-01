typedef int qword __attribute__((mode(TI)));
struct G { char pad[0x260]; void *p260; char pad2[0x300 - 0x264]; int n300; };
extern struct G D_002DDF40;
struct V { float x, y, z; } __attribute__((aligned(16)));
void func_0019E9D8(void *p, struct V *v);
qword func_001A62E8(void *p);
void GetStepplayerCmFocusPos(char *p, struct V *v)
{
    float z;
    if (*(int *)(p + 0xF54) != -1) {
        func_0019E9D8(p + 0xF50, v);
        return;
    }
    *(qword *)v = func_001A62E8(p);
    if (*(int *)(p + 0x208)) {
        z = *(float *)(p + 0x238) + *(float *)(p + 0x1048);
    } else if (*(int *)(p + 0x2DC) == 0x14 || D_002DDF40.n300 != 0 || D_002DDF40.p260 != p) {
        v->z += *(float *)(p + 0x248) + *(float *)(p + 0x1048);
        return;
    } else {
        z = *(float *)(p + 0x104C);
    }
    v->z = z;
}
