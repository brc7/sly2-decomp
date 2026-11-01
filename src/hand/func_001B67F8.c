struct S { char pad[0x13D0]; int k; };
void func_001F9B20(struct S *p);
void PresetStepAccel(struct S *p);
void func_00196DF8(void *p);
void func_00194170(void *p);
void func_001A1148(void *p);
void func_001B6CC0(struct S *p);
void func_0019DF70(void *p);
void func_001B67F8(struct S *p)
{
    char *b = (char *)p;
    if (p->k == 20) func_001F9B20(p);
    else PresetStepAccel(p);
    switch (p->k) {
    case 2: func_00196DF8(b + 0x2C0); break;
    case 13: func_00194170(b + 0x4C0); break;
    case 14: func_001A1148(b + 0xDF0); break;
    case 3: func_001B6CC0(p); break;
    case 20: func_0019DF70(b + 0xF50); break;
    }
}
