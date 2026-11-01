struct S { char pad[0x288]; unsigned int c; int a[8]; };
void AddVolbtnPushObject(struct S *p, int x)
{
    if (p->c < 8) {
        p->a[p->c++] = x;
    }
}
