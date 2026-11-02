// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    void *addr;      /* 0x000 */
    int arg_len;     /* 0x004 */
    char path[0xFC]; /* 0x008 */
    char args[0xFC]; /* 0x104 */
} LoadParam;
typedef struct { int pad[10]; } ClientData;
extern LoadParam D_0044C040;
extern ClientData D_0044C240;
int _lf_bind(void);
int func_00238E70(void);
void *func_0023320C(void *, const void *, unsigned int);
int sceSifCallRpc(ClientData *, int, unsigned int, void *, int, void *, int, void *, void *);
int _sceSifLoadModuleBuffer(void *addr, int args, const char *argp, int *result)
{
    if (_lf_bind() < 0) return -0x10000;
    if (func_00238E70()) return 0xFFFEFFFC;
    D_0044C040.addr = addr;
    if (argp) {
        if (args > 0xFC) {
            __builtin_memcpy(D_0044C040.args, argp, 0xFC);
            D_0044C040.arg_len = 0xFC;
        } else {
            func_0023320C(D_0044C040.args, argp, args);
            D_0044C040.arg_len = args;
        }
    } else {
        D_0044C040.arg_len = 0;
    }
    if (sceSifCallRpc(&D_0044C240, 6, 0, &D_0044C040, 0x200, &D_0044C040, 8, 0, 0) < 0)
        return 0xFFFEFFFF;
    *result = D_0044C040.arg_len;
    return (int)D_0044C040.addr;
}

