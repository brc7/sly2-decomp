struct E { int pad[5]; float g; };
float GRandInRange__Fff(float, float);
/** @sly2-annot addr=0x1F0AD0
 * Purpose: Weighted random pick: GRandInRange(0, ? gProbability) then cumulative walk
 * Info: proposed name PsmtChooseWeighted (T1); TU sm
 * Confidence: mid (draft) [T1 0.70] - evidence: semantic review; sources: semantic_tu_docs/sm
 * See: docs/reference/functions/animation.md#sm-0x1efa90-0x1f1768
 * @end-sly2-annot */
struct E *PsmtChooseWeighted(void *p, int n, struct E **ape)
{
    float sum = 0.0f;

    int i;
    for (i = 0; i < n; i++) sum += ape[i]->g;
    sum = GRandInRange__Fff(0.0f, sum);
    for (i = 0; i < n; i++) {
        sum -= ape[i]->g;
        if (sum <= 0.0f) return ape[i];
    }
    return 0;
}
