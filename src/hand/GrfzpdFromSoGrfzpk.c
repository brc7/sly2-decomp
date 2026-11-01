struct S { char pad[0x110]; unsigned char ab[26]; };
int GrfzpdFromSoGrfzpk(struct S *p, int f)
{
    int r = 0; int i; int m;
    for (i = 0, m = 1; i < 26; i++) { if (m & f) r |= p->ab[i]; m <<= 1; }
    return r;
}
