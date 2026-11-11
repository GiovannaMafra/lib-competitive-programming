#include "Point.cpp"

// A line/segment defined by its two endpoints p, q
struct line {
	pt p, q;
	line() {}
	line(pt p_, pt q_) : p(p_), q(q_) {}

	friend istream& operator >> (istream& in, line& r) {
		return in >> r.p >> r.q;
	}
};

// True if line r is vertical
bool isvert(line r) {
	return eq(r.p.x, r.q.x);
}

// True if point p lies on segment r (endpoints included)
bool isinseg(pt p, line r) {
	pt a = r.p - p, b = r.q - p;
	return eq((a ^ b), 0) and (a * b) < eps;
}

// Helper: parameter t of the point r.p + t*(r.q-r.p) that hits vector v
ld get_t(pt v, line r) {
	return (r.p^r.q) / ((r.p-r.q)^v);
}

// Orthogonal projection of point p onto line r
pt proj(pt p, line r) {
	if (r.p == r.q) return r.p;
	r.q = r.q - r.p; p = p - r.p;
	pt pr = r.q * ((p*r.q) / (r.q*r.q));
	return pr + r.p;
}

// Intersection point of the infinite lines r and s
// Returns (DINF, DINF) if r and s are parallel
pt inter(line r, line s) {
	if (eq((r.p - r.q) ^ (s.p - s.q), 0)) return pt(DINF, DINF);
	r.q = r.q - r.p, s.p = s.p - r.p, s.q = s.q - r.p;
	return r.q * get_t(r.q, s) + r.p;
}

// True if segments r and s intersect (touching at an endpoint counts)
bool interseg(line r, line s) {
	if (isinseg(r.p, s) or isinseg(r.q, s)
	 or isinseg(s.p, r) or isinseg(s.q, r)) return 1;

	return ccw(r.p, r.q, s.p) != ccw(r.p, r.q, s.q) and
	       ccw(s.p, s.q, r.p) != ccw(s.p, s.q, r.q);
}

// Full segment intersection: {} if none, {p} if unique, {a,b} if the
// segments overlap along a shared collinear stretch (a,b are its ends)
vector<pt> seginter(line r, line s) {
	if (!interseg(r, s)) return {};
	if (!col(r.p, r.q, s.p) or !col(r.p, r.q, s.q)) return {inter(r, s)};

	set<pt> res;
	if (isinseg(r.p, s)) res.insert(r.p);
	if (isinseg(r.q, s)) res.insert(r.q);
	if (isinseg(s.p, r)) res.insert(s.p);
	if (isinseg(s.q, r)) res.insert(s.q);
	return {res.begin(), res.end()};
}

// Distance from point p to the infinite line r
ld disttoline(pt p, line r) {
	return 2 * abs(sarea(p, r.p, r.q)) / dist(r.p, r.q);
}

// Distance from point p to segment r
ld disttoseg(pt p, line r) {
	if ((r.q - r.p)*(p - r.p) < 0) return dist(r.p, p);
	if ((r.p - r.q)*(p - r.q) < 0) return dist(r.q, p);
	return disttoline(p, r);
}

// Minimum distance between segments a and b (0 if they intersect)
ld distseg(line a, line b) {
	if (interseg(a, b)) return 0;

	ld ret = DINF;
	ret = min(ret, disttoseg(a.p, b));
	ret = min(ret, disttoseg(a.q, b));
	ret = min(ret, disttoseg(b.p, a));
	ret = min(ret, disttoseg(b.q, a));

	return ret;
}

// ===================== SWEEP-LINE HELPERS =====================
// Assumes segments are stored with p < q

// Orders segments by direction angle, then by relative position
bool operator < (const line& a, const line& b) {
	pt v1 = a.q - a.p, v2 = b.q - b.p;
	if (!eq(angle(v1), angle(v2))) return angle(v1) < angle(v2);
	return ccw(a.p, a.q, b.p);
}
bool operator == (const line& a, const line& b) {
	return !(a < b) and !(b < a);
}

// Comparator for a set used to do a left-to-right sweep line over segments
struct cmp_sweepline {
	bool operator () (const line& a, const line& b) const {
		if (a.p == b.p) return ccw(a.p, a.q, b.q);
		if (!eq(a.p.x, a.q.x) and (eq(b.p.x, b.q.x) or a.p.x+eps < b.p.x))
			return ccw(a.p, a.q, b.p);
		return ccw(a.p, b.q, b.p);
	}
};

pt sweep_dir; // set this to the current sweep direction before using cmp_sweepangle

// Comparator for a set used to do an angular sweep over segments
struct cmp_sweepangle {
	bool operator () (const line& a, const line& b) const {
		return get_t(sweep_dir, a) + eps < get_t(sweep_dir, b);
	}
};

/*LATEX_DESC_BEGIN***************************

   RETA / SEGMENTO

   line          -> struct base (p, q)
   isvert        -> se a reta eh vertical
   isinseg       -> se o ponto pertence ao segmento
   get_t         -> parametro auxiliar de intersecao
   proj          -> projecao cartesiana do ponto na reta
   inter         -> intersecao de duas retas (infinitas)
   interseg      -> se dois segmentos se interceptam (booleano)
   seginter      -> pontos exatos da intersecao (unico ou par colinear)
   disttoline / disttoseg / distseg -> distancias ponto-reta, ponto-seg, seg-seg
   operator< / == e cmp_sweepline    -> comparadores para sweep line
   cmp_sweepangle -> comparador para sweep angular (usa sweep_dir global)

*****************************LATEX_DESC_END*/
