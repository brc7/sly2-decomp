/* _rpc_get_packet (Sony libkernel sifrpc). */
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef struct {
    int pad[4];
    unsigned int rec_id;  /* 0x10 */
    void *pkt_addr;       /* 0x14 */
    int pid;              /* 0x18 */
    int pad2[9];
} Pkt;
typedef struct {
    int pid;      /* 0x0 */
    Pkt *table;   /* 0x4 */
    int len;      /* 0x8 */
} RpcData;
void func_00239450(void);
int func_002394A0(void);
void *func_00236C28(RpcData *rd)
{
    Pkt *p;
    int i, len;
    func_00239450();
    len = rd->len;
    p = rd->table;
    for (i = 0; i < len; i++, p++) {
        if (!(p->rec_id & 1)) {
            int pid;
            p->rec_id = (i << 16) | 5;
            if (++rd->pid == 1) {
                rd->pid++;
                pid = 1;
            } else {
                pid = rd->pid;
            }
            p->pkt_addr = p;
            p->pid = pid;
            func_002394A0();
            return p;
        }
    }
    func_002394A0();
    return 0;
}
