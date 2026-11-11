#include "Segment.cpp"

// Cuts a polygon by line r, keeping only the side where ccw(r.p, r.q, .) holds.
// Works for convex polygons in general; also correct for simple ones. O(n)
vector<pt> cut_polygon(vector<pt> v, line r) {
	vector<pt> ret;
	for (int j = 0; j < (int)v.size(); j++) {
		if (ccw(r.p, r.q, v[j])) ret.push_back(v[j]);
		if (v.size() == 1) continue;
		line s(v[j], v[(j+1)%v.size()]);
		pt p = inter(r, s);
		if (isinseg(p, s)) ret.push_back(p);
	}
	ret.erase(unique(ret.begin(), ret.end()), ret.end());
	if (ret.size() > 1 and ret.back() == ret[0]) ret.pop_back();
	return ret;
}

// Distance between two axis-aligned rectangles, given as (bottom-left, top-right)
ld dist_rect(pair<pt, pt> a, pair<pt, pt> b) {
	ld hor = 0, vert = 0;
	if (a.second.x < b.first.x) hor = b.first.x - a.second.x;
	else if (b.second.x < a.first.x) hor = a.first.x - b.second.x;
	if (a.second.y < b.first.y) vert = b.first.y - a.second.y;
	else if (b.second.y < a.first.y) vert = a.first.y - b.second.y;
	return dist(pt(0, 0), pt(hor, vert));
}

// Area of a simple polygon (Shoelace formula)
ld polarea(vector<pt> v) {
	ld ret = 0;
	for (int i = 0; i < (int)v.size(); i++)
		ret += sarea(pt(0, 0), v[i], v[(i + 1) % v.size()]);
	return abs(ret);
}

// Point-in-polygon test. Returns 0 outside, 1 inside, 2 on the border. O(n)
int inpol(vector<pt>& v, pt p) {
	int qt = 0;
	for (int i = 0; i < (int)v.size(); i++) {
		if (p == v[i]) return 2;
		int j = (i+1)%v.size();
		if (eq(p.y, v[i].y) and eq(p.y, v[j].y)) {
			if ((v[i]-p)*(v[j]-p) < eps) return 2;
			continue;
		}
		bool baixo = v[i].y+eps < p.y;
		if (baixo == (v[j].y+eps < p.y)) continue;
		auto t = (p-v[i])^(v[j]-v[i]);
		if (eq(t, 0)) return 2;
		if (baixo == (t > eps)) qt += baixo ? 1 : -1;
	}
	return qt != 0;
}

// Point-in-triangle test (border counts). A specialized, slightly cheaper
// alternative to inpol() when the polygon is known to be a triangle.
bool intriangle(pt p, pt a, pt b, pt c) {
	if (sarea(a, b, c) < -eps) swap(a, b); // make a,b,c ccw
	if (isinseg(p, line(a, b))) return 1;
	if (isinseg(p, line(b, c))) return 1;
	if (isinseg(p, line(c, a))) return 1;
	bool x = !ccw(a, b, p);
	bool y = !ccw(b, c, p);
	bool z = !ccw(c, a, p);
	return x == y and y == z;
}

// True if simple polygons v1 and v2 intersect (touching counts). O(n*m)
bool interpol(vector<pt> v1, vector<pt> v2) {
	int n = v1.size(), m = v2.size();
	for (int i = 0; i < n; i++) if (inpol(v2, v1[i])) return 1;
	for (int i = 0; i < n; i++) if (inpol(v1, v2[i])) return 1;
	for (int i = 0; i < n; i++) for (int j = 0; j < m; j++)
		if (interseg(line(v1[i], v1[(i+1)%n]), line(v2[j], v2[(j+1)%m]))) return 1;
	return 0;
}

// Minimum distance between polygons v1 and v2 (0 if they intersect)
ld distpol(vector<pt> v1, vector<pt> v2) {
	if (interpol(v1, v2)) return 0;

	ld ret = DINF;

	for (int i = 0; i < (int)v1.size(); i++) for (int j = 0; j < (int)v2.size(); j++)
		ret = min(ret, distseg(line(v1[i], v1[(i + 1) % v1.size()]),
		                       line(v2[j], v2[(j + 1) % v2.size()])));
	return ret;
}

// Center of mass (centroid) of a simple polygon. O(n)
pt polygonCenter(const vector<pt>& v) {
	pt res(0, 0); ld A = 0;
	for (int i = 0, j = (int)v.size()-1; i < (int)v.size(); j = i++) {
		res = res + (v[i]+v[j]) * (v[j]^v[i]);
		A += v[j] ^ v[i];
	}
	return res / A / 3;
}

