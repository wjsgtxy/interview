#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

void testUnMap()
{
    // access: [] 和 at(key)
    unordered_map<string, int> m = { { "Mars", 3000 }, { "Saturn", 60000 }, { "Jupiter", 70000 } };
    m.at("Mars") = 3396;
    for (auto &x : m) {
        cout << x.first << ": " << x.second << endl;
    }

    // buckets
    // bucket_count: hash表中有多少个桶
    int cnt = m.bucket_count();
    cout << "bucket_count: " << cnt << endl;

    // bucket_size(i): 第i个桶有多少个元素
    for (int i = 0; i < cnt; i++) {
        cout << "bucket #" << i << " has " << m.bucket_size(i) << " elements.\n";
    }

    // bucket(key): 查看key在哪个桶里面, 桶的编号：0 - (bucket_count-1)
    cout << "Mars is in bucket no: " << m.bucket("Mars") << endl;
    cout << "Saturn is in bucket no: " << m.bucket("Saturn") << endl;

    // Hash policy
    cout << "load factor:" << m.load_factor() << endl; // 我这里0.6

    // reserve(n): 设置hash表至少容纳n个元素所需桶的最适合数量(根据load_factor来计算) n要大于现有元素数量，否则无效
    m.reserve(6);
    cout << "6 ele has bucket_count: " << m.bucket_count() << endl; // 7

    // rehash(n): 重新hash，调整桶的数量大于等于n
    m.rehash(10);                                                       // 设置10个桶
    cout << "after rehash, bucket_count: " << m.bucket_count() << endl; // 11: 比10大的第一个质数

    // unordered_map 和 unordered_set begin迭代器比较特殊!!!
    // 有2个重载，可以传入一个n, 表示从第几个桶里面迭代
    for (int i = 0; i < m.bucket_count(); i++) {
        cout << "bucket " << i << " has element: ";
        for (auto it = m.begin(i); it != m.end(i); it++) {
            cout << it->first << ":" << it->second << ", ";
        }
        cout << endl;
    }
}

void testUnMultiMap(){
    cout << "testUnMultiMap--------------------" << endl;
    // 其他和unordered_map一样，唯一要注意的是 这个key可以重复
    // equal_range
    typedef unordered_multimap<string, string> smap;
    smap smp;
    smp = { {"orange","FL"}, {"strawberry","LA"}, {"strawberry","OK"}, {"pumpkin","NH"}};
    auto range = smp.equal_range("strawberry");
    cout << "Entries with strawberry: ";
    // 有一下几个member: key_type, mapped_type, value_type
    // value_type 定义为 pair<const key_type, mapped_type>
    // value_type表示整个值的类型，所以这里是pair，在其他stl容器中也是这么定义的
    for_each(range.first, range.second, [](smap::value_type &x){
        cout << " " << x.second;
    });
    cout << endl;
}

void testUnSet()
{
    cout << "testUnSet---------------------------" << endl; 
    unordered_set<string> myset = { "Mercury", "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune" };
    cout << "myset contains:";
    for (auto it = myset.begin(); it != myset.end(); ++it)
        cout << " " << *it;
    cout << endl;

    // 和上面map一样，迭代器有重载，表示第几个桶里面的set 元素
    cout << "myset's buckets contain:\n";
    for (unsigned i = 0; i < myset.bucket_count(); ++i) {
        cout << "bucket #" << i << " contains:";
        for (auto local_it = myset.begin(i); local_it != myset.end(i); ++local_it)
            cout << " " << *local_it;
        cout << endl;
    }
}

int main()
{
    // unordered_map
    // key唯一, 无序， hash(key)之后 organized into buckets
    // 通过key获取元素比map快， 但是range iteration效率比较低
    // 它的it也指向 pair<const Key, T> it->fist, it->second来获取key和value
    testUnMap();

    // unordered_multimap 没有单独的头文件，和unordered_map使用一个头文件
    testUnMultiMap();

    testUnSet();
    return 0;
}