struct P {
    char pad0[0x1020]; float x1020; char pad1[0x103C - 0x1024]; float x103C; char pad2[0x1048 - 0x1040]; float x1048;
    char pad3[0x10C8 - 0x104C]; int x10C8; char pad4[0x10D4 - 0x10CC]; float x10D4; float x10D8; char pad5[0x10E4 - 0x10DC];
    int x10E4; int x10E8; int x10EC; int x10F0; char pad6[0x1138 - 0x10F4]; float x1138; float x113C; float x1140; float x1144;
    char pad7[0x12F0 - 0x1148]; int x12F0; char pad8[0x1300 - 0x12F4]; int x1300; float x1304; char pad9[0x1318 - 0x1308]; int x1318;
};
void InitStep(struct P *p);
void InitJtAir(void *pv, struct P *p);
void InitJtGrab(void *pv, struct P *p);
void InitJtPlaces(void *pv, struct P *p);
void InitJtHurt(void *pv, struct P *p);
void InitJtMode(void *pv, struct P *p);
void InitThrow(void *pv, struct P *p);
void InitJtPipe(void *pv, struct P *p);
void InitJtgoal(void *pv, struct P *p);
void func_001F7918(struct P *p, int mask, int n);
/** @sly2-annot addr=0x13FF70
 * Purpose: STEPPLAYER vt7 Init: STEP Init (0x203948) then inits embedded components at
 *   +0x2C0,+0x4C0,+0x9E0,+0xDF0,+0xF00,+0x11B0,+0xF50,+0xFD0 and defaults (+0x103C=20.0 ...)
 * Info: proposed name InitStepplayer (T1); class STEPPLAYER slot7; TU stepplayer
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70+verdicts_tu_r1r2
 * See: docs/engine/player.md#10-source-files, docs/engine/player.md#11-key-functions,
 *   docs/engine/world-sw.md#9-source-files
 * @end-sly2-annot */
void InitStepplayer(struct P *p)
{
    InitStep(p);
    InitJtAir((char *)p + 0x2C0, p);
    InitJtGrab((char *)p + 0x4C0, p);
    InitJtPlaces((char *)p + 0x9E0, p);
    InitJtHurt((char *)p + 0xDF0, p);
    InitJtMode((char *)p + 0xF00, p);
    InitThrow((char *)p + 0x11B0, p);
    InitJtPipe((char *)p + 0xF50, p);
    InitJtgoal((char *)p + 0xFD0, p);


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
