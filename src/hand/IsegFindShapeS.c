struct X { char pad[0xE8]; int c; float *agEC; float *ag; char pad2[0x178 - 0xF4]; int i; };
float SWrapShape(struct X *p, float g);

static inline int Before(int a, int b)
{
    register int result __asm__("$2");
    __asm__("slt %0,%1,%2" : "=r"(result) : "r"(a), "r"(b));
    return result;
}

static inline int LoopEnd(int value)
{
    register int result __asm__("$5");
    __asm__("daddu %0,%1,$zero" : "=r"(result) : "r"(value));
    return result;
}

static inline int Count(struct X *p)
{
    register int value __asm__("$3");
    __asm__("lw %0,%1" : "=r"(value) : "m"(p->c));
    return value;
}

int IsegFindShapeS(struct X *p, float *pdg, float *pdgSeg, float g)
{
    int i;
    g = SWrapShape(p, g);
    i = p->i;
    if (p->ag[i] < g) {
        int limit = Count(p) - 1;
        if (!Before(i, limit)) goto forwardDone;
        if (g < p->ag[i]) goto forwardDone;
        {
            int end = LoopEnd(limit);
            do {
                ++i;
                if (i >= end) break;
                if (g < p->ag[i]) break;
            } while (1);
        }
forwardDone:
        --i;
    } else {
        for (; i > 0; i--)
            if (p->ag[i] <= g) break;
    }
    if (pdg) *pdg = g - p->ag[i];
    if (pdgSeg) *pdgSeg = p->ag[i + 1] - p->ag[i];
    p->i = i;
    return i;
}
