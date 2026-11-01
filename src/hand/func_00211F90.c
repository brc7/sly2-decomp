struct VT {
    char pad[0x188]; short d49; short pd49; void (*f49)(void *,int);
    short d50; short pd50; void (*f50)(void *);
};
struct O {char pad[0x54];int state;};
struct E {struct O *o;int x4;};
struct S {
    char pad[0xC];struct VT *vt;char pad10[0x100];
    int countA;int x114;struct E a[15];int x190;int countB;int x198;struct E b[0];
};
extern char D_002C3EC0[0x90];
extern char D_002A8AA0[];
void func_00212C50(struct S *);
int FTaskComplete(struct O *);
int func_002121E8(struct S *,int);
int func_0021ECD8(char *,char *,...);
void func_00211F90(struct S *p)
{
    int i;
    int ready=0;
    int readyB=0;
    int loading=0;
    func_00212C50(p);
    for(i=0;i<p->countB;i++) {
        struct O *o=p->b[i].o;
        if(o) {
            if(o->state==1)loading++;
            else if(FTaskComplete(o)) {
                ready++;
                readyB++;
            }
        }
    }
    for(i=0;i<p->countA;i++) {
        struct O *o=p->a[i].o;
        if(o) {
            if(o->state==1)loading++;
            else if(FTaskComplete(o)) ready++;
        }
    }
    if(func_002121E8(p,0x314)) {
        func_0021ECD8(D_002C3EC0,D_002A8AA0,readyB);
        func_0021ECD8(D_002C3EC0+0x30,D_002A8AA0,p->countB);
        func_0021ECD8(D_002C3EC0+0x60,D_002A8AA0,p->countB-readyB);
    }
    if(ready==p->countB+p->countA || !loading) {
        p->vt->f50((char*)p+p->vt->d50);
    } else if(loading>0) {
        p->vt->f49((char*)p+p->vt->d49,-1);
    }
}
