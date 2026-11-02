// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder
int kputchar(int c)
{
  while (*(volatile unsigned int *)0x1000F130 & 0x8000) ;
  *(volatile unsigned char *)0x1000F180 = c;
  return c;
}
