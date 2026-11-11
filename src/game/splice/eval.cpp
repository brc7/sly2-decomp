// eval.cpp -- the Splice evaluator: EvalExpression dispatches on the expression's tag to one evaluator per special form
// (set!, define, assert, if, or, and, cond, case, let, while, lambda, begin) or to EvalApply for a call.
// TU 0x126438..0x127B18 (19 functions, address order).
// SLY2_ALIAS: __static_initialization_and_destruction_0=StaticInit_eval
// SLY2_ALIAS: _GLOBAL_$I$*=GlobalConstructors_eval
#include "sly2/pair.h"
#include "sly2/sw.h"
#include "sly2/cframe.h"
#include "sly2/proc.h"
#include "sly2/method.h"

// 0x1279C0 StaticInit_eval and GlobalConstructors_eval: g++-generated for the six engine slot pools.
SLY2_INSTANTIATE_SLOT_POOLS()

extern "C" {
CRef CFrame_AddBinding(CFrame *frame, int sym, CRef *pref);
void printf(const char *psz);
}
extern const char D_002A43B0[] = "Splice assert: ";   /* assert failure message */

typedef CRefOwner<CFrame> SPF;   /* owning pointer to a frame (inline release) */

typedef CRefOwner<CProc> SPO;   /* owning pointer to a proc (inline release) */

/* Built-in procedure table entry (tag 0x19 indexes it). */
struct BIF
{
    CRef (*function)(int c, CRef *aref, CFrame *frame);
    int minArgCount;
    int hasRest;
};

extern BIF g_mpbifkbif[];

CRef EvalExpression(CPair *expr, CFrame *frame);
CRef EvalLambdaBody(CPair *expr, CFrame *frame);
CRef BuiltinPrint(int c, CRef *aref, CFrame *frame);
CRef BuiltinEqv(int c, CRef *aref, CFrame *frame);

/* The argument list of a special form: the rest of the list after the keyword. */
static inline CPair *PpairArgs(CPair *expr) { return ((CPair *)expr->ref.u.pv)->next.p; }

// Evaluates a symbol expression: looks the symbol up in the frame and its parents (CFrame::FindBinding) and
// returns the bound value (Nil when unbound).
// Also known as: RefEvalSymbol (upstream readable / Sly 1 name).
CRef EvalSymbol(CPair *expr, CFrame *frame)
{
    CRef ref;
    frame->FindBinding(expr->ref.u.n, 1, &ref);
    return ref;
}

// (set! sym expr): evaluates expr and stores it in the existing binding of sym found in the frame chain
// (CFrame::AssignBinding); returns Void.
// Also known as: RefEvalSet (upstream readable / Sly 1 name).
CRef EvalSet(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *args = PpairArgs(expr);
    ref = EvalExpression(args->next.p, frame);
    ref = frame->AssignBinding(args->ref.u.n, &ref);
    return ref;
}

// (define sym expr): evaluates expr and adds a new binding for sym to this frame (CFrame_AddBinding).
// Also known as: RefEvalDefine (upstream readable / Sly 1 name).
CRef EvalDefine(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *args = PpairArgs(expr);
    ref = EvalExpression(args->next.p, frame);
    ref = CFrame_AddBinding(frame, args->ref.u.n, &ref);
    return ref;
}

// (assert expr ...): evaluates each operand and prints "Splice assert: " plus the failing expression when one is false;
// returns Void. A debug aid for level scripts.
// Also known as: RefEvalAssert (upstream readable / Sly 1 name).
CRef EvalAssert(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *args = PpairArgs(expr);
    CPair *pmsg = args->next.p;
    ref = EvalExpression(args, frame);
    if (ref.u.n == 0)
    {
        CPair *pargsMsg = PpairArgs(pmsg);
        printf(D_002A43B0);
        BuiltinPrint(1, &pargsMsg->ref, frame);
    }
    ref.Decref();
    ref.InitTag(0x24);
    return ref;
}

// Special form 6 (keyword unknown, new in Sly 2): evaluates its first operand and returns that value, or Void when the
// form has no operand. (inferred: the keyword name is not known; see docs/systems/splice-scripting.md section 4.)
// Also known as: RefEvalDebug (upstream readable / Sly 1 name; RefEvalDebug__FP5CPairP6CFrame).
extern "C" CRef EvalSpecialForm6(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *args = ((CPair *)expr->ref.u.pv)->next.p;
    if (args)
        ref = EvalExpression(args, frame);
    else
    {
        ref.Decref();
        ref.InitTag(0x24);
    }
    return ref;
}

