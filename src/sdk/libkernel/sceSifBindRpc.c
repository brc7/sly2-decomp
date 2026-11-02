// (Claude agent, -Os sweep) MATCH: needed -fstrict-aliasing
// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder-db
typedef struct {
    int pad[5];
    void *pkt_addr;   /* 0x14 */
    int rec_id;       /* 0x18 */
    void *cd;         /* 0x1C */
    unsigned int sid; /* 0x20 */
} BindPkt;
typedef struct {
    void *pkt;   /* 0x00 */
    int id;      /* 0x04 */
    int sema;    /* 0x08 */
    int mode;    /* 0x0C */
    void *buff;  /* 0x10 */
    int x14, x18, x1c, x20;
    void *serve; /* 0x24 */
} ClientData;
struct SemaParam {
    int currentCount;
    int maxCount;
    int initCount;
    int numWaitThreads;
    unsigned int attr;
    unsigned int option;
};
extern char D_0044A600[];
extern char D_002AA9E8[];
void *func_00236C28(void *);
void func_00236CD0(void *);
int func_00234A20(struct SemaParam *);
int func_0023AFA8(unsigned int, void *, int, void *, void *, int);
int func_00234A30(int);
int func_00234A60(int);
int sceSifBindRpc(ClientData *bd, unsigned int request, unsigned int mode)
{
    BindPkt *bind;
    struct SemaParam sema;

    bd->buff = 0;
    bd->serve = 0;
    bind = (BindPkt *)func_00236C28(D_0044A600);
    if (bind == 0) return -1;
    bd->pkt = bind;
    bd->id = bind->rec_id;
    bind->sid = request;
    bind->pkt_addr = bind;
    bind->cd = bd;

    if (!(mode & 1)) {
        sema.maxCount = 1;
        sema.option = (unsigned int)D_002AA9E8;
        sema.initCount = 0;
        bd->sema = func_00234A20(&sema);
        if (bd->sema < 0) {
            func_00236CD0(bind);
            return -3;
        }
        if (func_0023AFA8(0x80000009, bind, 0x40, 0, 0, 0) == 0) {
            func_00236CD0(bind);
            func_00234A30(bd->sema);
            return -2;
        }
        func_00234A60(bd->sema);
        func_00234A30(bd->sema);
    } else {
        bd->sema = -1;
        if (func_0023AFA8(0x80000009, bind, 0x40, 0, 0, 0) == 0) {
            func_00236CD0(bind);
            return -2;
        }
    }
    return 0;
}
