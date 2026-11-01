struct CRef {
    int kind;
    union { int integer; float scalar; void *pointer; } value;
};
extern char D_002DEB30[];
void func_001331F8(void *p, int n, int deferred, int argumentCount,
                   void *const *arguments, struct CRef *output);
void func_001619A0(void *p, int n, void *q);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((struct t*)0x10000)->m)-0x10000)
typedef char CRef_KindOffset[FIELD_OFFSET(CRef,kind)==0?1:-1];
typedef char CRef_ValueOffset[FIELD_OFFSET(CRef,value)==4?1:-1];
typedef char CRef_Size[sizeof(struct CRef)==8?1:-1];
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Int_Size[sizeof(int)==4?1:-1];
/** @sly2-annot addr=0x1A61E0
 * Purpose: LO vt3: BASIC Splice-event post (0x1331F8) then for msg 5/6 notify D_002DEB30 with 0x1A/0x1B
 * Info: proposed name HandleLoSpliceEvent (T1); class LO slot3; TU lo
 * Notes: COLLISION: SendLoMessage already used (same name proposed for 0x133508); renamed to
 *   SendLoMessage_1A61E0 | superseded: SendLoMessage_1A61E0 (arbitration) | arbitration adopt r2b: CONFIRMED
 *   static: LO vtable slot 3 override; calls BASIC slot-3 base 0x1331F8 with the same args, then
 *   BroadcastGameEvent 0x1A/0x1B for event 5/6. Slot-3 dispat
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: r4_17F090+arbitration
 * See: docs/engine/game-progression.md#2-the-game-event-bus,
 *   docs/engine/object-model.md#2-the-lo-virtual-interface, docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
void func_001A61E0(void *p, int n, int deferred, int argumentCount,
                   void *const *arguments, struct CRef *output)
{
    func_001331F8(p, n, deferred, argumentCount, arguments, output);
    switch (n) {
    case 5:
        func_001619A0(D_002DEB30, 0x1A, p);
        break;
    case 6:
        func_001619A0(D_002DEB30, 0x1B, p);
        break;
    }
}
