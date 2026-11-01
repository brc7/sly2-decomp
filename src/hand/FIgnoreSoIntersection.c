typedef unsigned long ulong;
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct X { char pad[0xC]; struct VT *pvt; char pad2[0x148 - 0x10]; ulong fl; };
#define VF(p) (p)->pvt->a[62].pfn((char *)(p) + (p)->pvt->a[62].d)
int FIgnoreSoIntersection(struct X *pa, struct X *pb)
{
    if (((pa->fl & 0x4000000) || VF(pa)) && VF(pb)) return 1;
    return 0;
}