// (if test then [else]): takes the then branch only when the test value word is 1 (#t; the tag is not checked),
// otherwise the optional else branch (Void when absent). Unlike Scheme, non-#f values other than 1 take else.
// Also known as: RefEvalIf (upstream readable / Sly 1 name).
CRef EvalIf(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *ppairCond = PpairArgs(expr);
    CPair *ppairThen = ppairCond->next.p;
    CPair *ppairElse = ppairThen->next.p;
    ref = EvalExpression(ppairCond, frame);
    if (ref.u.n == 1)
        ref = EvalExpression(ppairThen, frame);
    else if (ppairElse)
        ref = EvalExpression(ppairElse, frame);
    else
    {
        ref.Decref();
        ref.InitTag(0x24);
    }
    return ref;
}

// (or expr ...): returns the first operand value that is not #f, or #f when all are false.
// Also known as: RefEvalOr (upstream readable / Sly 1 name).
CRef EvalOr(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *pq;
    for (pq = PpairArgs(expr); pq; pq = pq->next.p)
    {
        ref = EvalExpression(pq, frame);
        if (ref.m_tagk != 0x17 || ref.u.n != 0)
            return ref;
    }
    ref = false;
    return ref;
}

// (and expr ...): stops at the first #f operand and returns it; otherwise returns the last value.
// Also known as: RefEvalAnd (upstream readable / Sly 1 name).
CRef EvalAnd(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *pq;
    ref = true;
    for (pq = PpairArgs(expr); pq; pq = pq->next.p)
    {
        ref = EvalExpression(pq, frame);
        if (ref.m_tagk == 0x17 && ref.u.n == 0)
            return ref;
    }
    return ref;
}

// (cond (test body...) ... [else body...]): evaluates the body of the first clause whose test is true (else = tag 0xB).
// Also known as: RefEvalCond (upstream readable / Sly 1 name).
CRef EvalCond(CPair *expr, CFrame *frame)
{
    CRef ref;
    CRef test;
    CPair *pcl;
    ref.Decref();
    ref.InitTag(0x24);
    pcl = PpairArgs(expr);
    while (pcl)
    {
        CPair *pc = (CPair *)pcl->ref.u.pv;
        int f;
        if (pc->ref.m_tagk == 0xB)
        {
            f = 1;
        }
        else
        {
            test = EvalExpression(pc, frame);
            f = 0;
            if (test.m_tagk != 0x17 || test.u.n)
                f = 1;
            if (f && !pc->next.p)
                ref = test;
        }
        if (f)
        {
            for (pc = pc->next.p; pc; pc = pc->next.p)
                ref = EvalExpression(pc, frame);
            pcl = 0;
        }
        else
        {
            pcl = pcl->next.p;
        }
    }
    return ref;
}

// (case key ((datum...) body...) ... [else ...]): evaluates key and runs the first clause holding an eqv? datum
// (BuiltinEqv), or the else clause.
// Also known as: RefEvalCase (upstream readable / Sly 1 name).
CRef EvalCase(CPair *expr, CFrame *frame)
{
    CRef ref;
    CRef arefT[2];
    CRef test;
    CPair *pkey;
    CPair *pcl;
    ref.Decref();
    ref.InitTag(0x24);
    pkey = PpairArgs(expr);
    arefT[0] = EvalExpression(pkey, frame);
    pcl = pkey->next.p;
    while (pcl)
    {
        CPair *pc = (CPair *)pcl->ref.u.pv;
        int f;
        if (pc->ref.m_tagk == 0xB)
        {
            f = 1;
        }
        else
        {
            CPair *pk;
            f = 0;
            pk = (CPair *)pc->ref.u.pv;
            while (pk)
            {
                arefT[1] = pk->ref;
                test = BuiltinEqv(2, arefT, frame);
                if (test.m_tagk == 0x17 && test.u.n)
                {
                    f = 1;
                    pk = 0;
                }
                else
                {
                    pk = pk->next.p;
                }
            }
        }
        if (f)
        {
            for (pc = pc->next.p; pc; pc = pc->next.p)
                ref = EvalExpression(pc, frame);
            pcl = 0;
        }
        else
        {
            pcl = pcl->next.p;
        }
    }
    return ref;
}

