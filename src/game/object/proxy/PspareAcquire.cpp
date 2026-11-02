// Attach an object node, bind its resources, and update its children.
typedef int qword __attribute__((mode(TI)));
struct P;
struct VTE {short delta,pad;void *fn;};
struct M4 {qword rows[4];};
struct NODE {P *owner;NODE *next;char pad8[8];M4 matrix;};
struct TREE {int id;TREE *parent;char pad8[8];P *owner;};
struct BIND {int kind,pad4;void *resource;void *begin,*end;P *object;};
struct TABLE {int count;P **objects;int capacity,used;char pad10[0x54];void *states;NODE *nodes;};
struct P {void *resource;char pad4[8];VTE *vt;void *context;char pad14[0x24];float scale;char pad3C[0x14];void *channel;char pad54[0xC];M4 matrix;char padA0[0x40];TABLE *table;int disabled;NODE *head;int bindCount;BIND *binds;};
struct SW {char pad[0x3F08];TABLE *tables;TREE *root;};
struct DESC {int kind,flags;char pad8[0x20];};
struct STREAM {int flags;unsigned char *cursor;int remaining,consumed;unsigned char *raw;int rawCount,compression,pending,word20;unsigned char *decompressed;int word28,word2C;VTE *vt;void *begin,*end;};
struct WORK {int id;P *found,*child;void *cursor;P *owner;};
extern "C" {
extern SW D_003DD490;
extern DESC D_002AC880[];
NODE *func_001CADE0(P *,int);
void ReleasePspare__FP5PROXYP10PROXYSPARE(P *,NODE *);
int IcopyFromPspare__FP5PROXYP10PROXYSPARE(P *,NODE *);
int IdProxy__Fii(int,int);
TREE *func_001C99D8(TREE *,int,int *);
int func_0015E678(SW *,int,int,P *,void *,int,P **);
STREAM *func_00118480(STREAM *);
void func_00118500(STREAM *,void *,void *,int);
short S16Read__18CBinaryInputStream(STREAM *);
void LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream(P *,DESC *,int,STREAM *);
void CFrame_AddParent(void *,void *);
void FramePtr_Assign(void **,void *);
void CMemInputStream_ctor(STREAM *,int);
}
static inline P *Adjusted(P *p,const VTE &entry){return (P *)((char *)p+entry.delta);}
static inline void Channel(P *p,void *context,void *channel){VTE &v=p->vt[10];((void(*)(P *,void *,void *,int))v.fn)(Adjusted(p,v),context,channel,0);}
static inline void Matrix(P *p,const M4 *m){VTE &v=p->vt[37];((void(*)(P *,const M4 *))v.fn)(Adjusted(p,v),m);}
static inline void Reset(P *p){VTE &v=p->vt[8];((void(*)(P *))v.fn)(Adjusted(p,v));}
static inline void Parent(P *p,P *parent){VTE &v=p->vt[26];((void(*)(P *,P *))v.fn)(Adjusted(p,v),parent);}
static inline void Query(P *p,void **cursor){VTE &v=p->vt[3];((void(*)(P *,int,int,int,void **,int))v.fn)(Adjusted(p,v),14,0,1,cursor,0);}
/** @sly2-annot addr=0x1CAEC8
 * Purpose: Acquire a spare copy for the proxy: choose, release from previous owner, attach objects at proxy
 *   placement, apply init records
 * Info: proposed name PspareAcquire (T1); TU proxy
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/object-model.md#5-proxies-and-spares-object-pools,
 *   docs/formats/world-file.md#world-file-brx-format-sly-2-ps2-scus-97316,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
extern "C" NODE *PspareAcquire(P *p,int minimum){
 STREAM stream;
 WORK work;
 NODE *node=func_001CADE0(p,minimum);
 if(!node)return 0;
 if(node->owner)ReleasePspare__FP5PROXYP10PROXYSPARE(node->owner,node);
 node->owner=p;node->next=p->head;p->head=node;
 work.owner=p;
 node->matrix=p->matrix;
 int row=IcopyFromPspare__FP5PROXYP10PROXYSPARE(p,node);
 work.id=IdProxy__Fii(p->table-D_003DD490.tables,row);
 TREE *tree=func_001C99D8(D_003DD490.root,1,&work.id);
 for(int i=0;i<p->bindCount;i++){
  BIND *bind=&p->binds[i];
  if(bind->kind==5)continue;
  if(bind->kind==6){work.found=tree->owner;if(!work.found)continue;}
  else if(func_0015E678(&D_003DD490,0xA05,bind->kind,0,tree,1,&work.found)!=1)continue;
  bind->object=work.found;
  func_00118480(&stream);
  func_00118500(&stream,bind->begin,bind->end,0);
  int index;
  while((index=S16Read__18CBinaryInputStream(&stream))>=0){DESC *desc=&D_002AC880[index];LoadOptionFromBrx__FPvP5EOPIDiP18CBinaryInputStream(work.found,desc,desc->flags&0x2031,&stream);}
  if(bind->resource && !p->disabled){
   if(work.found->resource)CFrame_AddParent(bind->resource,work.found->resource);
   FramePtr_Assign(&work.found->resource,bind->resource);
  }
  CMemInputStream_ctor(&stream,2);
 }
 for(int i=0;i<p->table->count;i++){
 void **cursor=&work.cursor;
  P *child=p->table->objects[row*p->table->count+i];
  VTE &entry=child->vt[42];
  work.child=child;
  ((void(*)(P *,float))entry.fn)(Adjusted(child,entry),p->scale);Channel(work.child,p->context,p->channel);Matrix(work.child,&p->matrix);Reset(work.child);Parent(work.child,p);
  work.cursor=&work.child;Query(p,cursor);
  work.cursor=&work.owner;Query(work.child,cursor);
 }
 return node;
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char attach_layout[(sizeof(void*)==4 && sizeof(qword)==16 && sizeof(VTE)==8 && sizeof(M4)==0x40 && sizeof(TABLE)==0x6C && sizeof(BIND)==0x18 && sizeof(DESC)==0x28 && sizeof(STREAM)==0x3C && OFF(VTE,delta)==0 && OFF(VTE,fn)==4 && OFF(NODE,owner)==0 && OFF(NODE,next)==4 && OFF(NODE,matrix)==0x10 && OFF(TREE,owner)==0x10 && OFF(BIND,kind)==0 && OFF(BIND,resource)==8 && OFF(BIND,begin)==0xC && OFF(BIND,end)==0x10 && OFF(BIND,object)==0x14 && OFF(TABLE,count)==0 && OFF(TABLE,objects)==4 && OFF(TABLE,states)==0x64 && OFF(TABLE,nodes)==0x68 && OFF(P,resource)==0 && OFF(P,vt)==0xC && OFF(P,context)==0x10 && OFF(P,scale)==0x38 && OFF(P,channel)==0x50 && OFF(P,matrix)==0x60 && OFF(P,table)==0xE0 && OFF(P,disabled)==0xE4 && OFF(P,head)==0xE8 && OFF(P,bindCount)==0xEC && OFF(P,binds)==0xF0 && OFF(SW,tables)==0x3F08 && OFF(SW,root)==0x3F0C && OFF(DESC,flags)==4 && OFF(STREAM,vt)==0x30 && OFF(STREAM,begin)==0x34 && OFF(STREAM,end)==0x38 && OFF(WORK,id)==0 && OFF(WORK,found)==4 && OFF(WORK,child)==8 && OFF(WORK,cursor)==0xC && OFF(WORK,owner)==0x10)?1:-1];
