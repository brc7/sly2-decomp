extern "C" void func_00218638(void *p);
struct B { char pad[0xC];
  virtual void v1(); virtual void v2(); virtual void v3(int, int, int, int, int);
  virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
  virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
  virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
  virtual void v19(); virtual void v20(); virtual void v21(int);
};
struct S : B {
  char pad10[0x6C]; int x7C; char pad80[4]; void *x84; int x88; char pad8C[0x20]; int xAC;
  void F(int) __asm__("func_00214300");
  void M() __asm__("func_00213898");
};
void S::M()
{
    func_00218638(x84);
    if (xAC)
        F(-1);
    v21(0);
    v3(6, 0, 0, 0, 0);
    x88 = 0;
    x7C = -1;
}
