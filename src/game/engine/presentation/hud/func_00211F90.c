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
void UpdateWidget(struct S *);
int FTaskComplete(struct O *);
int FWidgetAnimPlaying(struct S *,int);
int func_0021ECD8(char *,char *,...);
/** @sly2-annot addr=0x211F90
 * Purpose: CID 0x81 Update: counts finished/active objects, writes counts into "Widget Item 0/1/2"
 *   (doneB,totalB,remainingB) for text ids 0x2FF..; shows while any active, hides when all done
 * Info: proposed name UpdateWidget129 (T2); class WIDGET129 slot16; TU hud
 * Notes: v2: demoted to T2 (placeholder token in name: 129) | runtime6 G17: counts complete tasks each frame
 *   and formats done/total/remaining | T2: placeholder token 129
 * Confidence: low (draft) [T2 0.65] - evidence: runtime (save-state RAM); sources: r7c_hud+runtime
 * See: docs/engine/game-progression.md#6-objective-counters-job-markers-and-the-episode-menu,
 *   docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00211F90(struct S *p)
{
    int i;
    int ready=0;
    int readyB=0;
    int loading=0;
    UpdateWidget(p);
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
    if(FWidgetAnimPlaying(p,0x314)) {
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
