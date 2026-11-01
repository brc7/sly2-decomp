struct MSG { int kind; int pad[3]; int val; char pad2[0x70 - 0x14]; };
void *memset(void *p, int c, unsigned int n);
void *func_00205458(void *p, int n);
void SetBhvguardMode(void *p, struct MSG *m);
void func_0014B468(void *p, int val)
{
    struct MSG m;
    void *o;
    memset(&m, 0, sizeof(m));
    o = func_00205458(p, 0x9C);
    if (val) {
        m.val = val;
        m.kind = 0xE;
        SetBhvguardMode(o, &m);
    } else if (func_00205458(p, 0xB6)) {
        m.kind = 1;
        SetBhvguardMode(o, &m);
    }
}
