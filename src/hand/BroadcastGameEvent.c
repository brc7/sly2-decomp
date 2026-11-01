struct X { char pad[0x7C]; void *f7c; char pad2[0x88 - 0x80]; struct X *pNext; };
struct G { char pad[0x3B0C]; void *f3b0c; };
extern struct G D_003DD490;
extern struct X *D_003E0F90;
extern char D_002DE578[];
void HandleGameEvent(void *pv, int n, void *p);
void HandleDifficultyMessage(void *pv, int n, void *p);
void SendTaskGameEvent(struct X *px, int n, void *p);
void HandleJobGameEvent(void *pv, int n, void *p);
/** @sly2-annot addr=0x1619A0
 * Purpose: Game event bus: HandleGameEvent(g_game), difficulty 0x154A90, every AVAILABLE task (list
 *   g_sw+0x3B00) of the current job (SendTaskGameEvent), then the current job (HandleJobGameEvent)
 * Info: proposed name BroadcastGameEvent (T1); TU game
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r3b_rest+verdicts_followups
 * See: docs/engine/game-progression.md#2-the-game-event-bus, docs/engine/game-progression.md#8-key-functions,
 *   docs/engine/game-progression.md#open-questions
 * @end-sly2-annot */
void BroadcastGameEvent(void *pv, int n, void *p)
{
    struct X *px;
    HandleGameEvent(pv, n, p);
    HandleDifficultyMessage(D_002DE578, n, p);
    for (px = D_003E0F90; px; px = px->pNext) {
        if (px->f7c == D_003DD490.f3b0c)
            SendTaskGameEvent(px, n, p);
    }
    if (D_003DD490.f3b0c)
        HandleJobGameEvent(D_003DD490.f3b0c, n, p);
}
