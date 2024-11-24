#include<bits/stdc++.h>
using namespace std;

int main(){

    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        vector<int>v(n);
        for(int i=0;i<n;i++){
            cin>>v[i];
        }

        bool flag = true;
        while(flag){
            flag = false;
            for(int i=0;i<n-1;i++){
                if(v[i]>v[i+1]){
                    flag = true;
                    int avg = (v[i]+v[i+1])/2;
                    v[i] = avg;
                    v[i+1] = avg;
                    if((v[i]+v[i+1])%2==1){
                        v[i+1]++;
                    }
                    i++;
                }
            }
        }
        cout<<*max_element(v.begin(),v.end())-*min_element(v.begin(),v.end())<<endl;
    }

    return 0;
}