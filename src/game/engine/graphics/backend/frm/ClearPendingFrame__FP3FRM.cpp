struct DL{void*first;void*last;int linkOffset;};
extern void*D_002DD2FC;
struct DLI{
 DL*list;void**next;int linkOffset;void*previous;
 DLI(DL*owner){int offset=owner->linkOffset;previous=D_002DD2FC;D_002DD2FC=this;list=owner;next=(void**)owner;linkOffset=offset;}
 ~DLI(){D_002DD2FC=previous;}
 void*Next(){void*value=*next;next=(void**)((unsigned int)value+linkOffset);return value;}
};
struct VTE{short delta,pad;void(*method)(void*);};
struct LO{char pad[12];VTE*vt;};
struct Y{char pad[0x1088];void*storage;};
struct FR{char pad[0x34];DL dl34,dl40;};
extern char D_003E1530[],D_003E1518[];
extern "C" int FIsDlEmpty__FP2DL(DL*);
extern "C" void RemoveDlEntry__FP2DLPv(DL*,void*);
extern "C" void ClearDl__FP2DL(DL*);
extern "C" void func_0011CD98(void*,void*);
/** @sly2-annot addr=0x160CE0
 * Purpose: Releases objects whose destruction was deferred until the frame finished: calls vtable slot 2
 *   (Decref) on every LO in frm->dl34 and frees the pool blocks in frm->dl40
 * Info: proposed name ClearPendingFrame (T1); TU frm
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r3_1539A8
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
extern "C" void ClearPendingFrame__FP3FRM(FR*frame){
 if(!FIsDlEmpty__FP2DL(&frame->dl34)){
  DLI iter(&frame->dl34);
  for(LO*object=(LO*)iter.Next();object;object=(LO*)iter.Next()){
   RemoveDlEntry__FP2DLPv(&frame->dl34,object);
   VTE*entry=&object->vt[2];entry->method((char*)object+entry->delta);
  }
  ClearDl__FP2DL(&frame->dl34);
 }
 if(!FIsDlEmpty__FP2DL(&frame->dl40)){
  DLI iter(&frame->dl40);
  for(Y*object=(Y*)iter.Next();object;object=(Y*)iter.Next()){
   if(object->storage)func_0011CD98(D_003E1530,object->storage);
   RemoveDlEntry__FP2DLPv(&frame->dl40,object);
   func_0011CD98(D_003E1518,object);
  }
  ClearDl__FP2DL(&frame->dl40);
 }
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Integer_Size[sizeof(int)==4&&sizeof(unsigned int)==4?1:-1];
typedef char DL_Size[sizeof(DL)==12?1:-1];
typedef char DL_First[FIELD_OFFSET(DL,first)==0?1:-1];
typedef char DL_Last[FIELD_OFFSET(DL,last)==4?1:-1];
typedef char DL_LinkOffset[FIELD_OFFSET(DL,linkOffset)==8?1:-1];
typedef char DLI_Size[sizeof(DLI)==16?1:-1];
typedef char DLI_List[FIELD_OFFSET(DLI,list)==0?1:-1];
typedef char DLI_Next[FIELD_OFFSET(DLI,next)==4?1:-1];
typedef char DLI_LinkOffset[FIELD_OFFSET(DLI,linkOffset)==8?1:-1];
typedef char DLI_Previous[FIELD_OFFSET(DLI,previous)==12?1:-1];
typedef char VTE_Size[sizeof(VTE)==8?1:-1];
typedef char VTE_Delta[FIELD_OFFSET(VTE,delta)==0?1:-1];
typedef char VTE_Method[FIELD_OFFSET(VTE,method)==4?1:-1];
typedef char LO_PrefixSize[sizeof(LO)==16?1:-1];
typedef char LO_VT[FIELD_OFFSET(LO,vt)==12?1:-1];
typedef char Y_PrefixSize[sizeof(Y)==0x108C?1:-1];
typedef char Y_Storage[FIELD_OFFSET(Y,storage)==0x1088?1:-1];
typedef char FR_PrefixSize[sizeof(FR)==0x4C?1:-1];
typedef char FR_FirstList[FIELD_OFFSET(FR,dl34)==0x34?1:-1];
typedef char FR_SecondList[FIELD_OFFSET(FR,dl40)==0x40?1:-1];
