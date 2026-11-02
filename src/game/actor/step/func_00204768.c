typedef int qword __attribute__((mode(TI)));
struct P;
struct Endpoint { char pad[8]; struct P *owner,*other; char pad10[0x30]; };
struct N { struct Endpoint e[2]; qword q80; float value; char pad94[8]; int kind; char padA0[0xC]; struct N *next; };
struct L { struct N *first; struct L *next; struct N **back; };
struct G { char pad[0x64]; struct L *groups; struct N *def; };
struct P { char pad[0xEC]; struct G *g; char padF0[0x118]; struct N *best; float t; char pad210[0x20]; qword position; };
extern float D_002A85F4,D_002DDED0;
struct SW;
extern struct SW D_003DD490;
void func_001F4F10(struct P *);
void FreeSwXpList__FP2SWP2XP(struct SW *,struct N *);
static inline struct P *Other(struct N*n,struct P*p){unsigned different=(unsigned)p^(unsigned)n->e[0].owner;int i=different>0;return ((struct Endpoint *)((char *)n+(i<<6)))->other;}
/** @sly2-annot addr=0x204768
 * Purpose: STEP vt58: base 0x1F4F10, then keep only the best (lowest distance) kind-14 base contact as +0x208
 *   (stamp +0x20C, pos +0x230) and free the other base contacts
 * Info: proposed name AdjustStepXps (T2); class STEP slot58; TU step
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00204768(struct P *p)
{
    struct N *best;
    struct L *group;
    func_001F4F10(p);
    {
        float value=D_002A85F4;
        group=p->g->groups;
        best=0;
        for(;;){
            struct N *node=group?group->first:p->g->def;
            for(;node;node=node->next){
                if(node->kind==14 && node->value<value){value=node->value;best=node;}
            }
            if(!group)break;
            group=group->next;
        }
    }
    p->best=best;
    if(best){p->t=D_002DDED0;p->position=*(qword *)((char *)best+0x50);}
    {
        struct N *removed=0;
        group=p->g->groups;
        for(;;){
            struct N **link=group?&group->first:&p->g->def;
            struct N *node;
            while((node=*link)!=0){
                if(Other(node,p)==p && node->kind==14 && node!=best){
                    *link=node->next;
                    node->next=removed;
                    removed=node;
                }else link=&node->next;
            }
            if(!group)break;
            *group->back=group->first;
            group=group->next;
        }
        FreeSwXpList__FP2SWP2XP(&D_003DD490,removed);
    }
}
