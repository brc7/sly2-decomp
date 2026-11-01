struct P { char pad[0x298]; int x298; char pad2[0x2DC - 0x29C]; int x2DC; char pad3[0x4C8 - 0x2E0]; int x4C8; char pad4[0xDF4 - 0x4CC]; int xDF4; };
struct S { struct P *x0; int x4; char pad[0x88]; int x90; };
extern int D_002E1E00;
struct GAME { char pad[0x3D78]; int counter; };
extern struct GAME D_003DD490;
int FSwHandsOff(struct GAME *g);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char P_x298_Offset[FIELD_OFFSET(P,x298)==664?1:-1];
typedef char P_x2DC_Offset[FIELD_OFFSET(P,x2DC)==732?1:-1];
typedef char P_x4C8_Offset[FIELD_OFFSET(P,x4C8)==1224?1:-1];
typedef char P_xDF4_Offset[FIELD_OFFSET(P,xDF4)==3572?1:-1];
typedef char S_x0_Offset[FIELD_OFFSET(S,x0)==0?1:-1];
typedef char S_x4_Offset[FIELD_OFFSET(S,x4)==4?1:-1];
typedef char S_x90_Offset[FIELD_OFFSET(S,x90)==144?1:-1];
typedef char GAME_counter_Offset[FIELD_OFFSET(GAME,counter)==15736?1:-1];
typedef char P_PrefixSize[sizeof(struct P)==3576?1:-1];
typedef char S_PrefixSize[sizeof(struct S)==148?1:-1];
typedef char GAME_PrefixSize[sizeof(struct GAME)==15740?1:-1];
typedef char int_Size[sizeof(int)==4?1:-1];
/** @sly2-annot addr=0x1A2230
 * Purpose: Base check: object flag x298, hands-off, or for kinds 0x2000/0x4000 depending on hurt/grab/air
 *   sub-states
 * Info: proposed name FStepplayerIgnoreZap (T2); TU hurt
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#hurt-0x1a0b18-0x1a2bd0
 * @end-sly2-annot */
int func_001A2230(struct S *s, int n)
{
    if (s->x0->x298)
        return 1;
    if (n == 0x2000 || n == 0x4000) {
        switch (s->x0->xDF4) {
        case -1:
            switch (s->x0->x4C8) {
            case -1:
                switch (s->x0->x2DC) {
                case -1:
                    goto ret0;
                case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
                    {
                        register int k __asm__("$3") = 0x4000;
                        register int result __asm__("$2") = 1;
                        if (n == k) return result;
                        result = 0;
                        return result;
                    }
                }
                return 0;
            case 1: case 2: case 5: case 6: case 9:
                goto ret0;
            }
            goto ret1;
        case 3: case 5: case 10: case 11:
            goto ret1;
        }
        goto ret0;
    }
    if (s->x4 != -1)
        return 1;
    if (s->x90 == 1)
        return 1;
    if (D_002E1E00 & 1)
        return 1;
    if (!FSwHandsOff(&D_003DD490))
        goto ret0;
ret1:
    return 1;
ret0:
    return 0;
}
