struct P2 { int a, b; };
void PostLoLoad(void *p);
void PostBtnLoad(void *p);
void SetVolbtnButtons(void *p, int n);
int func_001AE1D0(struct P2 *p);
void PostVolbtnLoad(char *p)
{
    PostLoLoad(p);
    PostBtnLoad(p + 0x130);
    SetVolbtnButtons(p, 0);
    if (*(int *)(p + 0x2C8)) {
        struct P2 s;
        s.a = 0x100;
        s.b = 0x10;
        *(int *)(p + 0x2C4) = func_001AE1D0(&s);
    }
}
