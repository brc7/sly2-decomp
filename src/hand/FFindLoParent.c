struct O { char pad[0x10]; struct O *x10; };
int FFindLoParent(struct O *a, struct O *b)
{
    while (a) {
        if (a == b) break;
        a = a->x10;
    }
    return a != 0;
}
