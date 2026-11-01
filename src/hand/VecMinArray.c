typedef int qword __attribute__((mode(TI)));
extern qword D_002A2DF0;

qword VecMinArray(int c, qword *p)
{
    qword m = D_002A2DF0;
    int i;
    for (i = 0; i < c; i++) {
        qword v = p[i];
        __asm__("vmini.xyz %0, %1, %2" : "=j"(m) : "j"(v), "0"(m));
    }
    return m;
}
