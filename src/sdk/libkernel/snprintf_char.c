// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -fstrict-aliasing
int snprintf_char(char **pp, int *left, int c)
{
    char *p;
    if (*left == 0) return 1;
    if (c >= 256) { **pp = 0; return 0; }
    if (*left == 1) c = 0;
    p = *pp;
    *p++ = c;
    *pp = p;
    (*left)--;
    return 1;
}
