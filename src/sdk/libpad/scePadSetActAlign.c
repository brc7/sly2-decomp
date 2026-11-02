// (Claude agent) 2 diffs: pad result pattern r = p->result; if (r == 1) { f(); r = p->result; }. Left: loop body sb/slti order (ours sb first) in every mode/compiler tried.
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct { int cmd; int port; int slot; unsigned char data[8]; int result; char pad[0x80 - 0x18]; } PadBuf;
extern PadBuf D_004507C0;
extern char D_00450580[];
int func_00237288(void *cd, int fno, unsigned int mode, void *send, int ssize, void *recv, int rsize, void *end, void *earg);
int func_00243458(int port, int slot, int state);
int scePadSetActAlign(int port, int slot, const unsigned char *data)
{
    int i;
    int r;
    PadBuf *p = &D_004507C0;
    D_004507C0.cmd = 8;
    p->port = port;
    p->slot = slot;
    for (i = 0; i < 6; i++)
        p->data[i] = data[i];
    if (func_00237288(D_00450580, 1, 0, p, 0x80, p, 0x80, 0, 0) < 0)
        return 0;
    r = p->result;
    if (r == 1) {
        func_00243458(port, slot, 2);
        r = p->result;
    }
    return r;
}
