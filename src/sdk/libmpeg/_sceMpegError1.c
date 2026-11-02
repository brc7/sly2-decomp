// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
__asm__(".macro sq r, m\n\tsd \\r, \\m\n\t.endm\n\t.macro lq r, m\n\tld \\r, \\m\n\t.endm");
// (Claude agent) near miss: 3/16, target sets a1 before a0 for the vsnprintf call
typedef char *va_list;
int sceVsnprintf(char *, unsigned int, const char *, va_list); void _sceMpegError(void *, const char *);
void _sceMpegError1(void *mp, const char *fmt, va_list ap) { char buf[256]; char *p = buf; sceVsnprintf(p, 256, fmt, ap); _sceMpegError(mp, p); }
