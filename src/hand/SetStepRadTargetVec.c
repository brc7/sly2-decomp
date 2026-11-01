typedef int qword __attribute__((mode(TI)));
float func_002458F0(float y, float x);
void func_00204660(void *p, float g);
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1, %0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0, %1" : "=r"(y) : "r"(q)); return y; }
void SetStepRadTargetVec(void *p, qword v) { func_00204660(p, func_002458F0(GetY(v), GetX(v))); }
