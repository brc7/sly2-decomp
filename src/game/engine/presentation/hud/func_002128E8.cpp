struct B { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
  virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10();
  virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
  virtual void v16(); };
struct S : B {
  char pad10[0x48]; int x58; char pad5c[0x14]; int x70; int x74; int x78; char pad7c[8]; void *x84;
  void A(int) __asm__("func_00212A70");
  void Bf(int) __asm__("func_00212A50");
  int F(void *, int) __asm__("func_002142B0");
  void M(int) __asm__("func_002128E8");
};
void S::M(int k)
{
    if (!x58)
        A(1);
    Bf(2);
    x70 = k;
    x74 = -1;
    x78 = -1;
    if (k != -1) {
        A(1);
        A(2);
        return;
    }
    x70 = 0x314;
    if (F(x84, 0x30F))
        x74 = 0x30F;
    if (F(x84, 0x310))
        x70 = 0x310;
    if (F(x84, 0x311))
        x78 = 0x311;
    if (x58 == 1)
        v16();
}
