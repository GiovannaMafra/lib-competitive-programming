/* Segment Tree (Point Update, Range Query)
 * Complexidade: O(log N) para update e query.
 * Memória: O(4*N)
 * Requisitos: 
 * - NODE deve ter: static merge(L, R), apply(V), e construtor identidade.
 * SE USA L E R ZERO INDEXADOS
 */

struct Node {
    ll val = 0;
    Node(ll v = 0) : val(v) {}
    
    static inline Node merge(const Node& l, const Node& r) {
        return Node(l.val + r.val);
    }

    // Para Point Set: val = v;
    // Para Point Add: val += v;
    inline void apply(int v) {
        val += v; 
    }
};

struct SegTree {
    int N;
    vector<Node> seg;

    SegTree(int n) : N(n), seg(4 * n) {}

    SegTree(const vector<int>& v) : N(v.size()), seg(4 * v.size()) {
        build(1, 0, N - 1, v);
    }

    void build(int no, int l, int r, const vector<int>& v) {
        if (l == r) {
            seg[no] = Node(v[l]);
            return;
        }
        int m = (l + r) >> 1;
        build(no << 1, l, m, v);
        build((no << 1) | 1, m + 1, r, v);
        seg[no] = Node::merge(seg[no << 1], seg[(no << 1) | 1]);
    }

    void update(int no, int l, int r, int idx, int val) {
        if (l == r) {
            seg[no].apply(val);
            return;
        }
        int m = (l + r) >> 1;
        if (idx <= m) update(no << 1, l, m, idx, val);
        else update((no << 1) | 1, m + 1, r, idx, val);
        seg[no] = Node::merge(seg[no << 1], seg[(no << 1) | 1]);
    }

    Node query(int no, int l, int r, int a, int b) {
        if (b < l || r < a) return Node();
        if (a <= l && r <= b) return seg[no];
        int m = (l + r) >> 1;
        return Node::merge(query(no << 1, l, m, a, b),
                           query((no << 1) | 1, m + 1, r, a, b));
    }

    void update(int idx, int val) { update(1, 0, N - 1, idx, val); }
    Node query(int l, int r) { return query(1, 0, N - 1, l, r); }
};