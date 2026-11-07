# `src/game/`

Game code, grouped by layer, lowest first: `platform` (1), `core` (2), `splice` (3), `object` (4), `world` (5),
`engine` (6), `level` (7), `actor` (8), `gameplay` (9), `ui` (10), `app` (11). Each file is one original translation
unit. Lower layers must not depend on higher ones; the layer rules, the known exceptions and a one-line description
of every directory are in [docs/architecture.md](../../docs/architecture.md); how the engine fits together is in
[docs/engine.md](../../docs/engine.md).
