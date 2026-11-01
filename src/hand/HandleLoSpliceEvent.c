struct CRef {
    int kind;
    union { int integer; float scalar; void *pointer; } value;
};
extern char D_002DEB30[];
void HandleBasicSpliceEvent(void *p, int n, int deferred, int argumentCount,
                   void *const *arguments, struct CRef *output);
void BroadcastGameEvent(void *p, int n, void *q);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char CRef_KindOffset[FIELD_OFFSET(CRef,kind)==0?1:-1];
typedef char CRef_ValueOffset[FIELD_OFFSET(CRef,value)==4?1:-1];
typedef char CRef_Size[sizeof(struct CRef)==8?1:-1];
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
void HandleLoSpliceEvent(void *p, int n, int deferred, int argumentCount,
                   void *const *arguments, struct CRef *output)
{
    HandleBasicSpliceEvent(p, n, deferred, argumentCount, arguments, output);
    switch (n) {
    case 5:
        BroadcastGameEvent(D_002DEB30, 0x1A, p);
        break;
    case 6:
        BroadcastGameEvent(D_002DEB30, 0x1B, p);
        break;
    }
}
