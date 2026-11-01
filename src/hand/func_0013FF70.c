struct P {
    char pad0[0x1020]; float x1020; char pad1[0x103C - 0x1024]; float x103C; char pad2[0x1048 - 0x1040]; float x1048;
    char pad3[0x10C8 - 0x104C]; int x10C8; char pad4[0x10D4 - 0x10CC]; float x10D4; float x10D8; char pad5[0x10E4 - 0x10DC];
    int x10E4; int x10E8; int x10EC; int x10F0; char pad6[0x1138 - 0x10F4]; float x1138; float x113C; float x1140; float x1144;
    char pad7[0x12F0 - 0x1148]; int x12F0; char pad8[0x1300 - 0x12F4]; int x1300; float x1304; char pad9[0x1318 - 0x1308]; int x1318;
};
void InitStep(struct P *p);
void func_00195950(void *pv, struct P *p);
void func_00191090(void *pv, struct P *p);
void func_0019F3E8(void *pv, struct P *p);
void func_001A0B18(void *pv, struct P *p);
void func_0019CBF8(void *pv, struct P *p);
void func_0021F2E0(void *pv, struct P *p);
void func_0019DE10(void *pv, struct P *p);
void func_00144638(void *pv, struct P *p);
void func_001F7918(struct P *p, int mask, int n);
void func_0013FF70(struct P *p)
{
    InitStep(p);
    func_00195950((char *)p + 0x2C0, p);
    func_00191090((char *)p + 0x4C0, p);
    func_0019F3E8((char *)p + 0x9E0, p);
    func_001A0B18((char *)p + 0xDF0, p);
    func_0019CBF8((char *)p + 0xF00, p);
    func_0021F2E0((char *)p + 0x11B0, p);
    func_0019DE10((char *)p + 0xF50, p);
    func_00144638((char *)p + 0xFD0, p);


    p->x103C = 20.0f;
    p->x10F0 = 1;
    p->x1048 = 125.0f;
    p->x10D4 = p->x10D8 = 1.0f;
    p->x1138 = 1.5f;
    p->x113C = 2.5f;
    p->x1140 = 3.0f;
    p->x1144 = 5.0f;
    p->x12F0 = p->x10F0;
    p->x1020 = 100.0f;
    p->x1300 = 100;
    p->x1304 = -60.0f;
    p->x10EC = -1;
    p->x10E4 = -1;
    p->x1318 = -1;
    p->x10C8 = -1;

    func_001F7918(p, 0x3FFFFFF, 0);
    func_001F7918(p, 0x10FF00, 8);
}
