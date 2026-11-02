// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
typedef char *va_list;
#define va_start(ap, last) (ap = ((char *)__builtin_next_arg(last) - (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8)))
#define va_end(ap)
int snprintf_char(char **pp, int *left, int c);
int func_00235B08(int (*put)(char **, int *, int), char **ctx, unsigned int size, const char *fmt, va_list ap);
int sceSnprintf(char *buf, unsigned int size, const char *fmt, ...)
{
    va_list ap;
    char *p = buf;
    int r;
    va_start(ap, fmt);
    r = func_00235B08(snprintf_char, &p, size, fmt, ap);
    va_end(ap);
    return r;
}
