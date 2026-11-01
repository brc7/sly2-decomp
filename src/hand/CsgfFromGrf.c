unsigned long GrfFromSgf(int n);
int CsgfFromGrf(unsigned long m, int *a) { int c = 0; int i; for (i = 0x2E; i >= 0; i--) if (m & GrfFromSgf(i)) { *a++ = i; c++; } return c; }
