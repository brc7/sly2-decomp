#!/usr/bin/env python3
"""warp_tour.py [--queue DIR] [--toolkit DIR] [--slot N] [--budget S] L11 [L22 ...] -- capture one PCSX2 save state
per level by warping through them over PINE (one cheap way to collect snapshots covering many classes).

How it works: a PINE bridge running next to PCSX2 watches a job queue (QUEUE/inbox/*.json -> QUEUE/outbox/*.json,
each job a list of ops: read32, write32, savestate_sync, loadstate, copystate...). For each level this script
  1. requests a warp by memory writes (the world's level-request block in g_sw: progression.warp, no menus);
  2. polls the level index, the pause flag and the frame counter until the target level runs (>120 frames);
  3. dismisses the "memory card removed - auto-save disabled" dialog that pauses the game after a load/warp:
     while the game clock's dt reads 0 it holds SQUARE through g_joy (progression.dismiss_memcard_dialog; the PO force
     mask does not reach menus); a pause that survives 3 presses is logged;
  4. savestate_sync into SLOT and copystate to QUEUE/data/lvl_<L>.p2s (game data: never commit; extract RAM with
     savestate.py).
A level that times out triggers a recovery loadstate of SLOT (seed SLOT with any good in-game state first). Progress
is resumable: tried levels go to QUEUE/w/tried.txt, captured ones are skipped; log in QUEUE/w/LOG.txt. --budget caps
the wall time of one invocation (seconds), so a scheduler can call it repeatedly.

Needs the PINE toolkit (jobs.py: Session/QueueRunner/new_id; progression.py: warp/is_paused/
dismiss_memcard_dialog/loadstate), which is not part of this repository: --toolkit or PINE_TOOLKIT. Queue: --queue or
PINE_QUEUE. Level names as progression.py knows them (L01..; `python3 progression.py levels`).
"""
import argparse, json, os, random, sys, time

LEVEL_IDX, G_SW, SW_PAUSED, SW_FRAME = 0x3E1110, 0x3DD490, 0x3BF0, 0x3D60   # level index; g_sw paused flag, frame count


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('levels', nargs='+')
    ap.add_argument('--queue', default=os.environ.get('PINE_QUEUE'), help='bridge job-queue dir [PINE_QUEUE]')
    ap.add_argument('--toolkit', default=os.environ.get('PINE_TOOLKIT'), help='PINE toolkit dir [PINE_TOOLKIT]')
    ap.add_argument('--slot', type=int, default=6, help='save-state slot for capture/recovery (default 6)')
    ap.add_argument('--budget', type=float, default=float(os.environ.get('BUDGET', '160')), help='seconds per run')
    ap.add_argument('--timeout', type=float, default=150, help='seconds to wait for one level to load')
    a = ap.parse_args()
    if not a.queue or not a.toolkit:
        ap.error('--queue and --toolkit (or PINE_QUEUE / PINE_TOOLKIT) are required')
    Q, TK = os.path.abspath(a.queue), os.path.abspath(a.toolkit)
    sys.path.insert(0, TK); os.chdir(TK)
    import jobs as J
    _nid = J.new_id; J.new_id = lambda prefix='job': _nid('w_' + prefix)
    import progression as P
    S = J.Session(J.QueueRunner(root=Q, timeout=90))
    os.makedirs(os.path.join(Q, 'w'), exist_ok=True)

    def log(s):
        open(os.path.join(Q, 'w', 'LOG.txt'), 'a').write(time.strftime('%H:%M:%S ') + s + '\n'); print(s, flush=True)

    def run(ops, tag='w', timeout=60):
        jid = 'w_%s_%s_%04d' % (tag, time.strftime('%H%M%S'), random.randint(0, 9999))
        p = os.path.join(Q, 'inbox', jid + '.json'); json.dump({'id': jid, 'ops': ops}, open(p + '.tmp', 'w'))
        os.rename(p + '.tmp', p)                  # atomic publish: the bridge never sees a partial job
        o = os.path.join(Q, 'outbox', jid + '.json'); t0 = time.time()
        while time.time() - t0 < timeout:
            if os.path.exists(o):
                time.sleep(0.1)
                try:
                    r = json.load(open(o))
                    return [x.get('value') if x.get('ok') else 'ERR:' + str(x.get('error')) for x in r['results']]
                except Exception:
                    pass
            time.sleep(0.2)
        return None

    def poll():
        v = run([{"op": "read32", "addr": LEVEL_IDX}, {"op": "read32", "addr": G_SW + SW_PAUSED},
                 {"op": "read32", "addr": G_SW + SW_FRAME}], 'p', 20)
        return v if v and all(isinstance(x, int) for x in v) else None

    def recover():
        try: log('recover %s' % P.loadstate(S, a.slot))
        except Exception as e: log('recover EXC %r' % e)

    def one(L, tmo):
        il = int(L[1:])
        try: P.warp(S, L, wait=False)
        except Exception as e: log('%s warp EXC %r' % (L, e)); return False
        t0 = time.time(); last = None; v = None; ok = False
        while time.time() - t0 < tmo:
            v = poll()
            if v:
                lv, paused, fr = v
                if lv == il and not paused and last is not None and fr > last and fr > 120: ok = True; break
                last = fr if lv == il else None
            time.sleep(1)
        dt = round(time.time() - t0)
        if not ok: log('%s TIMEOUT %ss last=%s' % (L, dt, v)); return False
        for _ in range(3):
            if not P.is_paused(S): break
            log('%s paused (memcard dialog?) -> SQUARE %s' % (L, P.dismiss_memcard_dialog(S)))
        time.sleep(5)
        if P.is_paused(S): log('%s still paused before save' % L)
        s = run([{"op": "savestate_sync", "slot": a.slot, "timeout": 25},
                 {"op": "copystate", "slot": a.slot, "name": "lvl_%s.p2s" % L},
                 {"op": "read32", "addr": LEVEL_IDX}, {"op": "read32", "addr": G_SW + SW_FRAME}], 's', 90)
        log('%s OK load=%ss save=%s' % (L, dt, s and [s[1] if not isinstance(s[1], dict) else 'copied', s[2], s[3]]))
        return bool(s) and s[2] == il

    T = os.path.join(Q, 'w', 'tried.txt')
    tried = open(T).read().split() if os.path.exists(T) else []
    have = lambda L: os.path.exists(os.path.join(Q, 'data', 'lvl_%s.p2s' % L))
    if tried and not have(tried[-1]) and not os.path.exists(T + '.recovered.' + tried[-1]):
        log('%s killed mid-run -> FAIL, recovering' % tried[-1]); recover(); open(T + '.recovered.' + tried[-1], 'w').close()
    t_end = time.time() + a.budget
    for L in a.levels:
        if have(L) or L in tried: continue
        if time.time() + 25 > t_end: break
        open(T, 'a').write(L + '\n')
        try: good = one(L, min(a.timeout, t_end - time.time()))
        except Exception as e: log('%s EXC %r' % (L, e)); good = False
        if not good: recover(); open(T + '.recovered.' + L, 'w').close()
    tried = open(T).read().split() if os.path.exists(T) else []
    print('left:', [L for L in a.levels if not have(L) and L not in tried])


if __name__ == '__main__':
    main()
