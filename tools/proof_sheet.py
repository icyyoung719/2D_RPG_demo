#!/usr/bin/env python3
"""Render a tileset contact sheet with the generated tile classes as coloured borders.

Verification aid for tools/tile_semantics.txt: instead of clicking through Tiled, look at
one image. Border colours:

    red     solid        cyan   water        green   walkable
    blue    empty        grey   no class yet (undecided)
    white dot in the corner = the tile is used by a map in assests/Maps/

Reads the properties that are actually in the tileset json, so the sheet shows what the
engine will see. No third-party dependencies (own PNG decode/encode).
"""
import argparse
import json
import os
import struct
import sys
import zlib

CLASS_COLOR = {
    "solid": (220, 40, 40), "water": (40, 190, 220), "walkable": (60, 190, 60),
    "empty": (60, 100, 230), "none": (150, 150, 150),
}
ZOOM = 2


def read_png(path):
    """Minimal PNG reader: 8-bit, non-interlaced, colour types 0/2/3/4/6."""
    data = open(path, "rb").read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        sys.exit(f"{path}: not a PNG")
    pos, idat, palette = 8, bytearray(), None
    width = height = depth = ctype = None
    while pos < len(data):
        (length,) = struct.unpack(">I", data[pos:pos + 4])
        tag = data[pos + 4:pos + 8]
        body = data[pos + 8:pos + 8 + length]
        pos += 12 + length
        if tag == b"IHDR":
            width, height, depth, ctype, _comp, _filt, interlace = struct.unpack(">IIBBBBB", body)
            if depth != 8 or interlace != 0:
                sys.exit(f"{path}: only 8-bit non-interlaced PNGs (depth={depth}, "
                         f"interlace={interlace})")
        elif tag == b"PLTE":
            palette = body
        elif tag == b"IDAT":
            idat += body
        elif tag == b"IEND":
            break
    raw = zlib.decompress(bytes(idat))
    if width is None or height is None or ctype is None:
        sys.exit(f"{path}: missing IHDR")
    channels = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[ctype]
    stride = width * channels
    out = bytearray(width * height * 4)
    prev = bytearray(stride)
    ip = 0
    for y in range(height):
        ftype = raw[ip]
        ip += 1
        line = bytearray(raw[ip:ip + stride])
        ip += stride
        for i in range(stride):
            a = line[i - channels] if i >= channels else 0
            b = prev[i]
            c = prev[i - channels] if i >= channels else 0
            if ftype == 1:
                line[i] = (line[i] + a) & 0xFF
            elif ftype == 2:
                line[i] = (line[i] + b) & 0xFF
            elif ftype == 3:
                line[i] = (line[i] + ((a + b) >> 1)) & 0xFF
            elif ftype == 4:
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[i] = (line[i] + pr) & 0xFF
        prev = line
        for x in range(width):
            src = x * channels
            dst = (y * width + x) * 4
            if ctype == 6:
                out[dst:dst + 4] = line[src:src + 4]
            elif ctype == 2:
                out[dst:dst + 3] = line[src:src + 3]
                out[dst + 3] = 255
            elif ctype == 0:
                out[dst:dst + 3] = bytes([line[src]] * 3)
                out[dst + 3] = 255
            elif ctype == 4:
                out[dst:dst + 3] = bytes([line[src]] * 3)
                out[dst + 3] = line[src + 1]
            elif ctype == 3:
                pi = line[src] * 3
                out[dst:dst + 3] = palette[pi:pi + 3]
                out[dst + 3] = 255
    return width, height, bytes(out)


def write_png(path, w, h, rgba):
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw += rgba[y * w * 4:(y + 1) * w * 4]

    def chunk(tag, body):
        return (struct.pack(">I", len(body)) + tag + body
                + struct.pack(">I", zlib.crc32(tag + body) & 0xFFFFFFFF))

    open(path, "wb").write(b"\x89PNG\r\n\x1a\n"
                           + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
                           + chunk(b"IDAT", zlib.compress(bytes(raw), 6))
                           + chunk(b"IEND", b""))


def find_child(parent, name):
    for entry in os.listdir(parent):
        if entry.lower() == name.lower():
            return os.path.join(parent, entry)
    return None


def tile_class_full(tile_entry):
    names = {p["name"] for p in tile_entry.get("properties", []) if p.get("value")}
    if "terrain" in names:
        return "water"
    for cls in ("solid", "walkable", "empty"):
        if cls in names:
            return cls
    return "none"


