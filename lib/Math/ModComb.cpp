#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define MOD 9987123

vector<ll> fact(1e6, -1);

void pre(){
    fact[0] = 1;
    for(ll i = 1; i < fact.size(); i++){
        fact[i] = (fact[i-1] * i) % MOD;
    }
}

ll fexp(ll a, ll b){
    ll ans = 1;

    while(b){
        if(b & 1) ans = (ans * a) % MOD;
        a = (a*a) % MOD;
        b >>= 1;
    }

    return ans;
}

ll inv(ll a, ll p){
    return fexp(a, p -2);
}

ll comb(ll n, ll k, ll p){
    return ((fact[n] * inv(fact[k], p)) % p) * inv(fact[n-k], p) % p;
}

ll extgcd(ll a, ll m, ll &x, ll &y) {
    if (m == 0) {
        x = 1; 
        y = 0; 
        return a; 
    }
 
    ll x1, y1; 
    ll g = extgcd(m, a % m, x1, y1);
    x = y1; 
    y = x1 - (a/m) * y1; 
    return g; 

    /*
    na main:
    inverso modular = ((val % m) + m) % m;
    */
}

/*LATEX_DESC_BEGIN***************************
    Combinação modular
    Teorema de Fermat - O(log p)
    Euclides Estendido - O(log n)
    Exponenciação rápida (O (Log P ) - p: potência)
    O(N) fatorial
*****************************LATEX_DESC_END*/