// (let ((sym val) ...) body...): evaluates each value in the outer frame, binds them in a new child frame and evaluates
// the body there.
// Also known as: RefEvalLet (upstream readable / Sly 1 name).
CRef EvalLet(CPair *expr, CFrame *frame)
{
    CRef ref;
    CRef refVal;
    CRef refT;
    SPF spf(spNull);
    CPair *args = PpairArgs(expr);
    CPair *pn;
    if (args->ref.m_tagk == 0x1F)
    {
        CFrame *pframeNew = CGcObj<CFrame>::PtNew();
        spf = pframeNew;
        for (pn = (CPair *)args->ref.u.pv; pn; pn = pn->next.p)
        {
            CPair *pb = (CPair *)pn->ref.u.pv;
            refVal = EvalExpression(pb->next.p, frame);
            refT = CFrame_AddBinding(spf.p, pb->ref.u.n, &refVal);
        }
        spf.p->SetSingleParent(frame);
    }
    else
    {
        spf = frame;
    }
    CPair *pq;
    for (pq = args->next.p; pq; pq = pq->next.p)
        ref = EvalExpression(pq, spf.p);
    return ref;
}

// (while test body): re-evaluates the one body expression until test yields #f (bool tag 0x17, value 0; any other
// value keeps looping); returns Void, not the last body value.
// Also known as: RefEvalWhile (upstream readable / Sly 1 name).
CRef EvalWhile(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *ppairCond = PpairArgs(expr);
    CPair *ppairBody = ppairCond->next.p;
    for (;;)
    {
        ref = EvalExpression(ppairCond, frame);
        if (ref.m_tagk == 0x17 && ref.u.n == 0)
            break;
        ref = EvalExpression(ppairBody, frame);
    }
    ref.Decref();
    ref.InitTag(0x24);
    return ref;
}

// (lambda (params... [. rest]) body...): builds a CProc closure over the current frame, recording the required
// parameter count, whether a rest parameter follows, the parameter list and the body.
// Also known as: RefEvalLambda (upstream readable / Sly 1 name).
CRef EvalLambda(CPair *expr, CFrame *frame)
{
    CPair *args = PpairArgs(expr);
    CPair *ppairBody = args->next.p;
    SPO sp(CGcObj<CProc>::PtNew());
    sp->m_pframe = frame;
    if (args->ref.m_tagk == 0)
        sp->m_ppairParams = 0;
    else
        sp->m_ppairParams = (CPair *)args->ref.u.pv;
    sp->m_ppairBody = ppairBody;
    int f = 0;
    int c = 0;
    CPair *pparam;
    for (pparam = sp->m_ppairParams.p; pparam;)
    {
        if (pparam->ref.m_tagk == 0x14)
        {
            f = 1;
            pparam = 0;
        }
        else
        {
            pparam = pparam->next.p;
            c++;
        }
    }
    sp->m_cargMin = c;
    sp->m_fRest = f;
    return CRef(sp.p);
}

// Runs a closure's body in its call frame: evaluates each body expression in order and returns the last value.
/* The symbol says CPair *, but the argument is the CProc being applied (EvalApply passes it). */
CRef EvalLambdaBody(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *pbody = ((CProc *)expr)->m_ppairBody.p;
    do
    {
        ref = EvalExpression(pbody, frame);
        pbody = pbody->next.p;
    } while (pbody);
    return ref;
}

// (begin expr ...): evaluates the operands in order and returns the last value (Void when empty).
// Also known as: RefEvalBegin (upstream readable / Sly 1 name).
CRef EvalBegin(CPair *expr, CFrame *frame)
{
    CRef ref;
    CPair *pq;
    ref.Decref();
    ref.InitTag(0x24);
    for (pq = PpairArgs(expr); pq; pq = pq->next.p)
        ref = EvalExpression(pq, frame);
    return ref;
}

/* EvalApply's owning pointers carry a padding word: the original's are 8 bytes in its stack frame (the frame size
   and the slot of every local depend on it). HYPOTHESIS: a second member not used here. A local derived struct of
   sly2/sp.h's owner (the same code; the copies start cleared). */
template <class T>
struct CRefOwnerPad : CRefOwner<T>
{
    int pad;
    CRefOwnerPad() : CRefOwner<T>(spNull) {}
    CRefOwnerPad(T *pt) : CRefOwner<T>(pt) {}
    void operator=(T *pt) { CRefOwner<T>::operator=(pt); }
};
typedef CRefOwnerPad<CPair> SPPAIR;
typedef CRefOwnerPad<CFrame> SPFRAME;

