#!/usr/bin/env python3
"""Generates the SVG figures of the editorial (run from this directory)."""
import math

C_EDGE = "#9aa3ad"; C_NODE = "#ffffff"; C_STROKE = "#2d3748"
C_SPINE = "#2b6cb0"; C_BIG = "#2f855a"; C_LEAF = "#a0aec0"; C_HOP = "#dd6b20"; C_BAD = "#c53030"

def svg(w, h, body):
    return (f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {w} {h}" width="{w}" height="{h}" '
            f'font-family="DejaVu Sans, sans-serif">{body}</svg>\n')

def node(x, y, label="", r=13, fill=C_NODE, stroke=C_STROKE, fs=11, sw=1.6):
    t = f'<text x="{x}" y="{y+4}" text-anchor="middle" font-size="{fs}" fill="#1a202c">{label}</text>' if label != "" else ""
    return f'<circle cx="{x}" cy="{y}" r="{r}" fill="{fill}" stroke="{stroke}" stroke-width="{sw}"/>' + t

def edge(a, b, color=C_EDGE, w=2, dash=None):
    d = f' stroke-dasharray="{dash}"' if dash else ""
    return f'<line x1="{a[0]}" y1="{a[1]}" x2="{b[0]}" y2="{b[1]}" stroke="{color}" stroke-width="{w}"{d}/>'

def arrow(a, b, color=C_HOP, shrink=15, w=2.2, bend=0):
    (x1, y1), (x2, y2) = a, b
    dx, dy = x2 - x1, y2 - y1; L = math.hypot(dx, dy)
    ux, uy = dx / L, dy / L
    sx, sy, ex, ey = x1 + ux * shrink, y1 + uy * shrink, x2 - ux * shrink, y2 - uy * shrink
    mx, my = (sx + ex) / 2 - uy * bend, (sy + ey) / 2 + ux * bend
    return (f'<path d="M{sx:.1f},{sy:.1f} Q{mx:.1f},{my:.1f} {ex:.1f},{ey:.1f}" fill="none" stroke="{color}" '
            f'stroke-width="{w}" marker-end="url(#ah)" opacity="0.9"/>')

DEFS = (f'<defs><marker id="ah" viewBox="0 0 10 10" refX="8" refY="5" markerWidth="6" markerHeight="6" '
        f'orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="{C_HOP}"/></marker></defs>')

