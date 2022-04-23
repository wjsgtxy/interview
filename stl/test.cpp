#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

bool cmp(char a, char b){
    if(toupper(a) < toupper(b)){
        return true;
    }else{
        return false;
    }
}

int main(){
    string s;
    getline(cin, s);
    vector<char> v;
    for(char c : s){
        if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) {
            v.push_back(c);
        }
    };
    // 对vector按照A-Z排序
    stable_sort(v.begin(), v.end(), cmp);
    for(char c:v){
        cout << c;
    }
    cout << endl;
    return 0;
}