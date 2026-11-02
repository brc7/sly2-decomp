// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int tolower(int);

int strcasecmp(const char *a, const char *b)
{
    while (*a && tolower(*a) == tolower(*b)) {
        a++;
        b++;
    }
    return tolower(*(unsigned char *)a) - tolower(*(unsigned char *)b);
}
