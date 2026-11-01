// Choose a weighted reward and distribute the remaining pickup count.
extern "C" {
struct ITEM {int kind,flags,key,count;};
struct PICK {int weight,key;};
struct TABLE {int minimum,maximum;float chance;struct PICK picks[6];};
struct OBJECT {char pad[0x230];void *state;};
struct OWNER;
struct P {char pad404[0x404];struct ITEM items[8];int count;char pad488[8];int busy;struct OWNER *owner;};
extern char D_003DD490[];
extern struct TABLE D_002C3968[],D_002C3748[];
void func_001C2DB8(struct OWNER *);
int func_00210AE0(void);
int func_00132B10(void *,int);
void *func_002333B4(void *,int,int);
struct OBJECT *func_0015E938(void *,int,int,void *);
int func_00164030(void *);
float func_0011D448(float,float);
int func_0011D3F8(int,int);
/** @sly2-annot addr=0x1F1A40
 * Purpose: Roll the guard's loot at load: per-world table (D_002C3968 for cid 0x3A guards, else D_002C3748,
 *   indexed by NWorldCur): random coin total split into 2-3 coin entries (oid 0x2B3) plus, with probability
 *   chance, one weighted-random loot entry (oid 0x2B5, key from 6 weighted picks)
 * Info: proposed name GenerateStepguardLoot (T1); TU pickpocket
 * Notes: names_v2 C proposed StartSound__F5SFXIDPP3AMBP3ALOP6VECTORfffffP2LMT9 (0.438)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void func_001F1A40(struct P *p)
{
    struct TABLE *table;
    struct ITEM reward;
    int id,total,remaining,slots;unsigned int i;
    func_001C2DB8(p->owner);
    if(p->busy)return;
    id=func_00210AE0();if(id==-1)id=1;
    if(func_00132B10(p,0x3A))table=&D_002C3968[id];else table=&D_002C3748[id];
    func_002333B4(&reward,0,sizeof(reward));
    if(p->count>0){
        struct OBJECT *object=func_0015E938(D_003DD490,4,p->items[0].kind,p->owner);
        if(object && func_00132B10(object,0x5C) && object->state && func_00164030(object->state))p->count=0;
        if(p->count>0){reward=p->items[0];}
        else goto choose_reward;
    }else{
choose_reward:
        if(func_0011D448(0.0f,1.0f)>table->chance){reward.key=-1;reward.kind=-1;}
        else{
            total=0;for(i=0;i<6;i++)total+=table->picks[i].weight;
            int choice=func_0011D3F8(0,total-1);
            const PICK *pick=&table->picks[0];
            register int weight asm("$3")=pick->weight; /* pin: weight must be v1 (global-alloc gives it v0 and swaps pick/weight otherwise) */
            asm("" : "+r"(weight)); /* empty asm: gives the first weight load a dependent so sched2 issues it before the choice copy, as in the target */
            i=0;goto test_pick;
next_pick:
            choice-=pick->weight;i++;
            if(i>=6)goto selected_pick;
            pick=&table->picks[i];
            weight=pick->weight;
test_pick:
            if(choice>=weight)goto next_pick;
            reward.key=pick->key;
selected_pick:;
            reward.count=1;reward.kind=0x2B5;
        }
    }
    p->count=0;
    remaining=func_0011D3F8(table->minimum,table->maximum);
    slots=func_0011D3F8(2,3);
    int j=0;
    if(slots>0){do{
        struct ITEM *item=&p->items[p->count++];
        item->kind=0x2B3;item->flags=0;
        int amount=remaining/(slots-j);
        if(amount<1)amount=1;
        remaining-=amount;item->count=amount;
        if(!remaining)break;j++;
    }while(j<slots);}
    if(reward.count>0)p->items[p->count++]=reward;
}

}
