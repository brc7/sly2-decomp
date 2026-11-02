// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int func_00239450(void);
int func_002394A0(void);
#define REG(a) (*(volatile unsigned int *)(a))
void _sceMpegStopIpuDma(void)
{
  int s = func_00239450();
  REG(0x1000F590) = REG(0x1000F520) | 0x10000;
  REG(0x1000B000) = 0;
  REG(0x1000B400) = 0;
  REG(0x1000F590) = REG(0x1000F520) & ~0x10000;
  if (s) func_002394A0();
  REG(0x1000B020) = 0;
  REG(0x1000B420) = 0;
  REG(0x10002010) = 0x40000000;
}
