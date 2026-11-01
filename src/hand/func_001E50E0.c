struct G { int a; };
extern struct G D_003DD490;
struct P { char pad[0x38]; int fOn; int id; };
void DecrementSwHandsOff(struct G *pg, int id);
void func_0020F0F0(struct G *pg);
void IncrementSwHandsOff(struct G *pg, int id);
void func_0020F0C0(struct G *pg);
void func_001E50E0(struct P *p, int fOn, int id)
{
if (!fOn) { if (!p->fOn) return; } else { if (p->fOn) { if (id == p->id) return; DecrementSwHandsOff(&D_003DD490, p->id); func_0020F0F0(&D_003DD490); } IncrementSwHandsOff(&D_003DD490, id); func_0020F0C0(&D_003DD490); p->fOn = fOn; p->id = id; return; } DecrementSwHandsOff(&D_003DD490, p->id); func_0020F0F0(&D_003DD490); p->fOn = fOn; p->id = id;
}
