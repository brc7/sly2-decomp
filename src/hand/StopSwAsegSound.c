struct O { char pad[0x60]; int id; };
void func_001FB098(struct O *p, float f);
void func_00258040(struct O **pp, int a);
void StopSwAsegSound(void *a, int id, void *b, struct O **pp)
{
    struct O *p = *pp;
    if (!p)
        return;
    if (id != -1 && p->id != id)
        return;
    func_001FB098(p, 0.0f);
    func_00258040(pp, 0);
}
