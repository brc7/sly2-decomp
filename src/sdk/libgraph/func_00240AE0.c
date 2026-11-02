// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef unsigned long u_long;
typedef struct {
    u_long SCAX0:11, :5, SCAX1:11, :5, SCAY0:11, :5, SCAY1:11, :5;
} SCISSOR;
typedef struct {
    char pad[0x20];
    u_long xyoffset;
    char pad2[8];
    SCISSOR scissor;
} ENV;
void func_00240AE0(ENV *env, short x, short y, short half)
{
    long oy = (y - ((u_long)(env->scissor.SCAY1 + 1) >> 1)) << 4;
    long ox = (x - ((u_long)(env->scissor.SCAX1 + 1) >> 1)) << 4;
    if (half)
        env->xyoffset = ox | ((oy + 8) << 32);
    else
        env->xyoffset = ox | (oy << 32);
}