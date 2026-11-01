struct K { int a; int b; };
extern struct K D_002A5F98;
extern char D_003DD490[];
void OnStepplayerActive(void *p);
void EnsureSwCallback(void *, void *, struct K, int);
void func_0018E3A0(void *p)
{
    OnStepplayerActive(p);
    EnsureSwCallback(D_003DD490, p, D_002A5F98, 0);
}
