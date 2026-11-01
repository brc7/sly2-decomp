// func_001B72B0: rotation (quaternion) and position of `stop` relative to the owner's two end nodes of p->link.
//  - No pose: transform the input basis into the right parent's frame (ConvertXfMat) and take its rotation
//    (func_0011BFF0); transform the input position through the left parent (func_0022FC48).
//  - With a pose: collect the parent chain from pose->first up to `stop` (at most 16 nodes) and the indices of the
//    owner's left/right nodes in it, build the relative frame replace = inverse(from * pose * inverse(input) * to)
//    (rotation re-orthonormalised through the quaternion, position = the right node's local position), then
//    accumulate the chain with `replace` substituted for the right node and output the final position.
//
// How the match was found (wave-4 lane 3, from w3lane6.v2 at aligned 16):
//  - Matrix products by value with const-reference operands, nested as written in one expression
//    (`M4 result = Mul(Mul(from, inverse), to)`): the outer call's return slot address is put in a register before the
//    inner call is expanded, so gcse meets &result before the inner temporary (the game's PRE copy order).
//  - The inverse helper uses an output pointer (`void InvertMatrix4Rigid(M4 *, const M4 *)`), so `replace` / `product` are
//    declared first and land in the slots the game uses.
//  - VU_VECTOR by-value parameters/results with an implicit qword constructor: their 16-byte register copies survive cse1
//    and grow the RTL to 272 insns before gcse (137 hash buckets instead of 131), which gives the game's spill-slot order of
//    the PRE address pseudos; they vanish from the final code.
//  - `P *const p`: the spilled pseudo is RTX_UNCHANGING, so reorg sees the reload of p after the chain loop as redundant and
//    the loop guard branches past it, and sched2 hoists the then-branch reload above the rotation store.
//  - LoadRotateMatrixEuler returns void (its result is never used); M4 *pmat = &replace for the basis copy and position.w gives the
//    mixed sp/s4 addressing (cse1 folds (mem (reg)) of a frame address but not (mem (plus reg c))).
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f[4]; int i[4]; } U;
typedef struct { U row[3]; } M3;
typedef struct { M3 basis; U position; } M4;
struct VU_VECTOR { qword q; VU_VECTOR(const qword &v) { q = v; } operator qword() const { return q; } };
typedef struct NODE { M4 local, world; char pad80[0x20]; struct NODE *parent; } NODE;
typedef struct { char pad[0x58]; NODE *left, *right; } OWNER;
typedef struct { char pad[0x50]; NODE *first; char pad54[0xC]; M4 transform; } POSE;
typedef struct { char pad[0x180]; OWNER *owner; int pad184; POSE *pose; } LINK;
typedef struct { char pad[0x13F0]; LINK *link; } P;
extern "C" M4 D_002A2D30;   // identity matrix
extern "C" M3 ConvertXfMat(NODE *, NODE *, const M3 &);
extern "C" M4 func_0022FF40(NODE *, NODE *, const M4 &);
extern "C" VU_VECTOR func_0022FC48(NODE *, NODE *, VU_VECTOR);
extern "C" VU_VECTOR func_0011BFF0(const M3 *);
extern "C" void LoadRotateMatrixEuler(M3 *, VU_VECTOR);
extern "C" qword *func_00256D18(qword *, VU_VECTOR);
extern "C" void InvertMatrix4Rigid(M4 *, const M4 *);
extern "C" M4 __ml__FRC7MATRIX4T0(const M4 &, const M4 &);

/** @sly2-annot addr=0x1B72B0
 * Purpose: Rotation and position of the carried object relative to the owner's two grip nodes (with optional
 *   pose chain)
 * Info: proposed name GetMurrayGripFrame (T2); TU murray
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
extern "C" void func_001B72B0(P *const p, NODE *stop, const M4 *input, VU_VECTOR *outRotation, VU_VECTOR *outPosition)
{
    if (!p->link->pose) {
        M3 basis;
        basis = ConvertXfMat(stop, p->link->owner->right->parent, input->basis);
        *outRotation = func_0011BFF0(&basis);
        *outPosition = func_0022FC48(stop, p->link->owner->left->parent, input->position.q);
        return;
    }
    {
        NODE *chain[16];
        U rotation, finalPosition;

        OWNER *owner = p->link->owner;
        NODE *node;
        int count = 0, left = -1, right = -1, j;
        M4 inverse;
        InvertMatrix4Rigid(&inverse, input);
        for (node = p->link->pose->first; node != stop; node = node->parent) {
            chain[count] = node;
            if (node == owner->left) left = count;
            if (node == owner->right) right = count;
            count++;
        }
        M4 from = func_0022FF40(p->link->pose->first, owner->right, D_002A2D30);
        M4 to = func_0022FF40(owner->right->parent, stop, D_002A2D30);
        from = __ml__FRC7MATRIX4T0(from, p->link->pose->transform);
        M4 replace;
        M4 result = __ml__FRC7MATRIX4T0(__ml__FRC7MATRIX4T0(from, inverse), to);
        InvertMatrix4Rigid(&replace, &result);
        rotation.q = func_0011BFF0(&replace.basis);
        LoadRotateMatrixEuler(&result.basis, rotation.q);
        func_00256D18(&replace.position.q, chain[right]->local.position.q);
        M4 *pmat = &replace;
        pmat->basis = result.basis;
        replace.basis.row[0].i[3] = replace.basis.row[1].i[3] = replace.basis.row[2].i[3] = 0;
        pmat->position.f[3] = 1.0f;
        result = D_002A2D30;
        M4 upper = D_002A2D30;
        for (int i = count - 1; i > left; i--) {
            if (i == right) result = __ml__FRC7MATRIX4T0(result, replace);
            else result = __ml__FRC7MATRIX4T0(result, chain[i]->local);
        }
        for (j = left - 1; j >= 0; j--) {
            if (j == right) upper = __ml__FRC7MATRIX4T0(upper, replace);
            else upper = __ml__FRC7MATRIX4T0(upper, chain[j]->local);
        }
        upper = __ml__FRC7MATRIX4T0(upper, p->link->pose->transform);
        M4 product;
        M4 intermediate = __ml__FRC7MATRIX4T0(__ml__FRC7MATRIX4T0(upper, inverse), result);
        InvertMatrix4Rigid(&product, &intermediate);
        finalPosition = product.position;
        if (outRotation) *outRotation = rotation.q;
        if (outPosition) *outPosition = finalPosition.q;
    }
}
