// SLY2_CC: tools/compilers/bundle/ee-gcc2.95.3-136
// SLY2_CFLAGS: -O2 -G0 -ffast-math -mvu0-use-vf0-vf31
void *func_002333B4(void *, int, int); void func_00230A98(void *, void *, float, float *, float *);
void LoadIdentityMatrix4(void *a, void *b, float *m, float *n, float f)
{
    if (m) {
        func_002333B4(m, 0, 0x40);
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    if (n) func_002333B4(n, 0, 0x40);
    func_00230A98(a, b, f, m, n);
}
