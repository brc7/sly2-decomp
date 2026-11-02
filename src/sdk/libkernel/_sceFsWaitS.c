// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
void _sceFsSemInit(void);
int WaitSema(int);
extern int _sceFs_sema;
int _sceFsWaitS(void)
{
    _sceFsSemInit();
    WaitSema(_sceFs_sema);
    return 0;
}
