struct E { int a, b, c; };
struct E *PdlFromSwPxrOid(char *p, int i)
{
    return &((struct E *)(p + 0x1838))[(i * 0x95675) & 0x1FF];
}
