```c++
#include<iostream>
#include<queue>
#include<vector>
#include<functional>
#include<deque>
using namespace std;

void printVector(vector<int>& v){
    for(int i : v){
        cout << i << " ";
    }
    cout << endl;
}

void testQueue(){
    // queue<int> q(10); // queue没有这种初始化方式
    queue<int> q;
    cout << "empty? " << q.empty() << endl;
    q.push(10);
    q.push(20);
    cout << "front: " << q.front() << " back: " << q.back() << endl;

    q.emplace(30);
    cout << "front: " << q.front() << " back: " << q.back() << endl;
    
    cout << "size:" << q.size() << endl;
    q.pop();
    cout << "size:" << q.size() << endl;

}

void testPriority_queue(){
    priority_queue<int> pq;
    pq.push(10);
    pq.push(30);
    pq.emplace(20);
    pq.emplace(50);
    pq.push(40);

    cout << "top:" << pq.top() << endl; // 50
    pq.pop();
    cout << "top:" << pq.top() << endl; // 40
}

// struct 重载运算符
struct cmp1{
    bool operator()(vector<int>&a, vector<int>&b){
        return a[0] > b[0];
    }
};

// class 重载运算符
class cmp2{
    public:
        bool operator()(vector<int>&a, vector<int>&b){
            return a[0] > b[0];
        }
};

// 3.定义函数
bool cmp3(vector<int>&a, vector<int>&b){
    return a[0] > b[0];
}

void testComp(){
    cout << "test comp------------" << endl;
    // 自定义优先队列的比较函数
    // 默认是less<T>, 即a<b返回true，则a在b的前面，而pop弹出的是优先队列的last，所以是大顶堆
    int ia[] = {10,60,50,20};
    priority_queue<int> pq2(ia, ia+4);
    cout << "less<int> top:" << pq2.top() << endl; // top:60 默认是less<T>

    priority_queue<int, vector<int>, greater<int>> pq3(ia, ia+4); // 小顶堆
    cout << "greater<int> top:" << pq3.top() << endl; // top:10


    vector<int> v1 = {10, 30};
    vector<int> v2 = {30, 60};
    vector<int> v3 = {20, 40};
    // priority_queue中的三个参数，后两个可以省去，因为有默认参数，1：数据类型，2：容器类型，3：比较函数
    // 不过如果有第三个参数的话，必定要写第二个参数。

    // 传入函数对象
    // priority_queue<vector<int>, vector<vector<int>>, cmp1> pq4; // ok
    // priority_queue<vector<int>, vector<vector<int>>, cmp2> pq4; // ok

    // 传入函数指针
    // decltype()用于获取函数指针类型，test是函数首地址，类型是void() &test是一个指针，类型是void(*)() 因此两者所代表的地址值是一样的，但是类型不一样
    // priority_queue<vector<int>, vector<vector<int>>, decltype(cmp3)> pq4(cmp3); // 编译失败
    // priority_queue<vector<int>, vector<vector<int>>, decltype(&cmp3)> pq4(&cmp3); // OK，最后这里传入cmp3或者&cmp3都可以！但是不能不传
    priority_queue<vector<int>, vector<vector<int>>, decltype(cmp3)*> pq4(cmp3); // 这样也是ok的，decltype后面添加一个*表示是一个指针


    // lambda表达式: 也属于传入函数指针
    auto cmp4 = [](vector<int>& a, vector<int>& b){ // 注意 cmp4类型写auto才编译成功，写bool不成功
        return a[0] > b[0];
    };
 
    // priority_queue<vector<int>, vector<vector<int>>, decltype(&cmp4)> pq4(); // 编译失败
    // priority_queue<vector<int>, vector<vector<int>>, decltype(cmp4)> pq4(cmp4); // ok

    // function包装lambda表达式, 要include<functional>
    function<bool(vector<int>&, vector<int>&)> cmp5(cmp3);
    // priority_queue<vector<int>, vector<vector<int>>, function<bool(vector<int>&, vector<int>&)>> pq4(cmp5); // OK

    pq4.push(v1);
    pq4.push(v2);
    pq4.push(v3);

    // printVector(pq4.pop()); // 直接pop弹不出最顶元素
    vector<int> v = pq4.top();
    printVector(v); // v1:10 30
    pq4.pop(); 

    v = pq4.top();
    printVector(v); // v3:20 40
}

void testDeque(){
    cout << "testDequeInit--------------" << endl;
    // 1.empty 2.fill 3.range 4.copy 5.move 6.initializer list
    deque<int> dq1;
    deque<int> dq2(2,10);
    deque<int> dq3(dq2.begin(), dq2.end());
    deque<int> dq4(dq2);
    deque<int> dq5(move(dq2));
    deque<int> dq6{1,2,3};

    for(auto it = dq6.begin(); it!=dq6.end(); it++){
        cout << *it << " ";
    }
    cout << endl;

    // Capacity
    // size, max_size, resize, empty, shrink_to_fit
    // resize(n) 和 resize(n, val) 和 vector 一样，多余的用val填充
    // shrink_to_fit优化存储空间，因为可能有冗余的空间一遍用来提高移动效率
    // 没有capacity函数

    // Access
    // opeartor[] 注意 deque可以直接下标访问！！！
    // at, front, back at和[]的区别在于会校验下标范围是否超出

    // Modifiers
    // assign, push_back, push_front, pop_back, pop_front, insert, erase, swap, clear
    // emplace, emplace_front, emplace_back
    // assign 就是重新分配内容，和初始化一样，有3个重载: range, fill, initializer list
    // insert 就是指定一个位置，然后和数据，包含：singal value, fill, range, move, inline
    // emplace和insert一样，指定一个位置，和元素的构造参数，emplace_back和front就不要位置了
    // erase 有两个重载：position和range

}

void testStack(){
    // stack lifo 是 container adaptors 
    // 底层容器只要实现 empty, size, back, push_back, pop_back就能做stack底层容器，符合条件的有vector, deque, list
    // stack默认的是deque实现的
    // member func: empty, size, top, push, emplace, pop, emplace
}

int main(){
    // queue的函数较少：empty, size, front, back, push, pop, emplace, swap
    // testQueue();

    // 优先队列，好像底层可以用vector实现 empty,size,top,push,emplace,pop
    // 头文件queue就可以了，也包含优先队列
    // testPriority_queue();

    // 自定义比较函数
    // testComp();

    // 双端队列 deque(发音deck)
    testDeque();

    // stack
    testStack();
    return 0;
}
```

