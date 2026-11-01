struct E { char pad[0x18]; int *ap; char pad2[0x50]; };
extern struct E *D_003E1398;
int IcopyFromIdProxy(void *p);
int IsrcFromIdProxy__Fi(void *p);
int OidFromIdProxy(void *p)
{
    struct E *pe;
    if (IcopyFromIdProxy(p) == -1) return -1;
    pe = &D_003E1398[IsrcFromIdProxy__Fi(p)];
    return pe->ap[IcopyFromIdProxy(p)];
}
