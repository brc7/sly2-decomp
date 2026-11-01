unsigned long GrfFromSgf(int n);
unsigned long GrfFromSgfArray(int c, int *a) { unsigned long m = 0; int i; for (i = 0; i < c; i++) m |= GrfFromSgf(a[i]); return m; }
