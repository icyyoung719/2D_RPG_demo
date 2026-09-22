#!/usr/bin/env python3
"""Write per-tile properties into the Tiled tilesets from tools/tile_semantics.txt.

By default only tiles that a map in assests/Maps/ actually uses are classified, so the
generated json stays small and reviewable. --all classifies every tile that has a rule.

The tileset json files are rewritten in a stable style (indent 1, compact separators);
"tiles" entries the generator does not manage (animations, object groups, hand-added
properties) are preserved. --check compares semantics, not bytes, so a reformat by Tiled
does not fail the check, but a semantic difference does.
"""
import argparse
import json
import os
import re
import sys

MANAGED = ("solid", "terrain", "walkable", "empty")
RANK = {"water": 3, "solid": 2, "walkable": 1, "empty": 0}
CLASSES = set(RANK) | {"?"}


def repo_root():
    return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def find_child(parent, name):
    """Case-insensitive lookup of a single file/dir inside parent (repo paths are mixed case)."""
    if not os.path.isdir(parent):
        return None
    for entry in os.listdir(parent):
        if entry.lower() == name.lower():
            return os.path.join(parent, entry)
    return None


def resolve_path(base, rel):
    """Resolve a Tiled-relative path case-insensitively."""
    cur = base
    for part in rel.replace("\\", "/").split("/"):
        if part in ("", "."):
            continue
        if part == "..":
            cur = os.path.dirname(cur)
            continue
        nxt = find_child(cur, part)
        if nxt is None:
            return None
        cur = nxt
    return cur


class Rules:
    def __init__(self):
        self.wang = {}
        self.anim = {}
        self.ranges = []
        self.tiles = {}
        self.anim_inherit = True
        self.matched = set()
        self.stale = set()

    def new_section(self):
        self.wang, self.anim, self.ranges, self.tiles = {}, {}, [], {}
        self.anim_inherit = True


