typedef struct X { char pad[0xC]; int nC; int n10; int n14; } X;

void ResetSlotheap(X *px)
{
    px->n14 = 0;
    px->n10 = 0;
    px->nC = 0;
}
