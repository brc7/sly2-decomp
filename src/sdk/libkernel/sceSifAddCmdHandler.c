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

// libkernel: registers handler and data for SIF command cid, with the caller's gp. System commands (cid with the
// top bit set) go into the system command table (index cid & 0x7FFFFFFF), the others into the user table.
// (This comment keeps the five lines of the draft block it replaced: the file uses SLY2_ASM gnu-reorder and inline
// asm, which cannot be built in every environment, so its line layout is left exactly as it was matched.)
//
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
