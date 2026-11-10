// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void sceSifExitCmd(void);
extern int _rpc_inited;
void sceSifExitRpc(void)
{
    sceSifExitCmd();
    _rpc_inited = 0;
}
