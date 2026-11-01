struct G { int a[4]; };
extern struct G D_003DD4A0;
void GetLoChildDl(char *p, void **pa, void **pb)
{
    if (p) { *pa = p + 0x64; *pb = p + 0x70; }
    else { *pa = &D_003DD4A0; *pb = (char *)&D_003DD4A0 + 0xC; }
}
