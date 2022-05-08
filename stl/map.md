```c++
#include <iostream>
#include <map>
#include <cstdio>

using namespace std;

void printMap(map<string, int> &mp){
    for(auto w : mp){
        cout << w.first << ":" << w.second << endl;
    }

    // c++17的语法 structure bindings
    // for(auto [k, v] : mp){
    //     cout << k << ":" << v;
    // }
}

void testInit(){
    map<string,int> mp;
    mp.insert(make_pair<string,int>("hello", 1)); // make_pair(v2,v2);
    // printMap(mp);

    // map初始化，通过一个map初始化另外一个map
    map<string, int> mp2(mp.begin(), mp.end());
    printMap(mp2);

    // map初始化，通过一个map初始化另外一个map
    map<string, int> mp3(mp);
    printMap(mp3);

    // 列表初始化，重复的key只会保留第一个
    map<string, int> mp4{{"hello", 1}, {"hello", 2},  {"world", 2}, {"xiao", 3}, {"yao", 4}}; // 列表初始化，hello为2的地方没有初始化到
}

void testCapacity(){
    map<string, int> mp{{"hello", 1}, {"world", 2}};

    
    // empty
    cout << "empty? " << mp.empty() << endl;

    // size
    cout << "size: " << mp.size() << endl;

    // max_size 输出 256204778801521550
    cout << "max size: " << mp.max_size() << endl;
}

void testModifiers(){
    // pair<iterator,bool> insert (const value_type& val); 返回一个pair
    map<string, int> mp{{"hello", 1}, {"world", 2}};

    // 插入失败
    pair<string, int> new_item1("hello", 2);
    pair<map<string,int>::iterator, bool> ret = mp.insert(new_item1);
    if(ret.second == false){
        // 插入失败, 迭代器指向已经存在的那个元素
        cout << "element " << ret.first->first << " already existed";
        cout << " with a value of " << ret.first->second << '\n';
    }

    // 插入成功
    ret = mp.insert(pair<string, int>("xiao", 2)); // 直接写pair的话，就不需要变量名称了
    if(ret.second == true){
        // 返回的pair的first指向成功插入的元素
        cout << "insert element " << ret.first->first << " value:" << ret.first->second << endl; 
    }
    // printMap(mp);

    // 第二种insert
    auto it = mp.begin();
    mp.insert(it, pair<string,int>("yao", 11)); // 并没有插在第一个位置，TODO，看文档好像只是一个提示，map的顺序还是按照key来的？？
    cout << "1------------" << endl;
    printMap(mp);

    // erase
    mp.erase(mp.begin()); // 删除了第一个插入的 "hello"
    cout << "erase 1---------" << endl;
    printMap(mp);

    // 先查找
    it = mp.find("yao");
    mp.erase(it);

    // 直接按照key删除
    mp.erase("xiao");
    cout<< "earse 2-----------" << endl;
    printMap(mp);

    // 删除一个范围
    mp.erase(mp.find("world"), mp.end());

    // clear 清空一个map
    mp.clear();

    // emplace 高效插入
    auto it2 = mp.emplace("hello", 1); // 也返回一个pair，和insert一样
    if(it2.second == true){
        cout << "insert sucess, key:" << it2.first->first << " value: " << it2.first->second << endl;
    }
    
}

void testObservers(){
    // key_compare key_comp() const;
    // 返回map内部的比较器
    map<char, int> mp;
    mp['e'] = 0;
    mp['a'] = 1;
    mp['b'] = 2;
    mp['c'] = 3;
    mp['d'] = 4;

    auto it = mp.find('c');
    map<char, int>::key_compare cm = mp.key_comp();
    auto it2 = mp.begin();
    while(cm(it2->first, it->first)){ // 比较大小 相当于 less(即 < )
        cout << "key: " << it2->first << " vaule: " << it2->second << endl; // 输出了ab,没有c
        it2++;
    }

    // 注意！！！map中的元素是会排序的！！！
    cout << "rbegin key: " <<  mp.rbegin()->first << endl; // 输出e

    // 正序输出 abcde
    for(auto it=mp.begin(); it!=mp.end(); it++){
        cout << " " << it->first; 
    }

    // 逆序输出 edcba
    for(auto it=mp.rbegin(); it!=mp.rend(); it++){
        cout << " " << it->first; 
    }

}

void testOperations(){
    // find: 找到返回迭代器，找不到返回map::end
    map<string,int> mp{{"hello", 1}, {"world", 2}};
    map<string,int>::iterator it = mp.find("hello");
    if(it != mp.end()){
        // cout << "ok find hello, value is " << *it << endl; // map迭代器
        cout << "ok find hello, value is " << it->second << endl; // todo map中存的是pair，迭代器指向的也是pair？
    }else{
        cout << "not found" << endl;
    }

    // count:找到返回1， 没找到返回0, map中没什么作用，感觉在multimap中有用
    size_t ct = mp.count("world");
    cout << "count world:" << ct << endl;

    // lower_bound, upper_bound
    map<char,int> mymap;
    mymap['a'] = 20;
    mymap['b']=40;
    mymap['c']=60;
    mymap['d']=80;
    mymap['e']=100;

    auto itlow = mymap.lower_bound('b'); // itlow points to b
    auto itup = mymap.upper_bound('d'); // itup points to e, not d!!

    // 删除两个迭代器之间的元素，[itlow, itup)
    mymap.erase(itlow, itup);
    for(auto c: mymap){
        cout << c.first << ":" << c.second << endl; // a:20 e:100
    }

    // equal_range http://www.cplusplus.com/reference/map/map/equal_range/
}

void testMultiMapModifiers(){
    multimap<char, int> ml;
    ml.insert(pair<char,int>('c', 10));
    ml.insert(pair<char,int>('b', 20));
    ml.insert(pair<char,int>('b', 10));
    ml.insert(pair<char,int>('d', 10));
    ml.insert(pair<char,int>('a', 10));

    // 注意，multimap也是有序的，这里a排在最前面
    for(auto p : ml){
        cout << p.first << ":" << p.second << " "; // a:10 b:20 b:10 c:10 d:10 
    }
    cout << endl;

    // find 查找，只返回一个，要返回多个，使用equal_range
    auto it = ml.find('b');
    cout << "find b:" << it->first << " value: " << it->second << endl; // b 20 find只返回找到的最前面的那个

    // equal_range 返回 pair<iterator,iterator>
    // 前一个 iter是 lower_bound 后面一个iter是upper_bound
    auto it_equal = ml.equal_range('b');
    cout << "it_equal find b: ";
    for(auto it_begin = it_equal.first; it_begin != it_equal.second; it_begin++){
        cout << it_begin->first << ":" << it_begin->second << " "; // b:20 b:10
    }
    cout << endl;


    // erase 3个重载 1：pos 2:key 3:first last
    ml.erase(ml.begin()); // 删除第一个元素,这里把a删除了
    ml.erase('b'); // 删除了2个b
    ml.erase(ml.find('a'), ml.end());

    for(auto p : ml){
        cout << p.first << ":" << p.second << " ";
    }
    cout << endl;
}
int main() {
    // 关联容器 associative containers

    // Map
    cout << "Map--------------------------" << endl;

    // 初始化相关
    // testInit();

    // empty size max_size
    // testCapacity();

    // insert erase swap clear emplace
    // testModifiers();

    // key_comp value_comp
    // testObservers();

    // 测试一些操作，包含 find,count, lower_bound, upper_bound, equal_range
    // testOperations();

    // multimap
    cout << "Multimap--------------------------" << endl;

    // multimap中key可以重复，速度一般比 unordered_multimap要慢，一般用二叉搜索树实现
    // erase
    testMultiMapModifiers();
    return 0;
}
```

