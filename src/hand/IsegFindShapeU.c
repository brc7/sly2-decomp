struct X { char pad[0xE8]; int c; float *ag; char pad2[0x178 - 0xF0]; int i; };
float UWrapShape(struct X *p, float g);
int IsegFindShapeU(struct X *p, float *pdg, float *pdgSeg, float g)
{
    int i;
    g = UWrapShape(p, g);
    i = p->i;
    if (p->ag[i] < g) { int n = p->c - 1; while (i < n) { if (g < p->ag[i]) break; i++; } i--; } else { while (i > 0) { if (p->ag[i] <= g) break; i--; } } { float g0 = p->ag[i]; if (pdg) *pdg = g - g0; if (pdgSeg) *pdgSeg = p->ag[i + 1] - g0; }
    p->i = i;
    return i;
}
