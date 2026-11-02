// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned char u_char;
typedef struct {
    u_long testa, testaaddr;
    u_long prim, primaddr;
    u_long rgbaq, rgbaqaddr;
    u_long xyz2a, xyz2aaddr;
    u_long xyz2b, xyz2baddr;
    u_long testb, testbaddr;
} sceGsClear;
#define SET_TEST(ate, atst, aref, afail, date, datm, zte, ztst) \
    ((u_long)(ate) | ((u_long)(atst) << 1) | ((u_long)(aref) << 4) | ((u_long)(afail) << 12) | \
     ((u_long)(date) << 14) | ((u_long)(datm) << 15) | ((u_long)(zte) << 16) | ((u_long)(ztst) << 17))
#define SET_RGBAQ(r, g, b, a, q) \
    ((u_long)(r) | ((u_long)(g) << 8) | ((u_long)(b) << 16) | ((u_long)(a) << 24) | ((u_long)(q) << 32))
#define SET_XYZ(x, y, z) ((u_long)(x) | ((u_long)(y) << 16) | ((u_long)(z) << 32))
static inline u_long Combine(u_long a,u_long b){u_long result;__asm__("or %0,%1,%2":"=r"(result):"r"(a),"r"(b));return result;}
int sceGsSetDefClear(sceGsClear *cp, short ztest, short x, short y, short w, short h,
                     u_char r, u_char g, u_char b, u_char a, u_int z)
{
    u_long zz = z;
    cp->testaaddr = 0x47;
    cp->testa = SET_TEST(0, 0, 0, 0, 0, 0, 1, 1);
    cp->primaddr = 0;
    cp->prim = 6;
    cp->rgbaqaddr = 1;
    cp->rgbaq = Combine((u_long)r,(u_long)0x3f800000 << 32) | Combine((u_long)b << 16,(u_long)g << 8) | ((u_long)a << 24);
    cp->xyz2aaddr = 5;
    cp->xyz2a = SET_XYZ(x << 4, y << 4, zz);
    cp->xyz2baddr = 5;
    cp->xyz2b = SET_XYZ((x + w) << 4, (y + h) << 4, zz);
    cp->testbaddr = 0x47;
    if (ztest)
        cp->testb = SET_TEST(0, 0, 0, 0, 0, 0, 1, ztest & 3);
    else
        cp->testb = SET_TEST(0, 0, 0, 0, 0, 0, 1, 1);
    __asm__ volatile("sync");
    return 6;
}