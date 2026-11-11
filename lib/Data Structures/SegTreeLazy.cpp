/* Lazy Segment Tree (Range Update, Range Query)
 * Realiza atualizações e consultas em range.
 * Complexidade: O(log N) para update e query.
 * Memória: O(4*N)
 * Requisitos:
 * - NODE deve ter: static merge(L, R), apply(TAG, L, R) e
 *   construtor identidade.
 * - TAG deve ter: compose(TAG) e construtor identidade.
 */

/* --- Exemplo de NODE e TAG (Soma e Multiplicacao com Modulo) ---
*/
struct Tag {
    ll mul = 1, add = 0;

    Tag(){};

    Tag(ll mul_, ll add_){
        mul = (mul_ % MOD);
        add = (add_ % MOD);
    }
    void inline compose(const Tag& t) {
        add = (add * t.mul + t.add) % MOD;
        mul = (mul * t.mul) % MOD;
    }
};

struct Node {
    ll val = 0;
    Node(ll v = 0) : val(v) {}
    static inline Node merge(const Node& l, const Node& r) {
        return Node((l.val + r.val) % MOD);
    }
    void inline apply(const Tag& t, int l, int r) {
        val = (val * t.mul + t.add * (r - l + 1)) % MOD;
    }
};

struct LazySegmentTree {
    int N;
    vector<Node> seg;
    vector<Tag> lazy;

    LazySegmentTree(int n) : N(n), seg(4 * n), lazy(4 * n) {}

    LazySegmentTree(const vector<ll>& v)
        : N(v.size()), seg(4 * v.size()), lazy(4 * v.size()) {
        build(1, 0, N - 1, v);
    }

    void build(int no, int l, int r, const vector<ll>& v) {
        if (l == r) {
            seg[no] = Node(v[l]);
            return;
        }
        int m = (l + r) >> 1;
        build(no << 1, l, m, v);
        build((no << 1) | 1, m + 1, r, v);
        seg[no] = Node::merge(seg[no << 1], seg[(no << 1) | 1]);
    }

    void push(int no, int l, int r) {
        int m = (l + r) >> 1;
        int e = no << 1, d = e | 1;

        seg[e].apply(lazy[no], l, m);
        lazy[e].compose(lazy[no]);

        seg[d].apply(lazy[no], m + 1, r);
        lazy[d].compose(lazy[no]);

        lazy[no] = Tag(); 
    }

    void update(int no, int l, int r, int a, int b, const Tag& v) {
        if (b < l || r < a) return;
        if (a <= l && r <= b) {
            seg[no].apply(v, l, r);
            lazy[no].compose(v);
            return;
        }
        push(no, l, r);
        int m = (l + r) >> 1;
        update(no << 1, l, m, a, b, v);
        update((no << 1) | 1, m + 1, r, a, b, v);
        seg[no] = Node::merge(seg[no << 1], seg[(no << 1) | 1]);
    }

    Node query(int no, int l, int r, int a, int b) {
        if (b < l || r < a) return Node();
        if (a <= l && r <= b) return seg[no];
        push(no, l, r);
        int m = (l + r) >> 1;
        return Node::merge(query(no << 1, l, m, a, b),
                           query((no << 1) | 1, m + 1, r, a, b));
    }

    void update(int l, int r, const Tag& v) { update(1, 0, N - 1, l, r, v); }
    Node query(int l, int r) { return query(1, 0, N - 1, l, r); }
};