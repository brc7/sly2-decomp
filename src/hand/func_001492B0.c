void RemoveSoHierarchy(void *p);
void SetButtonButtons(void *p, int n);
void func_001492B0(char *p)
{
    RemoveSoHierarchy(p);
    if ((unsigned int)(*(int *)(p + 0x1E0) - 1) < 2) {
        SetButtonButtons(p, 0);
    }
}
