
int lca(int u, int v){
    if(is_ancestor(u, v)) 
        return u;
    if(is_ancestor(v, u)) 
        return v;
    for (int i = l; i >= 0; --i)
    {
        if(!is_ancestor(up[u][i], v))
            u = up[u][i];
    }
    return up[u][0];
}  

bool is_ancestor(int u, int v){
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int dist(int u, int v){
    return depth[u] + depth[v] - 2*depth[(lca(u,v))];
}
/*LATEX_DESC_BEGIN***************************
	LCA com Binary Lifting O(logN)
    dist é a distancia entre dois vertices
*****************************LATEX_DESC_END*/