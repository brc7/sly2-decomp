struct X { char pad[0x64]; void *f64; void *f68; };
struct G { char pad[0x3B0C]; struct X *f3b0c; };
extern struct G D_003DD490;
int CploFindSwObjectsByClass(struct G *pg, int kind, int n, int m, int c, void **apv);
int FTaskComplete(void *p);
void EndCurrentJob(struct X *p);
void SetCurrentJob(struct X *p);
void UpdateCurrentJob(void)
{
    struct X *apx[256];
    struct X *pxFound = 0;
    int c = CploFindSwObjectsByClass(&D_003DD490, 5, 0x84, 0, 0x100, (void **)apx);
    int i;
    for (i = 0; i < c; i++) {
        struct X *px = apx[i];
        if (FTaskComplete(px->f64) && !FTaskComplete(px->f68))
            pxFound = px;
    }
    if (pxFound != D_003DD490.f3b0c) {
        if (D_003DD490.f3b0c)
            EndCurrentJob(D_003DD490.f3b0c);
        if (pxFound)
            SetCurrentJob(pxFound);
    }
}
