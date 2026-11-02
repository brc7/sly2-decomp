// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
int _sceSnprintf(void *a, void *b, int n, const char *fmt, va_list ap);
int _sceSnprintf_pf(void *a, void *b, int *pn, const char *fmt, ...)
{
    va_list ap;
    int r;

    va_start(ap, fmt);
    r = _sceSnprintf(a, b, *pn, fmt, ap);
    *pn -= r;
    return r;
}
