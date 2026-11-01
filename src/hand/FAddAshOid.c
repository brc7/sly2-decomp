struct P { int a, b; };
struct L { unsigned int c; struct P ap[8]; };
int FAddAshOid(struct L *l, int a, int b)
{
    if (l->c >= 8) return 0;
    l->ap[l->c].a = a;
    l->ap[l->c].b = b;
    l->c++;
    return 1;
}
