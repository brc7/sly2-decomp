typedef int qword __attribute__((mode(TI)));
struct G { int a; };
extern struct G D_003DD490;
struct H { char pad[0x90]; qword q90; };
extern struct H D_002DDF40;
void *PpoHero(struct G *g);
qword func_001A62E8(void *p);
qword func_00173A18(void) { struct G *g = &D_003DD490; if (PpoHero(g)) return func_001A62E8(PpoHero(g)); return D_002DDF40.q90; }
