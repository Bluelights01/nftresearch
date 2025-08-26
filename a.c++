#include<iostream>
using namespace std;
bool check(string &s){

     stack<char>st;
     for(auto i:s){
        if(i=='('){
            st.push('(');
        }
        else{
            if(!st.empty()){
                st.pop();
            }
            else{
                return 0;
            }
        }
     }
     if(st.empty()){
        return 1;
     }
     return 0;

}
void solve(int n,string &s){
   if(s.length()==2*n){
     if(check(s)==1){
        cout<<s<<endl;
     }
     return;
   }
   else{

      s.push_back('(');
      solve(n,s);
      s.pop_back();
      s.push_back(')');
      solve(n,s);
      s.pop_back();
   }
}
int main(){
    int n;
    cin>>n;
}