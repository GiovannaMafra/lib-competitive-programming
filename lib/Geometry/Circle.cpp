
// Circumcenter of the triangle formed by a, b, c
pt getcenter(pt a, pt b, pt c) {
	b = (a + b) / 2;
	c = (a + c) / 2;
	return inter(line(b, b + rotate90(a - b)),
	             line(c, c + rotate90(a - c)));
}

// Intersection points of line a-b with the circle centered at c, radius r
vector<pt> circ_line_inter(pt a, pt b, pt c, ld r) {
	vector<pt> ret;
	b = b-a, a = a-c;
	ld A = b*b;
	ld B = a*b;
	ld C = a*a - r*r;
	ld D = B*B - A*C;
	if (D < -eps) return ret;
	ret.push_back(c+a+b*(-B+sqrt(D+eps))/A);
	if (D > eps) ret.push_back(c+a+b*(-B-sqrt(D))/A);
	return ret;
}

// Intersection points of circles (a, r) and (b, R)
vector<pt> circ_inter(pt a, pt b, ld r, ld R) {
	vector<pt> ret;
	ld d = dist(a, b);
	if (d > r+R or d+min(r, R) < max(r, R)) return ret;
	ld x = (d*d-R*R+r*r)/(2*d);
	ld y = sqrt(r*r-x*x);
	pt v = (b-a)/d;
	ret.push_back(a+v*x + rotate90(v)*y);
	if (y > 0) ret.push_back(a+v*x - rotate90(v)*y);
	return ret;
}

/*LATEX_DESC_BEGIN***************************

   CIRCUNFERENCIA

   getcenter       -> centro da circunferencia dado 3 pontos
   circ_line_inter -> intersecao da circunferencia (c, r) com a reta ab
   circ_inter      -> intersecao das circunferencias (a, r) e (b, R)

*****************************LATEX_DESC_END*/