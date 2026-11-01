extern int D_002E1E00;
struct G;
extern struct G D_003DD490;
int FSwHandsOff(struct G *p);
int FInvulnerablePo(void) { /*B*/if ((D_002E1E00 & 1) || FSwHandsOff(&D_003DD490)) return 1; return 0;/*E*/ }