def parse_rules(path):
    sections, current, name = {}, None, None
    for lineno, raw in enumerate(open(path, encoding="utf-8"), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        m = re.fullmatch(r"\[tileset\s+(.+?)\]", line, re.I)
        if m:
            if name:
                sections[name] = current
            name, current = m.group(1).strip(), Rules()
            continue
        if current is None:
            sys.exit(f"{path}:{lineno}: directive before any [tileset ...] section")
        key, _, value = line.partition("=")
        key, value = key.strip().lower(), value.strip()
        if not key or not value:
            sys.exit(f"{path}:{lineno}: expected 'KEY = VALUE'")
        if value not in CLASSES:
            sys.exit(f"{path}:{lineno}: unknown class {value!r} (want one of {sorted(CLASSES)})")
        if key.startswith("wang "):
            current.wang[key[5:].strip()] = value
        elif key.startswith("anim "):
            current.anim[int(key[5:].strip())] = value
        elif key.startswith("tile "):
            for part in key[5:].split(","):
                current.tiles[int(part.strip())] = value
        elif key.startswith("range "):
            m2 = re.fullmatch(r"range\s+r(\d+)(?:\s*-\s*r?(\d+))?\s+c(\d+)(?:\s*-\s*c?(\d+))?",
                              key)
            if not m2:
                sys.exit(f"{path}:{lineno}: expected 'range rA-rB cC-cD = CLASS'")
            r1, r2, c1, c2 = m2.groups()
            current.ranges.append((int(r1), int(r2 or r1), int(c1), int(c2 or c1), value))
        elif key == "animation-inherit":
            current.anim_inherit = value.lower() in ("yes", "true", "on", "1")
        else:
            sys.exit(f"{path}:{lineno}: unknown directive {key!r}")
    if name:
        sections[name] = current
    return sections


def load_maps(assets):
    maps_dir = find_child(assets, "maps")
    if not maps_dir:
        sys.exit(f"no Maps/ directory under {assets}")
    out = []
    for entry in sorted(os.listdir(maps_dir)):
        if entry.lower().endswith(".json"):
            path = os.path.join(maps_dir, entry)
            out.append((path, json.load(open(path, encoding="utf-8"))))
    return out


def tileset_index(assets, maps):
    """tileset key -> dict(json path, data, firstgid, used tiles)."""
    index = {}
    for map_path, m in maps:
        map_dir = os.path.dirname(map_path)
        for ref in m.get("tilesets", []):
            if "source" not in ref:
                sys.exit(f"{map_path}: embedded tilesets are not supported by this tool")
            ts_path = resolve_path(map_dir, ref["source"])
            if ts_path is None:
                sys.exit(f"{map_path}: cannot resolve tileset source {ref['source']!r}")
            data = json.load(open(ts_path, encoding="utf-8"))
            key = data.get("name") or os.path.splitext(os.path.basename(ts_path))[0]
            entry = index.setdefault(key, {"path": ts_path, "data": data,
                                           "firstgid": ref["firstgid"], "used": {}})
            if entry["firstgid"] != ref["firstgid"]:
                sys.exit(f"{key}: conflicting firstgid "
                         f"({entry['firstgid']} vs {ref['firstgid']} in {map_path})")
        for layer in m.get("layers", []):
            if layer.get("type") != "tilelayer":
                continue
            for gid in layer.get("data", []):
                if not gid:
                    continue
                owner = owner_of(index, gid)
                if owner is None:
                    sys.exit(f"{os.path.basename(map_path)}:{layer.get('name')}: gid {gid} "
                             f"belongs to no known tileset")
                owner["used"].setdefault(gid - owner["firstgid"], set()).add(
                    f"{os.path.basename(map_path)}:{layer.get('name')}")
    return index


def owner_of(index, gid):
    best = None
    for entry in index.values():
        local = gid - entry["firstgid"]
        if 0 <= local < entry["data"]["tilecount"]:
            if best is None or entry["firstgid"] > best["firstgid"]:
                best = entry
    return best


def wang_classes(entry):
    """tile id -> (class, description) derived from every wang set in the tileset."""
    out = {}
    data = entry["data"]
    for ws in data.get("wangsets", []):
        colors = [c["name"] for c in ws.get("colors", [])]
        for wt in ws.get("wangtiles", []):
            present = sorted({colors[v - 1] for v in wt["wangid"] if 0 < v <= len(colors)})
            if present:
                out[wt["tileid"]] = present
    return out


def classify(entry, rules):
    """tile id -> (class, source). rules.matched collects what a rule hit."""
    data = entry["data"]
    cols = data["columns"]
    count = data["tilecount"]
    wang = wang_classes(entry)
    order = list(range(count))
    classes, source = {}, {}

    def assign(tid, cls, src, rule_tag=None):
        if not 0 <= tid < count:
            return False
        classes[tid], source[tid] = cls, src
        if rule_tag:
            rules.matched.add(rule_tag)
        return True

    for rid in order:
        present = wang.get(rid)
        if not present:
            continue
        ranked = [(RANK[rules.wang[c]], rules.wang[c], c) for c in present if c in rules.wang]
        if not ranked:
            continue
        ranked.sort(reverse=True)
        cls = ranked[0][1]
        tag = None if cls == "?" else f"wang {'+'.join(c for _r, _cl, c in ranked)}"
        assign(rid, cls, "wang " + "+".join(c for _r, _cl, c in ranked), tag)

    # explicit anim rules (a base tile plus every frame of its animation)
    anim_of = {t["id"]: [f["tileid"] for f in t.get("animation", [])]
               for t in data.get("tiles", []) if "animation" in t}
    for base, cls in rules.anim.items():
        hit = assign(base, cls, f"anim {base}", f"anim {base}")
        for frame in anim_of.get(base, []):
            assign(frame, cls, f"anim {base}", f"anim {base}")
        if not hit:
            rules.stale.add(f"anim {base}")

    for r1, r2, c1, c2, cls in rules.ranges:
        tag = f"range r{r1}-r{r2} c{c1}-c{c2}"
        for row in range(r1, r2 + 1):
            for col in range(c1, c2 + 1):
                assign(row * cols + col, cls, tag, tag)

    for tid, cls in rules.tiles.items():
        assign(tid, cls, f"tile {tid}", f"tile {tid}")

    if rules.anim_inherit:
        for base, frames in anim_of.items():
            if base not in classes:
                continue
            for frame in frames:
                if frame not in classes:
                    classes[frame] = classes[base]
                    source[frame] = f"animation of {base}"
    return classes, source


def props_for(cls):
    if cls == "solid":
        return [{"name": "solid", "type": "bool", "value": True}]
    if cls == "water":
        return [{"name": "solid", "type": "bool", "value": True},
                {"name": "terrain", "type": "string", "value": "water"}]
    if cls == "walkable":
        return [{"name": "walkable", "type": "bool", "value": True}]
    if cls == "empty":
        return [{"name": "empty", "type": "bool", "value": True}]
    return []


def managed_props(tile_entry):
    return {p["name"]: p.get("value") for p in tile_entry.get("properties", [])
            if p.get("name") in MANAGED}


def expected_props(entry, rules, only_used):
    classes, source = classify(entry, rules)
    ids = sorted(entry["used"]) if only_used else sorted(classes)
    out = {}
    for tid in ids:
        out[tid] = (props_for(classes.get(tid, "?")), classes.get(tid, "?"), source.get(tid, "-"))
    return out, classes, source


def apply(entry, expected):
    """Merge expected properties into the tileset json, preserving foreign data."""
    data = entry["data"]
    existing = {t["id"]: dict(t) for t in data.get("tiles", []) if "id" in t}
    changed = []
    for tid, (props, _cls, _src) in expected.items():
        if not props:
            continue
        tile = existing.setdefault(tid, {"id": tid})
        foreign = [p for p in tile.get("properties", []) if p.get("name") not in MANAGED]
        new_props = props + foreign
        if tile.get("properties") != new_props:
            changed.append(tid)
        tile["properties"] = new_props
    for tid in [tid for tid, tile in existing.items() if set(tile) == {"id"}]:
        del existing[tid]
    data["tiles"] = [existing[tid] for tid in sorted(existing)]
    return changed


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true", help="verify, change nothing")
    ap.add_argument("--all", action="store_true",
                    help="classify every tile with a rule, not just the ones maps use")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--assets", default=None, help="path to the assests directory")
    default_assets = os.path.join(repo_root(), "SFML_test", "assests")
    args = ap.parse_args()

    assets = args.assets or default_assets
    rules_path = os.path.join(repo_root(), "tools", "tile_semantics.txt")
    sections = parse_rules(rules_path)
    maps = load_maps(assets)
    index = tileset_index(assets, maps)

    for key in sections:
        if key not in index:
            print(f"warning: [tileset {key}] matches no tileset used by the maps")
    for key in index:
        if key not in sections:
            print(f"warning: tileset {key} has no [tileset ...] section")

    problems, unknown, summaries = [], [], []
    for key, entry in sorted(index.items()):
        rules = sections.get(key)
        if rules is None:
            continue
        expected, classes, source = expected_props(entry, rules, only_used=not args.all)
        counts = {}
        for tid, (props, cls, src) in expected.items():
            counts[cls] = counts.get(cls, 0) + 1
            if not props:
                unknown.append((key, tid, src, entry["used"].get(tid, set())))
        summaries.append((key, entry, expected, classes, source, counts))

    if unknown:
        print(f"\n{len(unknown)} used tile(s) have no class yet:")
        for key, tid, src, where in unknown:
            entry = index[key]
            print(f"  {key} tile {tid} (col {tid % entry['data']['columns']} "
                  f"row {tid // entry['data']['columns']}) -> add a rule in "
                  f"tools/tile_semantics.txt  [{', '.join(sorted(where))}]")

    for key, entry, expected, _classes, _source, counts in summaries:
        print(f"\n=== {key} ({entry['path']}) ===")
        print(f"    classified {len(expected)} tile(s): "
              + ", ".join(f"{c}={n}" for c, n in sorted(counts.items())))
        if args.check:
            tiles = {t["id"]: t for t in entry["data"].get("tiles", [])}
            for tid, (props, cls, src) in sorted(expected.items()):
                actual = managed_props(tiles.get(tid, {}))
                want = {p["name"]: p.get("value") for p in props}
                if actual != want:
                    problems.append(f"{key} tile {tid} ({cls}, {src}): json has "
                                    f"{actual or '{}'}, expected {want or '{}'}")
            for tid, tile in tiles.items():
                actual = managed_props(tile)
                if actual and tid not in expected:
                    problems.append(f"{key} tile {tid}: json has {actual} but no rule "
                                    f"produces it (stale or hand-edited)")
        else:
            changed = apply(entry, expected)
            if changed:
                print(f"    updated properties on {len(changed)} tile(s)")
            if not args.dry_run:
                with open(entry["path"], "w", encoding="utf-8") as fh:
                    json.dump(entry["data"], fh, indent=1, separators=(",", ":"),
                              ensure_ascii=False)
                    fh.write("\n")

    for key, rules in sorted(sections.items()):
        if rules.stale:
            print(f"warning: [tileset {key}] rule(s) matched no tile: "
                  f"{', '.join(sorted(rules.stale))}")

    if args.check:
        if problems:
            print(f"\nFAIL: {len(problems)} propert{'y' if len(problems) == 1 else 'ies'} "
                  f"out of date:")
            for p in problems[:40]:
                print(f"  {p}")
            if len(problems) > 40:
                print(f"  ... and {len(problems) - 40} more")
            print("\nrun: python3 tools/gen_tile_props.py")
            return 1
        if unknown:
            print("\nFAIL: every tile used by a map needs a class (see list above)")
            return 1
        print("\nOK: generated tile properties match tools/tile_semantics.txt")
    return 0


if __name__ == "__main__":
    sys.exit(main())
