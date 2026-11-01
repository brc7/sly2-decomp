struct S { char pad[0x174]; void *p174; };
void HandleBasicMessage(struct S *p, int n);
void func_001FB098(void *p, float g);
void func_00258040(void **pp, int n);
void HandlePoMessage(struct S *p, int n)
{
    HandleBasicMessage(p, n);
    if (n == 5) {
        void **pp = &p->p174;
        if (pp && *pp) {
            func_001FB098(*pp, 0.0f);
            func_00258040(pp, 0);
        }
    }
}
