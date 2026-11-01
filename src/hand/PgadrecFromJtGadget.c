void *PgadrecFromJtGadget(char *p, int n)
{
    switch (n) {
    case 0x1A: return p + 0x1FB0;
    case 0x25: return p + 0x1FBC;
    case 0x26: return p + 0x1FC8;
    case 0x29: return p + 0x1FD4;
    }
    return 0;
}
