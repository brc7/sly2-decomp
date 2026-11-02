struct H { int id; void *table; };
struct DIRECTION_STATE;
struct MARKER;
union PAYLOAD {
    int oid;
    struct H *curvature;
    struct DIRECTION_STATE *direction;
    void *resource;
};
struct E { int kind; union PAYLOAD value; int n8, nC; };
extern char D_003DD490[];
void func_0020BD88(struct H *, void *);
void func_0020C7E0(struct DIRECTION_STATE *, void *);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *, int, int, void *);
int CidBasic(void *);
struct MARKER *func_0012F938(void *, int);
struct H *func_0020C010(int);
void func_0020BD78(struct H *);

/** @sly2-annot addr=0x209130
 * Purpose: Resolve oid -> ASEG (cid 0x6D; kind 2 wrapper if it has label 0x22) or SM (cid 0x6F, kind 1); else
 *   -1
 * Info: proposed name ResolveAnimBinding (T1); TU stepguard
 * Notes: names_v2 C proposed SetPrsenSensors__FP5PRSEN7SENSORS (0.181)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r7b_guard
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void ResolveAnimBinding(struct E *e, void *context)
{
    void *resource;
    if (e->kind != 2) {
        if (e->kind == 3) {
            func_0020C7E0(e->value.direction, context);
            return;
        }
    } else {
        func_0020BD88(e->value.curvature, context);
        return;
    }
    resource = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, e->value.oid, context);
    if (!resource) {
        e->kind = -1;
        return;
    }
    switch (CidBasic(resource)) {
    case 0x6D:
        if (func_0012F938(resource, 0x22)) {
            e->kind = 2;
            e->value.curvature = func_0020C010(0x40);
            func_0020BD78(e->value.curvature);
            e->value.curvature->table = resource;
            func_0020BD88(e->value.curvature, context);
            if (e->nC == -1)
                e->nC = 0;
        } else {
            e->value.resource = resource;
            e->kind = 0;
        }
        break;
    case 0x6F:
        e->kind = 1;
        e->value.resource = resource;
        if (e->nC == -1)
            e->nC = 0;
        break;
    default:
        e->kind = -1;
        break;
    }
}
