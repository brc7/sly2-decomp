struct S { char pad[0x150]; unsigned int c; int a[7]; };
void AddSensorNoTriggerObject(struct S *p, int n) {
    if (p->c < 7) {
        p->a[p->c++] = n;
    }
}