// Convex hull of a point set, returned in ccw order. O(n log n)
// The hull itself must not contain collinear points on its edges.
// Change !ccw to ccw if you want to include collinear points on its edges
vector<pt> convex_hull(vector<pt> v) {
	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end());
	if (v.size() <= 1) return v;
	vector<pt> l, u;
	for (int i = 0; i < (int)v.size(); i++) {
		while (l.size() > 1 and !ccw(l.end()[-2], l.end()[-1], v[i]))
			l.pop_back();
		l.push_back(v[i]);
	}
	for (int i = (int)v.size() - 1; i >= 0; i--) {
		while (u.size() > 1 and !ccw(u.end()[-2], u.end()[-1], v[i]))
			u.pop_back();
		u.push_back(v[i]);
	}
	l.pop_back(); u.pop_back();
	for (pt i : u) l.push_back(i);
	return l;
}

// Wraps a convex hull with O(log n) point-location and extreme-point queries
struct convex_pol {
	vector<pt> pol;

	convex_pol() {}
	convex_pol(vector<pt> v) : pol(convex_hull(v)) {}

	// True if p lies inside or on the border of the hull
	bool is_inside(pt p) {
		if (pol.size() == 0) return false;
		if (pol.size() == 1) return p == pol[0];
		int l = 1, r = pol.size();
		while (l < r) {
			int m = (l+r)/2;
			if (ccw(p, pol[0], pol[m])) l = m+1;
			else r = m;
		}
		if (l == 1) return isinseg(p, line(pol[0], pol[1]));
		if (l == (int)pol.size()) return false;
		return !ccw(p, pol[l], pol[l-1]);
	}

	// Binary search for the vertex extreme with respect to comparator cmp
	int extreme(const function<bool(pt, pt)>& cmp) {
		int n = pol.size();
		auto extr = [&](int i, bool& cur_dir) {
			cur_dir = cmp(pol[(i+1)%n], pol[i]);
			return !cur_dir and !cmp(pol[(i+n-1)%n], pol[i]);
		};
		bool last_dir, cur_dir;
		if (extr(0, last_dir)) return 0;
		int l = 0, r = n;
		while (l+1 < r) {
			int m = (l+r)/2;
			if (extr(m, cur_dir)) return m;
			bool rel_dir = cmp(pol[m], pol[l]);
			if ((!last_dir and cur_dir) or
			   (last_dir == cur_dir and rel_dir == cur_dir)) {
				l = m;
				last_dir = cur_dir;
			} else r = m;
		}
		return l;
	}

	// Vertex that maximizes the dot product with v (support function)
	int max_dot(pt v) {
		return extreme([&](pt p, pt q) { return p*v > q*v; });
	}

	// Indices of the two tangent points seen from an external point p
	pair<int, int> tangents(pt p) {
		auto L = [&](pt q, pt r) { return ccw(p, r, q); };
		auto R = [&](pt q, pt r) { return ccw(p, q, r); };
		return {extreme(L), extreme(R)};
	}
};

// ===================== LATTICE POINTS (PICK'S THEOREM) =====================
// These two assume the polygon has integer-valued coordinates stored in pt.

// Interior lattice points, given twice the polygon's area and its boundary
// point count. Pick's theorem: Area = inside + bound/2 - 1  =>  2*Area = 2*inside + bound - 2
ll cntInsidePts(ld area2x, ll bound) {
	return (ll)llround(area2x + 2 - bound) / 2;
}

// Number of lattice points on segment a-b, endpoints included
ll latticePointsInSeg(pt a, pt b) {
	ll dx = llround(abs(a.x - b.x));
	ll dy = llround(abs(a.y - b.y));
	return __gcd(dx, dy) + 1;
}

/*LATEX_DESC_BEGIN***************************

   POLIGONO

   cut_polygon    -> corta poligono com a reta r, O(n)
   dist_rect      -> distancia entre retangulos (lados paralelos aos eixos)
   polarea        -> area do poligono
   inpol          -> 0 fora, 1 dentro, 2 na borda. O(n)
   intriangle     -> ponto dentro de triangulo (borda inclusa), caso especial de inpol
   interpol       -> se dois poligonos se intersectam. O(n*m)
   distpol        -> distancia entre poligonos
   polygonCenter  -> centro de massa (centroide). O(n)
   convex_hull    -> fecho convexo, O(n log n), sem pontos colineares na borda
   convex_pol     -> is_inside/extreme/max_dot/tangents em O(log n)
   cntInsidePts / latticePointsInSeg -> Teorema de Pick (assume coords inteiras)

*****************************LATEX_DESC_END*/