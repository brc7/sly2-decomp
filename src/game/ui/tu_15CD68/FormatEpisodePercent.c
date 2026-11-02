typedef struct CRef { int m_tagk; int m_tag; } CRef;
struct X { char pad[0x110]; int i; };
struct G { char pad[0x3B7C]; void *ap[9]; char pad2[0x4774 - 0x3BA0]; struct X *px; };
extern struct G D_003DD490;
extern char D_003D4A60[];
extern void *D_002BE2F8[];
extern char D_002A5160[];
void RefGetGlobalSaveVar(CRef *pref, void *p, void *q);
void func_00129EC8(CRef *pref);
int FPowerupAcquired(void *p);
void func_0021ECD8(char *pch, char *fmt, ...);
/** @sly2-annot addr=0x15D8F8
 * Purpose: Text callback: '%d%%' of the selected episode's percent (95% scale, +5 if a per-episode flag is
 *   set)
 * Info: proposed name FormatEpisodePercent (T1); TU tu_15CD68
 * Notes: names_v2 v1 proposed ExplodeExplsExplso__FP5EXPLSP6EXPLSO (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r3b_rest
 * See: docs/engine/game-progression.md#6-objective-counters-job-markers-and-the-episode-menu,
 *   docs/reference/functions/hud-ui.md#tu_15cd68-0x15cd68-0x15dc80
 * @end-sly2-annot */
void FormatEpisodePercent(char *pch)
{
    struct G *pg = &D_003DD490;
    void *p;
    if (pg->px && (p = pg->ap[pg->px->i]) != 0) {
        CRef ref;
        int n;
        RefGetGlobalSaveVar(&ref, p, D_003D4A60);
        n = ref.m_tag;
        func_00129EC8(&ref);
        n = n * 95 / 100;
        if (FPowerupAcquired(D_002BE2F8[pg->px->i]))
            n += 5;
        func_0021ECD8(pch, D_002A5160, n);
    }
}
