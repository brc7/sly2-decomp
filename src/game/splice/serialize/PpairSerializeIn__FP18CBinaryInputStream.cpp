// MATCH (Claude agent laneB14, 2026-10-23), from v5 (4): fNext computed BEFORE tagk (so b dies at the andi and sched puts tagk first), the b register pin and the barrier removed. No pins or asms.
// v5 (Claude agent laneB11, aligned 4): v4 + empty asm barrier after the fNext computation (lw sp.p now first in the join block). Left: andi/srl/sltiu order before the switch range check (target interleaves sltiu, andi fNext in the beqz slot). Barrier is a probe, not a final fix.
// v4 (Claude agent, aligned 5): v3 + b pinned to $17 (s1) and loops 2-5 as { float *ag = pref->u.ag; float g = f(); ag += i; *ag = g; }. Left: 8 lines of scheduling after the D_003E1C34 block (target: lw sp.p in the beqz slot, andi tagk before srl fNext).
// 227 diff, structurally complete (NODE loader, jump table 0x2A4500). Mostly a one-instruction shift: target fills the
// beqz delay slot with the sp.p load (pref first), we put srl b,7 there; target also keeps the float-loop counter in $s2
// (6 saved regs, frame 0x90) where we use 5. `register ... b __asm__("$17")` keeps b in s1 like the target.
struct NODE; struct OBJ; struct T18; struct T19; struct BIS;
struct CRef {
    int m_tagk; union { int n; float g; void *pv; float *ag; NODE *pnode; } u;
    CRef() { m_tagk = -1; }
    CRef(const CRef &r);
    void Decref();
    void InitTag(int tagk);
    ~CRef() { Decref(); }
    CRef &operator=(const CRef &r);
    CRef &operator=(int n);
    CRef &operator=(float g);
    CRef &operator=(bool f);
    CRef &operator=(OBJ *p);
    CRef &operator=(T18 *p);
    CRef &operator=(T19 *p);
    CRef &operator=(NODE *p);
    void SetTag(int tagk) { Decref(); InitTag(tagk); }
};
extern char D_004520A8[];
extern int D_003E1C34;
extern "C" {
void func_0011CD98(void *pheap, void *pv);
void GcPair_Unlink(void *pv);
NODE *PvNewPair();
int U8Read__18CBinaryInputStream(BIS *pbis);
int S32Read__18CBinaryInputStream(BIS *pbis);
int S16Read__18CBinaryInputStream(BIS *pbis);
float F32Read__18CBinaryInputStream(BIS *pbis);
unsigned int U32Read__18CBinaryInputStream(BIS *pbis);
}
struct SPM {
    NODE *p;
    ~SPM();
    void operator=(NODE *pn);
};
struct NODE {
    int cref;
    int x4;
    CRef ref;
    SPM pnext;
    void operator delete(void *pv) { GcPair_Unlink(pv); func_0011CD98(D_004520A8, pv); }
};
struct SPN {
    NODE *p;
    void Set(NODE *pn);
    SPN() {}
    SPN(NODE *pn) { Set(pn); }
    SPN(const SPN &);
    ~SPN() { NODE *pn = p; if (pn && --pn->cref == 0) delete pn; }
    NODE *operator->() { return p; }
};
extern "C" SPN PpairSerializeIn__FP18CBinaryInputStream(BIS *pbis);
/** @sly2-annot addr=0x12A3D8
 * Purpose: Deserializes a compiled Splice expression tree from the level stream (typed U8 tag,
 *   S32/S16/F32/U32 payloads, jump table 0x2A4500)
 * Info: proposed name PpairSerializeIn (T1); TU serialize
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels;
 *   splice Ref node list (WE header) | splice_scripts format doc cites this function by this name
 *   (CONFIRMED(code) in the doc)
 * Confidence: high [T1 0.97] - evidence: static analysis; sources:
 *   r1_116918+parse-verified+format-doc:splice_scripts
 * See: docs/engine/splice-scripting.md#1-layout-of-the-runtime,
 *   docs/engine/splice-scripting.md#6-source-files, docs/engine/splice-scripting.md#7-key-functions
 * @end-sly2-annot */
SPN PpairSerializeIn__FP18CBinaryInputStream(BIS *pbis) { unsigned int b = U8Read__18CBinaryInputStream(pbis);
 if (b == 0xFF) return SPN(0);
 SPN sp; NODE *pnNew = PvNewPair(); sp.Set(pnNew);
 if (D_003E1C34) { S32Read__18CBinaryInputStream(pbis); S16Read__18CBinaryInputStream(pbis); S16Read__18CBinaryInputStream(pbis); S16Read__18CBinaryInputStream(pbis); S16Read__18CBinaryInputStream(pbis); S16Read__18CBinaryInputStream(pbis); S16Read__18CBinaryInputStream(pbis); }
 CRef *pref = &sp->ref; int fNext = (b >> 7) & 0xFF; int tagk = b & 0x7F;
 switch (tagk) {
 case 0: case 1: case 2: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 20:
   pref->SetTag(tagk); break;
 case 21: *pref = S32Read__18CBinaryInputStream(pbis); break;
 case 22: *pref = F32Read__18CBinaryInputStream(pbis); break;
 case 23: *pref = U32Read__18CBinaryInputStream(pbis) != 0; break;
 case 24: *pref = (T18 *)U32Read__18CBinaryInputStream(pbis); break;
 case 25: *pref = (T19 *)U32Read__18CBinaryInputStream(pbis); break;
 case 33: *pref = (OBJ *)U32Read__18CBinaryInputStream(pbis); break;
 case 26: pref->SetTag(0x1A); for (int i = 0; i < 3; i++) { float *pg = &pref->u.ag[i]; *pg = F32Read__18CBinaryInputStream(pbis); } break;
 case 27: pref->SetTag(0x1B); for (int i = 0; i < 16; i++) { float *ag = pref->u.ag; float g = F32Read__18CBinaryInputStream(pbis); ag += i; *ag = g; } break;
 case 28: pref->SetTag(0x1C); for (int i = 0; i < 3; i++) { float *ag = pref->u.ag; float g = F32Read__18CBinaryInputStream(pbis); ag += i; *ag = g; } break;
 case 29: pref->SetTag(0x1D); for (int i = 0; i < 2; i++) { float *ag = pref->u.ag; float g = F32Read__18CBinaryInputStream(pbis); ag += i; *ag = g; } break;
 case 30: pref->SetTag(0x1E); for (int i = 0; i < 3; i++) { float *ag = pref->u.ag; float g = F32Read__18CBinaryInputStream(pbis); ag += i; *ag = g; } break;
 case 31: *pref = PpairSerializeIn__FP18CBinaryInputStream(pbis).p; break;
 } if (fNext) { NODE *pn = sp.p; SPN next = PpairSerializeIn__FP18CBinaryInputStream(pbis); pn->pnext = next.p; } return SPN(sp.p); }
