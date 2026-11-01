struct S { char pad[0x184]; int st; int f188; int f18c; };
void HandleBasicMessage(struct S *, int, int);
void SetBootyState(struct S *, int);
void HandleBootyMessage(struct S *p, int msg, int a) {
    HandleBasicMessage(p, msg, a);
    switch (msg) {
    case 4:
        if (p->st == 5) SetBootyState(p, 0);
        break;
    case 8:
        if (a == p->f18c) {
            switch (p->st) {
            case 2: SetBootyState(p, 4); break;
            case 3: SetBootyState(p, 1); break;
            }
        }
        break;
    }
}