// Function application: evaluates the operator and the operands, then calls a built-in (tag 0x19, the
// g_mpbifkbif table of {function, minimum arguments, rest flag}), a native object method (tag 0x22), or a lambda (new
// frame binding the parameters, extra arguments collected into the rest list), checking the argument count.
// Also known as: RefEvalApply (upstream readable / Sly 1 name).
CRef EvalApply(CPair *expr, CFrame *frame)
{
    CRef ref;
    BIF *pbif = 0;
    CProc *proc = 0;
    CMethod *pmeth = 0;
    CPair *ppairFn = (CPair *)expr->ref.u.pv;
    CRef refOp = EvalExpression(ppairFn, frame);
    int cargMin, fRest;
    if (refOp.m_tagk == 0x19)
    {
        pbif = &g_mpbifkbif[refOp.u.n];
        cargMin = pbif->minArgCount;
        fRest = pbif->hasRest;
    }
    else if (refOp.m_tagk == 0x22)
    {
        pmeth = (CMethod *)refOp.u.pv;
        fRest = 0;
        cargMin = pmeth->m_crefReq;
    }
    else
    {
        proc = (CProc *)refOp.u.pv;
        cargMin = proc->m_cargMin;
        fRest = proc->m_fRest;
    }
    int carg = cargMin + (fRest != 0);
    CRef aref[32];
    CPair *pn = ppairFn->next.p;
    int c = cargMin;
    int i;
    for (i = 0; i < c; i++)
    {
        ref = EvalExpression(pn, frame);
        aref[i] = ref;
        pn = pn->next.p;
    }
    if (fRest)
    {
        SPPAIR spCur(pn);
        SPPAIR spLast;
        SPPAIR spList;
        while (spCur.p)
        {
            SPPAIR spNew(CGcObj<CPair>::PtNew());
            spNew->ref = EvalExpression(spCur.p, frame);
            if (spLast.p)
            {
                spLast->next = spNew.p;
                spLast = spNew.p;
            }
            else
            {
                spLast = spNew.p;
                spList = spLast.p;
            }
            c++;
            spCur = spCur->next.p;
        }
        if (spList.p)
            aref[i] = spList.p;
        else
            aref[i].SetNil();
    }
    switch (refOp.m_tagk)
    {
    case 0x19:
        ref = pbif->function(c, aref, frame);
        break;
    case 0x20:
    {
        SPFRAME spf(CGcObj<CFrame>::PtNew());
        int dummy[2];   /* sizes the frame like the original (0x240) */
        spf.p->SetSingleParent(proc->m_pframe.p);
        CPair *pparam = proc->m_ppairParams.p;
        for (i = 0; i < carg; i++)
        {
            if (pparam->ref.m_tagk == 0x14)
                pparam = pparam->next.p;
            CRef r = CFrame_AddBinding(spf.p, pparam->ref.u.n, &aref[i]);
            pparam = pparam->next.p;
        }
        ref = EvalLambdaBody((CPair *)proc, spf.p);
        break;
    }
    case 0x22:
        ref = pmeth->m_pfnthunk(pmeth->m_pbasic, c, aref);
        break;
    }
    return ref;
}

// The Splice evaluator's entry point: atoms evaluate to themselves, symbols are looked up, and a list is dispatched on
// its head tag to the special-form evaluators (1 set!, 2 define, 5 assert, 6 form 6, 7 if, 8 or, 9 and, 10 cond,
// 12 case, 13 let, 14 while, 15 lambda, 16 begin, 17 import -> Nil, 18 quote -> datum); any other list is a call.
// Also known as: RefEval (upstream readable / Sly 1 name).
CRef EvalExpression(CPair *expr, CFrame *frame)
{
    switch (expr->ref.m_tagk)
    {
    default:
        return expr->ref;
    case 0x18:
        return EvalSymbol(expr, frame);
    case 0x1F:
    {
        CPair *pn = (CPair *)expr->ref.u.pv;
        switch (pn->ref.m_tagk)
        {
        case 1: return EvalSet(expr, frame);
        case 2: return EvalDefine(expr, frame);
        case 5: return EvalAssert(expr, frame);
        case 6: return EvalSpecialForm6(expr, frame);
        case 7: return EvalIf(expr, frame);
        case 8: return EvalOr(expr, frame);
        case 9: return EvalAnd(expr, frame);
        case 10: return EvalCond(expr, frame);
        case 12: return EvalCase(expr, frame);
        case 13: return EvalLet(expr, frame);
        case 14: return EvalWhile(expr, frame);
        case 16: return EvalBegin(expr, frame);
        case 15: return EvalLambda(expr, frame);
        case 18: return pn->next.p->ref;
        case 17: return CRef(-1);
        }
        return EvalApply(expr, frame);
    }
    }
}

/* The frame and proc pools' GC members are CGcObj<CFrame>'s and CGcObj<CProc>'s (sly2/gc.h), the frame destructor
   (0x257078) and the counted-pointer copies CRefPtr<T>'s (sly2/basic.h), all emitted by splice/gc.cpp. */
