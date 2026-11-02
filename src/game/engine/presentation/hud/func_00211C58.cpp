struct B { char pad[0xC]; B() __asm__("func_00212158"); virtual void f(); };
struct D : B { char pad2[0x100]; int a[4]; int x120; D() __asm__("func_00211C58"); virtual void f(); };
D::D() { a[0] = -1; a[1] = -1; a[2] = -1; a[3] = -1; x120 = -1; }
