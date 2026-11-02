struct S { char pad[0x80]; char x80[0x48]; int fc8; };
struct B { char ab[0x410]; };
void func_001D57A8(struct B *pb);
void CullSceneRenderList(struct B *pb, void *p);
void RenderSoGlobset(void *p, int a, int b, float f, float g);
/** @sly2-annot addr=0x1B4478
 * Purpose: MS::RenderSelf (vt19): draw globset +0x80 via 0x1D57A8/0x1D5808 path if +0xC8 else
 *   DrawGlobset(0x168638)
 * Info: proposed name RenderMsGlobset (T1); class MS slot19; TU ms
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r5_1AE130
 * See: docs/engine/rendering.md#13-key-functions, docs/reference/functions/rendering.md#ms-0x1b4438-0x1b4728
 * @end-sly2-annot */
void RenderMsGlobset(struct S *p)
{
    if (p->fc8) {
        struct B b;
        func_001D57A8(&b);
        CullSceneRenderList(&b, p->x80);
    } else {
        RenderSoGlobset(p->x80, 0, 0, 1.0f, 1.0f);
    }
}
