void func_001F4FA8(void *p);
void func_001991A0(void *p);
void UpdateJtInternalXpsHide(void *p);
void UpdateJtInternalXpsPipe(void *p);
void func_00140D00(char *p)
{
    func_001F4FA8(p);
    if (*(int *)(p + 0x2DC) != -1) func_001991A0(p + 0x2C0);
    if (*(int *)(p + 0x4CC) != -1) UpdateJtInternalXpsHide(p + 0x4C0);
    if (*(int *)(p + 0xF54) != -1) UpdateJtInternalXpsPipe(p + 0xF50);
}
