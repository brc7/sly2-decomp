typedef int qword __attribute__((mode(TI)));
class B {
public:
    int a, b, c;
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual qword Get33();
    virtual qword Get34();
    virtual void Set35(qword);
    virtual void Set36(qword);
};
class X : public B {
public:
    char pad[0xE8 - 0x10];
    void *pE8;
};
extern "C" void func_001A6B88(X *, X *);
/** @sly2-annot addr=0x1F49C8
 * Purpose: SO vt38: LO::MatchLoXf then copy velocities via vt33..36 when SO has physics
 * Info: proposed name MatchSoXf (T2); class SO slot38; TU so
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
extern "C" void func_001F49C8(X *p, X *q)
{
    func_001A6B88(p, q);
    if (p->pE8) {
        p->Set35(q->Get33());
        p->Set36(q->Get34());
    }
}
