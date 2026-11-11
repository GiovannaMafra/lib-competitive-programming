#include <bits/stdc++.h>
using namespace std;

typedef double ld;
typedef long long ll;
const ld DINF = 1e18;
const ld pi = acos(-1.0);
const ld eps = 1e-9;

#define sq(x) ((x)*(x))

bool eq(ld a, ld b) {
	return abs(a - b) <= eps;
}

// A 2D point, also used as a vector. Coordinates are doubles.
// If you need exact lattice-point results (e.g. Pick's theorem in
// Polygon.cpp), keep the values you store here integral.
struct pt {
	ld x, y;
	pt(ld x_ = 0, ld y_ = 0) : x(x_), y(y_) {}

	bool operator < (const pt p) const {
		if (!eq(x, p.x)) return x < p.x;
		if (!eq(y, p.y)) return y < p.y;
		return 0;
	}
	bool operator == (const pt p) const {
		return eq(x, p.x) and eq(y, p.y);
	}
	pt operator + (const pt p) const { return pt(x+p.x, y+p.y); }
	pt operator - (const pt p) const { return pt(x-p.x, y-p.y); }
	pt operator * (const ld c) const { return pt(x*c, y*c); }
	pt operator / (const ld c) const { return pt(x/c, y/c); }
	ld operator * (const pt p) const { return x*p.x + y*p.y; } // dot product
	ld operator ^ (const pt p) const { return x*p.y - y*p.x; } // cross product

	friend istream& operator >> (istream& in, pt& p) {
		return in >> p.x >> p.y;
	}
};

// Euclidean distance between p and q
ld dist(pt p, pt q) {
	return hypot(p.y - q.y, p.x - q.x);
}

// Squared distance between p and q (avoids the sqrt call)
ld dist2(pt p, pt q) {
	return sq(p.x - q.x) + sq(p.y - q.y);
}

// Length (norm) of vector v
ld norm(pt v) {
	return dist(pt(0, 0), v);
}

// Angle of vector v with the x-axis, normalized to [0, 2*pi)
ld angle(pt v) {
	ld ang = atan2(v.y, v.x);
	if (ang < 0) ang += 2*pi;
	return ang;
}

// Signed angle between vectors p and q, in the range [-pi, pi]
ld angle(pt p, pt q) {
	return atan2(p ^ q, p * q);
}

// Cartesian quadrant of p: 0(++) 1(-+) 2(--) 3(+-)
int quad(pt p) {
	return (p.x < 0) ^ (3 * (p.y < 0));
}

// Signed area of triangle p,q,r (positive if p->q->r turns ccw)
ld sarea(pt p, pt q, pt r) {
	return ((q-p)^(r-q))/2;
}

// True if p, q, r are collinear
bool col(pt p, pt q, pt r) {
	return eq(sarea(p, q, r), 0);
}

// True if the turn p -> q -> r is counter-clockwise
bool ccw(pt p, pt q, pt r) {
	return sarea(p, q, r) > eps;
}

// Comparator to sort points by polar angle around the origin
bool cmp_ang(pt p, pt q) {
	return quad(p) != quad(q) ? quad(p) < quad(q) : (p ^ q) > eps;
}

// Scalar projection: signed length of p's projection onto direction q
ld projlen(pt p, pt q) {
	return (p*q) / norm(q);
}

// Rotates p by th radians, counter-clockwise
pt rotate(pt p, ld th) {
	return pt(p.x * cos(th) - p.y * sin(th),
	          p.x * sin(th) + p.y * cos(th));
}

// Rotates p by 90 degrees counter-clockwise
pt rotate90(pt p) {
	return pt(-p.y, p.x);
}

// Rotates p by 90 degrees clockwise
pt rotateCW90(pt p) {
	return pt(p.y, -p.x);
}

/*LATEX_DESC_BEGIN***************************

   PONTO & VETOR

   pt        -> struct base (x, y). '*' eh produto escalar, '^' eh vetorial
   dist/dist2/norm -> distancias e norma
   angle(v)      -> angulo do vetor com eixo x, em [0, 2pi)
   angle(p, q)   -> angulo entre p e q, em [-pi, pi]
   quad(p)       -> quadrante cartesiano de p
   sarea         -> area com sinal de p,q,r
   col / ccw     -> colinearidade / sentido anti-horario
   cmp_ang       -> comparador de ordenacao por angulo polar
   projlen       -> tamanho (com sinal) da projecao escalar de p em q
   rotate / rotate90 / rotateCW90 -> rotacoes

*****************************LATEX_DESC_END*/