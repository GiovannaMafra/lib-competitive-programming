#include <bits/stdc++.h>
using namespace std;

// Se for usar MIN ou MAX, altere este valor neutro conforme os comentários
const long long INF = 1e18; 

struct BinaryLiftingLCA {
    int n, max_log;
    vector<int> depth;
    vector<vector<int>> up;
    vector<vector<long long>> val; // Tabela de Binary Lifting para os valores

    // adj guarda pares: {destino, peso}
    BinaryLiftingLCA(int n, int root, const vector<vector<pair<int, long long>>>& adj) : n(n) {
        max_log = __lg(n) + 1;
        depth.assign(n + 1, 0);
        up.assign(n + 1, vector<int>(max_log, root));
        
        // --- PARA SOMA: inicialize com 0
        // --- PARA MIN:  inicialize com INF
        // --- PARA MAX:  inicialize com -INF
        val.assign(n + 1, vector<long long>(max_log, 0));

        auto dfs = [&](auto self, int u, int p, int d, long long w_parent) -> void {
            depth[u] = d;
            up[u][0] = p;
            val[u][0] = w_parent;

            for (int i = 1; i < max_log; ++i) {
                int anc = up[u][i - 1];
                up[u][i] = up[anc][i - 1];
                
                // === OPERAÇÃO ENTRE SALTOS ===
                // --- PARA SOMA: val[u][i] = val[u][i - 1] + val[anc][i - 1];
                // --- PARA MIN:  val[u][i] = min(val[u][i - 1], val[anc][i - 1]);
                // --- PARA MAX:  val[u][i] = max(val[u][i - 1], val[anc][i - 1]);
                val[u][i] = val[u][i - 1] + val[anc][i - 1];
            }

            for (auto& [v, w] : adj[u]) {
                if (v != p) self(self, v, u, d + 1, w);
            }
        };

        // Para ARESTAS: a aresta acima da raiz tem peso neutro (0 para soma, INF para min, -INF para max)
        dfs(dfs, root, root, 0, 0);
    }

    int get_kth_ancestor(int u, int k) {
        for (int i = 0; i < max_log; ++i) {
            if ((k >> i) & 1) u = up[u][i];
        }
        return u;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = get_kth_ancestor(u, depth[u] - depth[v]);
        if (u == v) return u;

        for (int i = max_log - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                u = up[u][i];
                v = up[v][i];
            }
        }
        return up[u][0];
    }

    // Consulta do valor no caminho u <-> v
    long long query_path(int u, int v) {
        // === VALOR INICIAL DA RESPOSTA ===
        // --- PARA SOMA: long long ans = 0;
        // --- PARA MIN:  long long ans = INF;
        // --- PARA MAX:  long long ans = -INF;
        long long ans = 0;

        auto lift = [&](int& node, int k) {
            for (int i = 0; i < max_log; ++i) {
                if ((k >> i) & 1) {
                    // === ACUMULAR A RESPOSTA ===
                    // --- PARA SOMA: ans += val[node][i];
                    // --- PARA MIN:  ans = min(ans, val[node][i]);
                    // --- PARA MAX:  ans = max(ans, val[node][i]);
                    ans += val[node][i];
                    node = up[node][i];
                }
            }
        };

        if (depth[u] < depth[v]) swap(u, v);

        // 1. Sobe 'u' até a mesma profundidade de 'v'
        lift(u, depth[u] - depth[v]);

        if (u == v) return ans;

        // 2. Sobe ambos até um nível abaixo do LCA
        for (int i = max_log - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                lift(u, 1 << i);
                lift(v, 1 << i);
            }
        }

        // 3. Pula o último passo de ambos até o LCA
        lift(u, 1);
        lift(v, 1);

        return ans;
    }
};

/*LATEX_DESC_BEGIN***************************
	Operações associativas com Binary Lifting (max, min, soma, gcd)
    esse codigo as arestas arazenam os valores na arvore
    se quiser que os nós armazem os valores:
        dfs não vai levar em cosideração o peso;
        val[u][0] = node_val[u];
        Na query, antes de retornar leva em cosideração o valor do lca
*****************************LATEX_DESC_END*/

