struct E { int a, b, c; };
struct E *PdlFromSwCid(char *p, int i)
{
    return &((struct E *)(p + 0x3038))[i];
}
