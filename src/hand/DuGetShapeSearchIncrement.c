
struct S { char pad[0xE8]; int c; };
float UMaxShape(struct S *p);
float UMinShape(struct S *p);
float DuGetShapeSearchIncrement(struct S *p)
{
    float a = UMaxShape(p);
    return (a - UMinShape(p)) / (p->c * 10.0f);
}
