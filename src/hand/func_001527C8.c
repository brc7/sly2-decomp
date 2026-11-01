struct K { short x; short y; int i; };
extern struct K D_002A5010;
extern char D_003DD490[];
void PostLoLoad(void *p);
void PostSwCallback(void *g, void *p, struct K k, int n);
void func_001527C8(void *p)
{
    PostLoLoad(p);
    PostSwCallback(D_003DD490, p, D_002A5010, 0);
}
