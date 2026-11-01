extern char D_002DEB30[];
extern void *D_003E0F9C;
void BroadcastGameEvent(void *p, int n, void *q);
void func_00162E38(void *p);
void func_001B0970(void)
{
    BroadcastGameEvent(D_002DEB30, 0x2A, 0);
    func_00162E38(D_003E0F9C);
}
