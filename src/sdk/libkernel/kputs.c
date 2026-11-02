// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0
int Deci2Call(int, void *);
void kputs(char *s)
{
    char *a[1];
    a[0] = s;
    Deci2Call(0x10, a);
}
