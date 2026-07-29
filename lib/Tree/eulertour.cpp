#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
vector<vector<int>> adj(MAXN);

int timer;
vector<int> tin;
vector<int> tout;

void euler_tour(int node, int prev){
    tin[node] = timer++;
    for(auto n : adj[node]){
        if(n != prev) {euler_tour(n, node);}
    }
    tout[node] = timer;
}

bool is_ancestor(int u, int v){
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

/*LATEX_DESC_BEGIN***************************
	Euler Tour O(n)
    Lineariza um grafo
    Subarvore com raiz u é de tin[u] até tout[u]
    Verificar a ancestralida u é ancestral de v se in[u] <= in[v] <= out[u]
    
*****************************LATEX_DESC_END*/