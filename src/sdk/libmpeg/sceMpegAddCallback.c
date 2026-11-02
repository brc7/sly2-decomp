// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
typedef struct { void *func; void *data; int gp; } CB;
typedef struct { char pad[0xC]; CB cb[1]; } SYS;
typedef struct { char pad[0x40]; SYS *sys; } sceMpeg;
void *sceMpegAddCallback(sceMpeg *mp, int type, void *func, void *data)
{
  SYS *p = mp->sys;
  void *old = p->cb[type].func;
  int gp;
  p->cb[type].func = func;
  p->cb[type].data = data;
  __asm__ volatile("move %0, $gp" : "=r"(gp));
  p->cb[type].gp = gp;
  return old;
}
