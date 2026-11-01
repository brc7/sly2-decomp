struct E { int a, b, c; };
struct E *PdlFromSwOid(char *p, int i)
{
    return &((struct E *)(p + 0x38))[(i * 0x95675) & 0x1FF];
}
