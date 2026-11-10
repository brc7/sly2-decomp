// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int sceSifGetReg(int);
void sceSifExecNotifyHandler(int, int);   /* 0x23A920 (the ELF's jal target; was spelled sceSifSetReg) */
void func_0023A9C8(void);
int sceSifSyncIop(void)
{
    if (!(sceSifGetReg(4) & 0x40000)) return 0;
    func_0023A9C8();
    sceSifExecNotifyHandler(1, 1);
    sceSifExecNotifyHandler(0, 1);
    return 1;
}
