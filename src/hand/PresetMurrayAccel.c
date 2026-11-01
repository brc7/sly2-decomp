struct S { char pad[0x13D0]; int k; };
void func_001F9B20(struct S *p);
void PresetStepAccel(struct S *p);
void PresetJtAccelJump(void *p);
void PresetJtAccelHide(void *p);
void PresetJtAccelZap(void *p);
void func_001B6CC0(struct S *p);
void PresetJtAccelPipe(void *p);
void PresetMurrayAccel(struct S *p)
{
    char *b = (char *)p;
    if (p->k == 20) func_001F9B20(p);
    else PresetStepAccel(p);
    switch (p->k) {
    case 2: PresetJtAccelJump(b + 0x2C0); break;
    case 13: PresetJtAccelHide(b + 0x4C0); break;
    case 14: PresetJtAccelZap(b + 0xDF0); break;
    case 3: func_001B6CC0(p); break;
    case 20: PresetJtAccelPipe(b + 0xF50); break;
    }
}