def main():
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--assets", default=os.path.join(root, "SFML_test", "assests"))
    ap.add_argument("--out", default=os.path.join(root, "build", "tile-proof"))
    ap.add_argument("--tileset", default=None, help="only this tileset (name or file stem)")
    ap.add_argument("--only-used", action="store_true",
                    help="render only tiles used by maps in Maps/")
    ap.add_argument("--zoom", type=int, default=ZOOM, help="pixel zoom per tile (default 2)")
    args = ap.parse_args()
    zoom = max(1, args.zoom)

    maps_dir = find_child(args.assets, "maps")
    world_dir = find_child(args.assets, "world")
    if maps_dir is None or world_dir is None:
        sys.exit(f"expected Maps/ and World/ directories under {args.assets}")
    used = {}
    meta_cache = {}
    for entry in sorted(os.listdir(maps_dir)):
        if not entry.lower().endswith(".json"):
            continue
        m = json.load(open(os.path.join(maps_dir, entry), encoding="utf-8"))
        for ref in m.get("tilesets", []):
            stem = os.path.splitext(os.path.basename(ref["source"]))[0]
            if stem not in meta_cache:
                ts_file = next((f for f in os.listdir(world_dir)
                                if os.path.splitext(f)[0].lower() == stem.lower()), None)
                meta_cache[stem] = (json.load(open(os.path.join(world_dir, ts_file),
                                                   encoding="utf-8"))["tilecount"]
                                    if ts_file else 0)
            count_tiles = meta_cache[stem]
            for layer in m.get("layers", []):
                if layer.get("type") != "tilelayer":
                    continue
                for gid in layer.get("data", []):
                    local = gid - ref["firstgid"]
                    if gid and 0 <= local < count_tiles:
                        used.setdefault(stem, set()).add(local)

    os.makedirs(args.out, exist_ok=True)
    count = 0
    for fname in sorted(os.listdir(world_dir)):
        if not fname.lower().endswith(".json"):
            continue
        path = os.path.join(world_dir, fname)
        ts = json.load(open(path, encoding="utf-8"))
        name = ts.get("name") or os.path.splitext(fname)[0]
        if args.tileset and args.tileset.lower() not in (name.lower(),
                                                         os.path.splitext(fname)[0].lower()):
            continue
        stem = os.path.splitext(fname)[0]
        used_ids = used.get(stem, set())
        image = find_child(world_dir, ts["image"])
        if image is None:
            print(f"{name}: image {ts['image']} not found, skipped")
            continue
        iw, ih, img = read_png(image)
        cols, count_tiles = ts["columns"], ts["tilecount"]
        tile_w, tile_h = ts["tilewidth"], ts["tileheight"]
        classes = {t["id"]: tile_class_full(t) for t in ts.get("tiles", [])}

        ids = sorted(used_ids) if args.only_used else list(range(count_tiles))
        cw = ch = tile_w * zoom + 3
        W, H = cols * cw, ((len(ids) + cols - 1) // cols) * ch
        canvas = bytearray(W * H * 4)
        for y in range(H):
            for x in range(W):
                v = 205 if ((x // 8) + (y // 8)) % 2 else 175
                canvas[(y * W + x) * 4:(y * W + x) * 4 + 4] = bytes((v, v, v, 255))

        tally = {}
        for slot, tid in enumerate(ids):
            cls = classes.get(tid, "none")
            tally[cls] = tally.get(cls, 0) + 1
            bx, by = (slot % cols) * cw + 2, (slot // cols) * ch + 2
            sx, sy = (tid % cols) * tile_w, (tid // cols) * tile_h
            for yy in range(tile_h * zoom):
                line = ((sy + yy // zoom) * iw + sx) * 4
                for xx in range(tile_w * zoom):
                    si = line + (xx // zoom) * 4
                    if img[si + 3] == 0:
                        continue
                    di = ((by + yy) * W + bx + xx) * 4
                    canvas[di:di + 4] = img[si:si + 4]
            col = CLASS_COLOR[cls]
            for xx in range(-1, tile_w * zoom + 1):
                for yy in (-1, tile_h * zoom):
                    px, py = bx + xx, by + yy
                    if 0 <= px < W and 0 <= py < H:
                        canvas[(py * W + px) * 4:(py * W + px) * 4 + 4] = bytes(col + (255,))
            if tid in used_ids:            # white dot marks "used by a map"
                for yy in range(3):
                    for xx in range(3):
                        di = ((by + yy + 1) * W + bx + xx + 1) * 4
                        canvas[di:di + 4] = bytes((255, 255, 255, 255))

        out = os.path.join(args.out, f"{stem}_proof.png")
        write_png(out, W, H, bytes(canvas))
        print(f"{name}: {len(ids)} tiles -> {out}")
        print(f"    classes: " + ", ".join(f"{k}={v}" for k, v in sorted(tally.items()))
              + f"   (used by maps: {len(used_ids & set(range(count_tiles)))})")
        count += 1
    if not count:
        sys.exit("no tileset matched")
    print(f"\nborder colours: red=solid, cyan=water, green=walkable, blue=empty, "
          f"grey=no class (undecided); white dot = used by a map")
    return 0


if __name__ == "__main__":
    sys.exit(main())
