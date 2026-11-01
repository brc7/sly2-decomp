struct S { char pad[0x4c0]; int f4c0; char pad2[0x13d0 - 0x4c4]; int st; };
int func_00194640(void *, int);
int STEPPLAYER_FInvulnerable(struct S *, int);
int BENTLEY_FInvulnerable(struct S *p, int msg) {
    switch (p->st) { case 13: if (msg == 0x4000) return 1; break; case 3: if (func_00194640(&p->f4c0, msg)) return 1; break; }
    return STEPPLAYER_FInvulnerable(p, msg);
}
