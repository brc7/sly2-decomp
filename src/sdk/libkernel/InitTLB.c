// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int GetMemorySize(void);
void InitTLB32MB(void);
void _InitTLB(void);
void InitTLB(void)
{
    if (GetMemorySize() == 0x2000000) InitTLB32MB();
    else _InitTLB();
}
