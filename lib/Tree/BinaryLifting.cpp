#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int n, l = 30;
vector<vector<int>> adj(MAXN);

int timer;
vector<int> tin;
vector<int> tout;
vector<vector<int>> up;
vector<int> depth(MAXN);

void dfs(int v, int p){
    tin[v] = ++timer;
    up[v][0] = p;
    for (int i = 1; i <= l; ++i)
        up[v][i] = up[up[v][i-1]][i-1];

    for(auto u : adj[v]){
        if(u != p)
            dfs(u, v);
    }
    tout[v] = ++timer;
}

void preprocess(int root){
    tin.resize(n);
    tout.resize(n);
    timer = 0;
    l = ceil(log2(n));
    up.assign(n, vector<int>(l+1));
    dfs(root, root);
}

int kth_anc(int node, int k){
    int cur = node;
    for (int i = 0; i <= l; i++)
    {
        if((k >> i) & 1) cur = up[cur][i];
    }
    return cur;
}

/*LATEX_DESC_BEGIN***************************
	Binary Lifting O(NlogN)
    kth_anc O(logK)
    
*****************************LATEX_DESC_END*/