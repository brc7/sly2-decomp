struct S { char pad[0xE4]; int x; };
float SMinShape(struct S *p);
float SMaxShape(struct S *p);
int func_00182FE8(struct S *p, float *pg)
{
    if (p->x) {
        pg[0] = SMinShape(p);
        pg[1] = SMaxShape(p);
        return 1;
    }
    return 0;
}
