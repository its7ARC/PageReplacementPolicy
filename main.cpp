//
//  main.cpp
//  PageReplacement
//
//  Created by Anshuman Raj Chauhan on 19/04/21.
//

//inclusions
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <limits.h>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <string>
#include <cstring>
#include <climits>
#include <queue>

//macros
#define ll int  //
#define pb push_back
#define pf push_front
#define ppf pop_front
#define ppb pop_back
#define ff first
#define ss second
#define ii pair<ll,ll>
#define vi vector<ll>
#define vii vector<pair<ll,ll>>
#define vvi vector<vector<ll>>
#define res reserve
#define mii map<ll,ll>
#define umii unordered_map<ll,ll>
#define u_map unordered_map
#define s_bits __builtin_popcount    //
#define t_z __builtin_ctz   //
#define l_z __builtin_clz   //
#define lb lower_bound
#define ub upper_bound
#define inf INT_MAX
#define MOD 1000000007
#define fr(i,a,b) for(auto i = a; i < b; i++)
#define tr(it,a,b) for(auto (it) = (a); (it) != (b);)
using namespace std;


/*####*/

/*Sample Input1
 3
 6
 1 3 0 3 6 5
 */

/* Sample Input2
 4
 14
 7 0 1 2 0 3 0 4 2 3 0 3 2 3
 */


/*####*/


//Program

void line(){
    fr(i,0,40) cout << "--";
    cout <<"\n";
}


struct PageTable{
    
    //2 hashmaps for page search in page table complexity to be O(logn);
    mii arr_page;
    umii page_arr;
    ll n_pf;
    map<ll, vi> query_instances;
    
    PageTable(ll n):n_pf(n){}
    
    void clear(){
        arr_page.clear(); page_arr.clear(); query_instances.clear();
    }
    
    bool fifo_find(ii query){
        if(page_arr.find(query.ss) != page_arr.end()) return 1;
        
        if(ll(arr_page.size()) == n_pf){
            auto it1 = arr_page.begin();
            auto it2 = page_arr.find(it1->ss);
            arr_page.erase(it1); page_arr.erase(it2);
        }
        arr_page.insert(query);
        page_arr.insert({query.ss,query.ff});
        
        return 0;
    }
    
    bool optimalPR_find(ii query){
        if(page_arr.find(query.ss) != page_arr.end()) return 1;
        
        if(ll(arr_page.size()) == n_pf){
            mii::iterator remove;
            ll remove_dum = -1;
            
            //choosing page to remove
            tr(it,arr_page.begin(),arr_page.end()){
                auto curr = it++;
                vi instances = query_instances[curr->ss];
                auto first_occ_it = ub(instances.begin(),instances.end(),query.ff);
                if(first_occ_it == instances.end()){ remove = curr; break;}
                else if(*first_occ_it > remove_dum){
                    remove = curr; remove_dum = *first_occ_it;
                }
            }
            
            auto it2 = page_arr.find(remove->ss);
            arr_page.erase(remove); page_arr.erase(it2);
        }
        arr_page.insert(query);
        page_arr.insert({query.ss,query.ff});
        
        return 0;
    }
    
    bool lru_find(ii query){
        if(page_arr.find(query.ss) != page_arr.end()){
            auto it1 = page_arr.find(query.ss);
            auto it2 = arr_page.find(it1->ss);
            it1->ss = query.ff;
            arr_page.erase(it2); arr_page.insert(query);
            return 1;
        }
        
        if(ll(arr_page.size()) == n_pf){
            auto it1 = arr_page.begin();
            auto it2 = page_arr.find(it1->ss);
            arr_page.erase(it1); page_arr.erase(it2);
        }
        arr_page.insert(query);
        page_arr.insert({query.ss,query.ff});
        
        return 0;
    }
    
    void display(){
        if(arr_page.empty()) cout << "Empty\n";
        ll idx = 0;
        tr(it,arr_page.begin(),arr_page.end()){
            auto curr = it++;
            cout << idx++ << ": " << curr->ss << "\n";
        }
        return;
    }
};


class PageReplacement{
    vii query;
    //number of page frames
    ll q_idx = 0;
    ll n_pf;
    PageTable page_table;

public:
    PageReplacement(ll n):n_pf(n), page_table(n_pf){};
    void insert(ll i){
        query.pb({q_idx++,i});
    }
    
    //FIFO page replacement
    void fifo_pr(){
        line();
        cout << "FIFO Page Replacement\n";
        ll hit = 0;
        page_table.clear();
        fr(i,0,ll(query.size())){
            cout << "Page Table\n";
            page_table.display();
            if(page_table.fifo_find(query[i])){
                cout << "Query #" << i+1 << ": Query Page " << query[i].ss << ": HIT\n";
                hit++;
            }
            else cout << "Query #" << i+1 << ": Query Page " << query[i].ss << ": MISS\n";
            cout << "\n";
        }
        ll tot_q = ll(query.size());
        cout << "Total Queries: " << tot_q << "\n";
        cout << "# HIT: " << hit << "\n";
        cout << "# Page Faults: " << tot_q - hit << "\n";
        line();
    }
    
    //Optimal Page Replacement
    void optimal_pr(){
        line();
        cout << "Optimal Page Replacement\n";
        ll hit = 0;
        page_table.clear();
        map<ll, vi> query_instances;
        fr(i,0,ll(query.size())) query_instances[query[i].ss].pb(query[i].ff);
        page_table.query_instances = query_instances;
        fr(i,0,ll(query.size())){
            cout << "Page Table\n";
            page_table.display();
            if(page_table.optimalPR_find(query[i])){
                cout << "Query #" << i+1 << ": Query Page " << query[i].ss << ": HIT\n";
                hit++;
            }
            else cout << "Query #" << i+1 << ": Query Page " << query[i].ss << ": MISS\n";
            cout << "\n";
        }
        ll tot_q = ll(query.size());
        cout << "Total Queries: " << tot_q << "\n";
        cout << "# HIT: " << hit << "\n";
        cout << "# Page Faults: " << tot_q - hit << "\n";
        line();
    }
    
    //LRU Page Replacement
    void lru_pr(){
        line();
        cout << "LRU Page Replacement\n";
        ll hit = 0;
        page_table.clear();
        fr(i,0,ll(query.size())){
            cout << "Page Table\n";
            page_table.display();
            if(page_table.lru_find(query[i])){
                cout << "Query #" << i+1 << ": Query Page " << query[i].ss << ": HIT\n";
                hit++;
            }
            else cout << "Query #" << i+1 << ": Query Page " << query[i].ss << ": MISS\n";
            cout << "\n";
        }
        cout << "Page Table\n";
        page_table.display();
        
        ll tot_q = ll(query.size());
        cout << "Total Queries: " << tot_q << "\n";
        cout << "# HIT: " << hit << "\n";
        cout << "# Page Faults: " << tot_q - hit << "\n";
        line();
    }
    
    void simulate(){
        if(query.empty()){
            cout << "Query Underflow\n";
            return;
        }
        
        fifo_pr();
        optimal_pr();
        lru_pr();
    }
    
};


int main(){
    
    ll n, page, n_pf;
    
    cout << "Enter number of page frames\n";
    cin >> n_pf;
    PageReplacement page_rep(n_pf);
    
    cout << "Enter number of page reference queries\n";
    cin >> n;
    cout << "Enter page indices in order for simulation of different page replacement policies\n";
    fr(i,0,n){
        cin>> page;
        page_rep.insert(page);
    }
    
    page_rep.simulate();
    
    return 0;
    
}