# ---- fig1: sample 4 (complete binary tree, 15 vertices) with the hop order
def fig1():
    W, H = 640, 260
    pos = {}
    for v in range(15):
        d = (v + 1).bit_length() - 1; i = v + 1 - (1 << d)
        pos[v] = (W * (2 * i + 1) / (2 << d), 35 + d * 62)
    b = DEFS
    for v in range(1, 15): b += edge(pos[v], pos[(v - 1) // 2])
    seq = [7, 8, 3, 1, 9, 10, 4, 0, 5, 11, 12, 2, 14, 6, 13]
    for i in range(14): b += arrow(pos[seq[i]], pos[seq[i + 1]], bend=18)
    for v in range(15): b += node(*pos[v], str(v))
    for k, v in enumerate(seq):
        x, y = pos[v]
        b += f'<circle cx="{x+15}" cy="{y-14}" r="8.5" fill="{C_HOP}"/><text x="{x+15}" y="{y-10.5}" text-anchor="middle" font-size="9.5" fill="#fff" font-weight="bold">{k+1}</text>'
    return svg(W, H, b)

# ---- fig2: two spiders
def spider(cx, cy, legs, leaves, R=58):
    b = ""; pts = []; n = len(legs) + leaves
    for k in range(n):
        ang = -math.pi / 2 + 2 * math.pi * k / n
        L = legs[k] if k < len(legs) else 1
        prev = (cx, cy)
        for j in range(1, L + 1):
            p = (cx + math.cos(ang) * R * j * 0.75, cy + math.sin(ang) * R * j * 0.75)
            b += edge(prev, p, C_BIG if L >= 2 else C_EDGE, 2.4 if L >= 2 else 2)
            pts.append((p, L >= 2)); prev = p
    for p, big in pts: b += node(*p, r=7, fill="#c6f6d5" if big else "#edf2f7", stroke=C_BIG if big else "#718096")
    b += node(cx, cy, r=10, fill="#bee3f8", stroke=C_SPINE)
    return b

def fig2():
    b = spider(170, 135, [2, 2, 2, 2], 3) + spider(470, 135, [2, 2, 2, 2, 2], 2)
    b += '<text x="170" y="262" text-anchor="middle" font-size="15" fill="#2f855a" font-weight="bold">4 legs of length 2  =&gt;  answer 2</text>'
    b += '<text x="470" y="262" text-anchor="middle" font-size="15" fill="#c53030" font-weight="bold">5 legs of length 2  =&gt;  answer 3</text>'
    return svg(640, 275, b)

# ---- fig3: spine with clusters, types 0 / A / C
def caterpillar(x0, y0, h, side, top=None):
    """chain of h vertices hanging below (x0,y0); every chain vertex gets one leaf on `side`"""
    s = ""; prev = top or (x0, y0); chain = []
    for j in range(h):
        p = (x0, y0 + 48 + 40 * j); s += edge(prev, p, C_BIG, 2.6); chain.append(p); prev = p
    for p in chain:
        q = (p[0] + 24 * side, p[1] + 16); s += edge(p, q, C_LEAF, 1.6)
        s += node(*q, r=5, fill="#edf2f7", stroke="#718096", sw=1.2)
    for p in chain: s += node(*p, r=7.5, fill="#c6f6d5", stroke=C_BIG)
    return s

def fig3():
    W, H = 720, 250; y = 60; xs = [50 + 88 * i for i in range(8)]
    types = ["0", "A", "0", "C", "0", "A", "C", "0"]
    b = ""
    for i in range(7): b += edge((xs[i], y), (xs[i + 1], y), C_SPINE, 3.2)
    def leaf(x0, dx):
        q = (x0 + dx, y + 44); return edge((x0, y), q, C_LEAF, 1.6) + node(*q, r=5.5, fill="#edf2f7", stroke="#718096", sw=1.2)
    b += leaf(xs[1], -16) + leaf(xs[1], 16)
    b += caterpillar(xs[3] - 22, y, 3, -1, (xs[3], y)) + caterpillar(xs[3] + 22, y, 2, 1, (xs[3], y)) + leaf(xs[3], 0)
    b += caterpillar(xs[5], y, 2, 1) + leaf(xs[5], -28)
    b += caterpillar(xs[6] - 20, y, 2, -1, (xs[6], y)) + caterpillar(xs[6] + 20, y, 3, 1, (xs[6], y))
    for i, t in enumerate(types):
        col = {"0": "#ffffff", "A": "#fefcbf", "C": "#fed7d7"}[t]
        b += node(xs[i], y, t, r=15, fill=col, stroke=C_SPINE, fs=13, sw=2.2)
    b += f'<text x="{xs[0]}" y="{y-24}" text-anchor="middle" font-size="13" fill="{C_SPINE}" font-weight="bold">u</text>'
    b += f'<text x="{xs[-1]}" y="{y-24}" text-anchor="middle" font-size="13" fill="{C_SPINE}" font-weight="bold">v</text>'
    return svg(W, H, b)

# ---- fig4: a big branch (caterpillar from r) traversed r ... c, then c -> w
def fig4():
    W, H = 560, 200; w = (80, 60); r = (220, 60); c2 = (360, 60); c3 = (490, 60)
    L3 = (490, 150); l2a, l2b = (335, 150), (385, 150); la, lc = (195, 150), (245, 150)
    b = DEFS
    b += edge((10, 60), w, C_SPINE, 3.2, "6,4") + edge(w, (80, 190), C_SPINE, 3.2, "6,4")
    b += edge(w, r, C_BIG, 2.6) + edge(r, c2, C_BIG, 2.6) + edge(c2, c3, C_BIG, 2.6)
    for p, par in [(la, r), (lc, r), (l2a, c2), (l2b, c2), (L3, c3)]: b += edge(par, p, C_LEAF, 1.8)
    order = [r, l2b, l2a, c3, L3, c2, la, lc, w]
    bends = [-22, 14, -26, 14, 26, 18, 14, 30]
    for i in range(len(order) - 1): b += arrow(order[i], order[i + 1], bend=bends[i])
    b += node(*w, "w", r=16, fill="#bee3f8", stroke=C_SPINE, fs=13)
    b += node(*r, "r", r=15, fill="#c6f6d5", stroke=C_BIG, fs=13)
    for p in [c2, c3]: b += node(*p, r=11, fill="#c6f6d5", stroke=C_BIG)
    for p in [la, l2a, l2b, L3]: b += node(*p, r=9, fill="#edf2f7", stroke="#718096")
    b += node(*lc, "c", r=11, fill="#edf2f7", stroke="#718096", fs=12)
    for k, p in enumerate(order[:-1]):
        b += f'<circle cx="{p[0]+13}" cy="{p[1]-14}" r="8" fill="{C_HOP}"/><text x="{p[0]+13}" y="{p[1]-10.5}" text-anchor="middle" font-size="9.5" fill="#fff" font-weight="bold">{k+1}</text>'
    return svg(W, H, b)

for name, f in [("fig1", fig1), ("fig2", fig2), ("fig3", fig3), ("fig4", fig4)]:
    open(name + ".svg", "w").write(f())
print("ok")
