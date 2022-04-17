#include<iostream>
#include<vector>
#include<list>
#include<cmath>
#include <array>

using namespace std;
void printVector(vector<int> &v){
    for(int i : v){
        cout << i << " ";
    }
    cout << endl;
}

void printList(list<int> &l){
    for(auto it = l.begin(); it!=l.end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}

void printListDouble(list<double> &l){
    for(auto it = l.begin(); it!=l.end(); it++){
        cout << *it << " ";
    }
    cout << endl;
}

void testCapacity(){
    vector<int> v;
    // size 返回包含的元素个数
    cout << "size:" << v.size() << " capacity: " << v.capacity() << endl; // size:0 capacity:0

    for(int i=0; i<10; i++) v.push_back(i);
    cout << "size:" << v.size() << " capacity: " << v.capacity() << endl; // size:10 capacity:16

    // max_size 返回最大的容量
    cout << "max_size: " << v.max_size() << endl; // 4611686018427387903

    printVector(v);
    
    // resize to n
    // 1. n>size 不足的补充0
    v.resize(12);
    printVector(v);
    cout << "resize to 12 after size:" << v.size() << " capacity: " << v.capacity() << endl; // size:12 capacity: 16

    // 2. n<size 只保留前n个元素
    v.resize(5);
    printVector(v);
    cout << "resize to 5 after size:" << v.size() << " capacity: " << v.capacity() << endl; // size:5 capacity: 16

    // 3. 不足的补充自定义的value
    v.resize(10, 99); // 用99填充后面的5个
    printVector(v);
    cout << "resize to 10 after size:" << v.size() << " capacity: " << v.capacity() << endl; // size:10 capacity: 16

    // reserve: Minimum capacity for the vector. 改变capacity
    v.reserve(32);
    cout << "reserve to 32 after size:" << v.size() << " capacity: " << v.capacity() << endl; // size:10 capacity: 32

    // shrink_to_fit 将capacity变成size大小，最大化空间利用率
    v.shrink_to_fit();
    cout << "shrink_to_fit after size:" << v.size() << " capacity: " << v.capacity() << endl; // size:10 capacity: 10
}

void testAccess(){
    cout << "testAccess---------------" << endl;
    // data 返回vector内部的array
    vector<int> v{1,2,3,4,5};
    int* p = v.data();
    printVector(v);

    *p = 10;
    ++p;
    *p = 20;
    p[2] = 100; // 注意，上面将p加了1，那么p0就不在是v0了，而是指向了v1，所以p2就是v3了！！！

    printVector(v); //10 20 3 100 5 

    // front back
    cout << "front: " << v.front() << " back: " << v.back() << endl;
}

void testModifiers(){
    cout << "testModifiers-------------" << endl;
    vector<int> v{1,2,3,4,5,6,7}; // 列表初始化

    // assign
    v.assign(6,10); // v的内容变成6个10了
    printVector(v);

    // assign 初始化列表
    vector<int> v2;
    v2.assign({1,2,3,4,5,6,7}); 
    printVector(v2);

    // assign可以用来截取自身
    v2.assign(v2.begin()+1, v2.end()-1); // 2 3 4 5 6 
    printVector(v2);

    /*
    single element (1)	
    iterator insert (const_iterator position, const value_type& val);
    fill (2)	
    iterator insert (const_iterator position, size_type n, const value_type& val);
    range (3)	
    template <class InputIterator>
    iterator insert (const_iterator position, InputIterator first, InputIterator last);
    move (4)	
    iterator insert (const_iterator position, value_type&& val);
    initializer list (5)	
    iterator insert (const_iterator position, initializer_list<value_type> il);
    */

    // insert 在指定位置插入
    vector<int> v3(3,10);
    auto it = v3.begin();
    
    // 注意，下面这种方式插入不行，因为it迭代器失效了！！！
    // v3.insert(it, 20); // 现在头部插入一个20
    // v3.insert(it, 2, 30); // 在前面插入2个30 

    it = v3.insert(it, 20); // 注意：返回An iterator that points to the first of the newly inserted elements.
    v3.insert(it, 2, 30); // 30 30 20 10 10 10 
    printVector(v3);

    /*
    iterator erase (const_iterator position);
    iterator erase (const_iterator first, const_iterator last);
    */
   v3.erase(v3.begin());
   v3.erase(v3.begin(), v3.begin()+3);
   printVector(v3);

   v3.clear();

   // emplace 和insert很像
   v3.push_back(1);
   v3.push_back(2);
   v3.emplace(v3.begin(), 3);
   v3.emplace_back(4);
   printVector(v3); // 3 1 2 4
}

// 条件函数
bool single_digit(const int& value){
    return value > 10;
}
// 条件类
struct is_odd{
    bool operator()(const int& value){
        return  value%2 == 1;
    }
};

void testListOperation1(){
    cout << "list unique api-----------" << endl;

    // Splice: transfer elements from list to list （splice英文 绞接两段绳子的意思）
    // 3个重载函数 1:entire list 2:single element 3:range
    // 1：splice(pos, list) 2:splice(pos, list, iter) 3:splice(pos, list, first, last)
    list<int> l1{1, 2, 3, 4};
    list<int> l2{10, 20, 30};

    auto it = l1.begin();
    ++it; // points to 2

    // 1, entire，l1把l2所有的元素都弄过来了
    l1.splice(it, l2); // l1: 1 10 20 30 2 3 4  l2: empty it任然指向元素2！

    // 2, single , 把l1的元素2弄过来
    l2.splice(l2.begin(), l1, it); // l1:1 10 20 30 3 4  l2:2 it现在失效了

    // 3, range
    it = l1.begin();
    advance(it, 3); // it points to 30
    // 把l1的 后面部分交换到前面部分
    l1.splice(l1.begin(), l1, it, l1.end()); // l1:30 3 4 1 10 20
    printList(l1);

    // remove(value) 移除元素
    // 和erase不同，erase是按照pos来移除的，这里是按照value移除
    l1.remove(10);

    // void remove_if (Predicate pred); 条件移除
    l1.remove_if(single_digit); // 大于10的都移除了
    cout << "after remove if-----------" << endl;
    printList(l1); // l1: 3 4 1

    l1.remove_if(is_odd()); // 奇数都移除了
    printList(l1); // l1: 4
}
// a binary predicate implemented as a function:
bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

// a binary predicate implemented as a class:
struct is_near {
  bool operator() (double first, double second)
  { return (fabs(first-second)<5.0); }
};

// compare only integral part:
bool mycomparison (double first, double second)
{ return ( int(first)<int(second) ); }

void testListOperation2(){
    // unique: 去掉重复元素，注意，只能去掉相邻的相同元素的后一个，所以要先排序
    // 2个重载 1：unique() 2:unique(binary_pred) 自定义重复的判断函数，返回true表示相同
    list<double> li = {12.15,  2.72, 73.0,  12.77,  12.77, 3.14,
                       12.77, 73.35, 72.25, 15.3,  72.25};
    li.unique();
    printListDouble(li); // 12.15 2.72 73 12.77 3.14 12.77 73.35 72.25 15.3 72.25 只把相邻的12.77去掉了

    li.sort(); // 先排序
    li.unique();
    printListDouble(li); // 2.72 3.14 12.15 12.77 15.3 72.25 73 73.35

    // 去除整数部分一样的值
    li.unique(same_integral_part);
    printListDouble(li); // 2.72 3.14 12.15 15.3 72.25 73

    // 去除相邻的值（相邻的定义看函数）
    li.unique(is_near());
    printListDouble(li); // 2.72 12.15 72.25

    list<double> ll = {1, 4, 5, 6, 7, 8, 9, 10, 11, 13};
    ll.unique(is_near());
    printListDouble(ll); // 1 6 11 注意：相当于6是和1比较的！！！相当于4去掉之后就没有了，后面的继续和1比较

    // sort 2个重载 1：sort() 2:sort(comp) 自定义比较函数
    // reverse 翻转链表
    ll.reverse();
    printListDouble(ll); // 11 6 1

    // merge：合并两个有序数组，无序的合并使用 splice
    // 2个重载 1：merge(list) 2：merge(list, comp)自定义比较函数
    list<double> first{3.1, 2.2, 2.9};
    list<double> second{3.7, 7.1, 1.4};
    first.sort();
    second.sort();

    first.merge(second); 
    printListDouble(first); // 1.4 2.2 2.9 3.1 3.7 7.1
    printListDouble(second); // second 已经变成empty了，所以的元素被移动了！没有元素的构造！！

    second.push_back(2.1); 
    first.merge(second, mycomparison); // 注意，比较函数只比较了整数部分，所以2.1排在了2.2, 2.9后面，3.1前面
    printListDouble(first); // 1.4 2.2 2.9 2.1 3.1 3.7 7.1 
}

void testArray(){
    cout << "testArray-------------------------" << endl;
    // array: c++的数组
    // 定义的时候要给出长度，array长度保持不变，不能更改
    // array存储效率很高，因为除了元素和长度，基本什么信息都不存
    // 交换两个array效率很低，不要这么操作
    array<int, 5> marr = {2, 6, 77, 34, 50};

    // 也有8个迭代器
    for(auto it = marr.begin(); it!=marr.end(); it++){
        cout << *it << " ";
    }
    cout << "front: " << marr.front() << " back: " << marr.back() << endl;
    // 填充
    marr.fill(6);

    for(int i=0; i < marr.size(); i++){
        cout << marr.at(i) << " ";
    }
}

int main(){
    // capacity: size, max_size, resize, capacity, empty, reserve, shrink_to_fit
    // testCapacity();

    // operator[], at, data, front, back
    // testAccess();

    // assign, push_back, pop_back, insert, erase, swap, chear, emplace, emplace_back
    // testModifiers();

    // list是双向链表，forward_list是单向链表
    // list缺点就是不能按照位置获取元素，插入效率高
    // member func和deque很像，但是list有一些独有的api
    // testListOperation1();

    // sort, unique, 
    // testListOperation2();

    // array
    testArray();
    return 0;
}