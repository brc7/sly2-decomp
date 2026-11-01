struct S { char pad[0x2AC]; unsigned int c; int a[8]; };
void AddButtonPushObject(struct S *p, int x)
{
    if (p->c < 8) {
        p->a[p->c++] = x;
    }
}
