// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder-db
typedef struct {
    void *pkt;      /* 0x00 */
    int x4;
    int sema;       /* 0x08 */
    int xc, x10;
    int buff;     /* 0x14 */
    void *gp;       /* 0x18 */
    void (*end)(void *); /* 0x1C */
    void *earg;     /* 0x20 */
    int server;   /* 0x24 */
} Client;
typedef struct {
    int pad[7];
    Client *client; /* 0x1C */
    unsigned int cid; /* 0x20 */
    int server;   /* 0x24 */
    int buff;     /* 0x28 */
} Rend;
void iSignalSema(int);
void func_00236CD0(void *);
static inline void *SetGP(void *gp)
{
    void *old;
    __asm__ volatile("move %0, $gp\n\tmove $gp, %1" : "=&r"(old) : "r"(gp) : "memory");
    return old;
}
void _request_end(Rend *r)
{
    Client *c;
    switch (r->cid) {
    case 0x8000000A:
        c = r->client;
        if (c->end) {
            void *gp = SetGP(c->gp);
            c->end(c->earg);
            SetGP(gp);
            c = r->client;
        }
        break;
    case 0x80000009:
        c = r->client;
        c->server = r->server;
        c->buff = r->buff;
        break;
    case 0x8000000B:
    default:
        c = r->client;
        break;
    }
    if (c->sema >= 0)
        iSignalSema(c->sema);
    func_00236CD0(c->pkt);
    c->pkt = 0;
}
