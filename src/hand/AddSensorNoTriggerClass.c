struct S { char pad[0x170]; unsigned int c; int a[7]; };
void AddSensorNoTriggerClass(struct S *p, int n) {
    if (p->c < 7) {
        p->a[p->c++] = n;
    }
}
