struct S { char pad[0xE4]; int ne4; };
float SMinShape(struct S *p);
float SMaxShape(struct S *p);
int func_001C3BB0(struct S *p, float *pg)
{
    /*B*/if (p->ne4) { pg[0] = SMinShape(p); pg[1] = SMaxShape(p); return 1; } return 0;/*E*/
}
