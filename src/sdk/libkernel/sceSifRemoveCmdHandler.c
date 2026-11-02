// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { void *func; void *data; int gp; } H;
extern H *_sif_sys_handlers; extern H *_sif_usr_handlers;
void sceSifRemoveCmdHandler(int cid)
{
    if (cid < 0) _sif_sys_handlers[cid & 0x7fffffff].func = 0;
    else _sif_usr_handlers[cid].func = 0;
}
