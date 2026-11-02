// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
// SLY2_ASM: gnu-reorder
typedef struct { void *func; void *data; void *gp; } SifCmdHandlerData;
typedef struct {
    int x0, x4, x8;
    SifCmdHandlerData *sys_cmd_buffer;
    int sys_size;
    SifCmdHandlerData *usr_cmd_buffer;
    int usr_size;
} SifCmdData;
extern SifCmdData D_0044EDD8;

static inline void *GetGp(void)
{
    void *gp;
    __asm__ volatile("move %0, $gp" : "=r"(gp) : : "memory");
    return gp;
}

/** @sly2-annot addr=0x23ADA8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/sdk-libraries.md#sdk_libs-0x233000-0x254f00
 * @end-sly2-annot */
void sceSifAddCmdHandler(int cid, void *handler, void *data)
{
    if (cid < 0) {
        D_0044EDD8.sys_cmd_buffer[cid & 0x7FFFFFFF].func = handler;
        D_0044EDD8.sys_cmd_buffer[cid & 0x7FFFFFFF].data = data;
        D_0044EDD8.sys_cmd_buffer[cid & 0x7FFFFFFF].gp = GetGp();
    } else {
        D_0044EDD8.usr_cmd_buffer[cid].func = handler;
        D_0044EDD8.usr_cmd_buffer[cid].data = data;
        D_0044EDD8.usr_cmd_buffer[cid].gp = GetGp();
    }
}
