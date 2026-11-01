typedef unsigned long ulong;
struct A { char pad[0x28]; ulong lo : 60; ulong b60 : 1; ulong hi : 3; };
struct B { char pad[0x10]; ulong lo : 50; ulong b50 : 1; ulong hi : 13; };
extern "C" void AddSoPartToTree(A *, B *);
extern "C" void ShowSoPart(A *pa, B *pb)
{
    if (pb->b50) { pb->b50 = 0;
    bool f = pa->b60; if (f) AddSoPartToTree(pa, pb); }
}
