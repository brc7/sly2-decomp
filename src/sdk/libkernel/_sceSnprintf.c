// SLY2_CC: tools/compilers/extra/ee-gcc2.9-991111
// SLY2_CFLAGS: -O2 -G0
// SLY2_ASM: gnu-reorder-db
/* _sceSnprintf (Sony libc printf core: formats fmt/ap through the put callback; called by sceSnprintf and _sceSnprintf_pf).
   Matched by Lilly's agent (T1628) from the T1619 near-miss (lilly8): the loop-test copy `ch` is a `short`, and the
   loop top reads it (`z = (char)ch`, '%' test on z), so the entry keeps `move v0,a1` and the top extends v0.
   Other keys (T1616/T1619): `t = i * 10 - '0'; i = t + (char)c;` digit steps, a `c = *fmt;` reload on every path
   into `next:`, loop end `ch = *fmt; c = ch;`, and the sign-extended '*' test load first in case '.'. */
typedef char *va_list;
#define va_arg(ap, T) (*(T *)((ap += 8) - 8))
typedef int (*PutFunc)(void *, int *, int);
int printfloat(PutFunc put, void *arg, int *w, double d);

int _sceSnprintf(PutFunc put, void *arg, int n, const unsigned char *fmt, va_list ap)
{
    char buf[32];
    char prefix[32];
    char *end;
    char *cp;
    char *start;
    const char *digits;
    int *pn;
    int flags;
    int width;
    int prec;
    int plen;
    int total;
    int size;
    int i;
    int t;
    int pad;
    int c;
    short ch;
    short z;
    long v;
    unsigned long u;
    char *s;
    float f;

    end = &buf[31];
    total = 0;
    c = *fmt;
    while ((ch = c) != 0) {
        z = (char)ch;
        pn = &n;
        flags = 0;
        width = 0;
        prec = 0;
        plen = 0;
        if (z != '%') goto lit;
    again:
        fmt++;
        c = *fmt;
        z = c;
        switch ((char)c) {
        case '-': flags |= 1; goto again;
        case '+': flags |= 2; goto again;
        case '0': flags |= 4; goto again;
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            i = 0;
            do {
                t = i * 10 - '0';
                i = t + (char)c;
                c = *++fmt;
            } while (c >= '0' && c <= '9');
            width = i;
        num:
            if (c == 0) goto next;
            fmt--;
            goto again;
        case '.':
            fmt++;
            flags |= 0x20;
            t = (char)*fmt;
            c = *fmt;
            if ((char)t == '*') {
                i = va_arg(ap, int);
                prec = i < -1 ? -1 : i;
                goto again;
            }
            i = 0;
            while (c >= '0' && c <= '9') {
                t = i * 10 - '0';
                i = t + (char)c;
                c = *++fmt;
            }
            prec = i < -1 ? -1 : i;
            goto num;
        case ' ': flags |= 8; goto again;
        case '#': flags |= 0x10; goto again;
        case 'l': flags |= 0x40; goto again;
        case 'h': flags |= 0x80; goto again;
        case 'o':
            if (flags & 0x40) u = va_arg(ap, unsigned long);
            else if (flags & 0x80) u = va_arg(ap, unsigned short);
            else u = va_arg(ap, unsigned int);
            cp = end;
            if (u == 0) {
                if (!(flags & 0x20) || prec != 0) *cp-- = '0';
            } else {
                do {
                    *cp-- = (u & 7) + '0';
                    u >>= 3;
                } while (u);
                if (flags & 0x10) *cp-- = '0';
            }
            start = cp + 1;
            size = end - cp;
            goto out;
        case 'x':
            digits = "0123456789abcdef";
            if (flags & 0x10) {
                prefix[0] = '0';
                prefix[1] = 'x';
                plen = 2;
            }
            goto hex;
        case 'X':
            digits = "0123456789ABCDEF";
            if (flags & 0x10) {
                prefix[0] = '0';
                prefix[1] = 'X';
                plen = 2;
            }
        hex:
            if (flags & 0x40) u = va_arg(ap, unsigned long);
            else if (flags & 0x80) u = va_arg(ap, unsigned short);
            else u = va_arg(ap, unsigned int);
            cp = end;
            if (u == 0) {
                plen = 0;
                if (!(flags & 0x20) || prec != 0) *cp-- = '0';
            } else {
                do {
                    *cp-- = digits[u & 0xF];
                    u >>= 4;
                } while (u);
            }
            start = cp + 1;
            size = end - cp;
            goto out;
        case 'd':
        case 'i':
            if (flags & 0x40) v = va_arg(ap, long);
            else if (flags & 0x80) v = va_arg(ap, short);
            else v = va_arg(ap, int);
            cp = end;
            if (v >= 0) {
                if (flags & 2) {
                    prefix[0] = '+';
                    plen = 1;
                } else if (flags & 8) {
                    prefix[0] = ' ';
                    plen = 1;
                }
            } else {
                prefix[0] = '-';
                plen = 1;
            }
            if (v == 0) {
                if (!(flags & 0x20) || prec != 0) *cp-- = '0';
            } else {
                if (v < 0) v = -v;
                while (v) {
                    *cp-- = v % 10 + '0';
                    v /= 10;
                }
            }
            start = cp + 1;
            size = end - cp;
            goto out;
        case 'u':
            if (flags & 0x40) u = va_arg(ap, unsigned long);
            else if (flags & 0x80) u = va_arg(ap, unsigned short);
            else u = va_arg(ap, unsigned int);
            cp = end;
            if (u == 0) {
                if (!(flags & 0x20) || prec != 0) *cp-- = '0';
            } else {
                do {
                    *cp-- = u % 10 + '0';
                    u /= 10;
                } while (u);
            }
            start = cp + 1;
            size = end - cp;
            goto out;
        case 'e':
        case 'f':
            f = va_arg(ap, double);
            if (f == 0) {
                total += put(arg, pn, '0');
                c = *fmt;
            } else {
                total += printfloat(put, arg, pn, f);
                c = *fmt;
            }
            goto next;
        case 's':
            s = va_arg(ap, char *);
            if (s == 0) {
                put(arg, pn, '(');
                put(arg, pn, 'n');
                put(arg, pn, 'u');
                put(arg, pn, 'l');
                put(arg, pn, 'l');
                put(arg, pn, ')');
                c = *fmt;
                goto next;
            }
            cp = s;
            if (flags & 0x20) {
                for (i = 0; i < prec && *cp++; i++)
                    ;
                size = i;
            } else {
                for (size = 0; *cp; cp++)
                    size++;
            }
            start = s;
            goto out;
        case 'c':
            cp = end;
            v = va_arg(ap, char);
            flags |= 0x100;
            prec = 1;
            *cp-- = v;
            start = cp + 1;
            size = end - cp;
            goto out;
        case 'p':
            u = va_arg(ap, unsigned int);
            digits = "0123456789abcdef";
            cp = end;
            if (u == 0) {
                if (!(flags & 0x20) || prec != 0) *cp-- = '0';
            } else {
                do {
                    *cp-- = digits[u & 0xF];
                    u >>= 4;
                } while (u);
            }
            prefix[0] = '0';
            prefix[1] = 'x';
            plen = 2;
            start = cp + 1;
            size = end - cp;
            goto out;
        default:
            if ((char)z == 0) goto next;
            cp = end;
            flags |= 0x100;
            prec = 1;
            *cp-- = (char)c;
            break;
        }
        size = end - cp;
        start = cp + 1;
    out:
        if (size < prec) prec -= size;
        else prec = 0;
        width = width - size - plen - prec;
        cp = start + size;
        if (flags & 0x100)
            pad = (flags & 4) && width > 0 ? '0' : ' ';
        else
            pad = (flags & 0x24) == 4 && width > 0 ? '0' : ' ';
        if (flags & 1) {
            for (i = 0; i < plen; i++) total += put(arg, pn, prefix[i]);
            for (i = 0; i < prec; i++) total += put(arg, pn, '0');
            while (start < cp) total += put(arg, pn, *start++);
            for (i = 0; i < width; i++) total += put(arg, pn, ' ');
            c = *fmt;
        } else if (flags & 4) {
            if (pad == '0') {
                for (i = 0; i < plen; i++) total += put(arg, pn, prefix[i]);
                for (i = 0; i < width; i++) total += put(arg, pn, pad);
            } else {
                for (i = 0; i < width; i++) total += put(arg, pn, pad);
                for (i = 0; i < plen; i++) total += put(arg, pn, prefix[i]);
            }
            for (i = 0; i < prec; i++) total += put(arg, pn, '0');
            while (start < cp) total += put(arg, pn, *start++);
            c = *fmt;
        } else {
            for (i = 0; i < width; i++) total += put(arg, pn, pad);
            for (i = 0; i < plen; i++) total += put(arg, pn, prefix[i]);
            for (i = 0; i < prec; i++) total += put(arg, pn, '0');
            while (start < cp) total += put(arg, pn, *start++);
            c = *fmt;
        }
        goto next;
    lit:
        if (z == 0) goto next;
        total += put(arg, pn, z);
        c = *fmt;
    next:
        if (c) fmt++;
        ch = *fmt;
        c = ch;
    }
    put(arg, &n, 0);
    return total;
}
