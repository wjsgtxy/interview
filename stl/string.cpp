#include<string>
#include <cstring>
#include<iostream>

using namespace std;

void testConvert(){
    // stoi, stol, stoul, stoll, stoull, stof, stod, stold
    // int stoi (const string&  str, size_t* idx = 0, int base = 10);
    // idx指针，非空的话返回非数字的下一个字符 base说明str是多少进制的
    string str_dec = "2001, A Space 0dyssey";
    string str_hex = "40c3";
    string str_bin = "-10010110001";
    string str_auto = "0x7f";
    string::size_type idx;

    int i_dec = stoi(str_dec, &idx); // idx 指向 ',' 第一个非数字字符
    int i_hex = stoi(str_hex, nullptr, 16);
    int i_bin = stoi(str_bin, nullptr, 2);
    int i_auto = stoi(str_auto, nullptr, 0); // 127

    cout << str_dec << ": " << i_dec << " and [" << str_dec.substr(idx) << "]\n";
    cout << str_hex << ": " << i_hex << endl;
    cout << str_bin << ": " << i_bin << endl;
    cout << str_auto << ": " << i_auto << endl;

    // to_string 将数字转换成字符
    string str = to_string(3.14159);
    cout << "pi: " << str.substr(0, 4) << endl; // 3.14
}

void testCapacity(){
    string str = "hello world";
    // size 和 length 返回一样的值
    // size: 11 length: 11 capacity: 15
    cout << "size: " << str.size() << " length: " << str.length() << " capacity: " << str.capacity() << endl;

    // resize(n) 或者 resize(n, char c);
    // reserve：设置capacity最小值
    // shrink_to_fit: capacity减少到size大小
}

void testModifiers(){
    string name ("John");
    string family ("Smith");
    name += " K. ";         // c-string
    name += family;         // string
    name += '\n';           // character

    cout << name;

    // void push_back (char c);
    string str = "hell";
    str.push_back('o');


    // replace 替换str部分,有多个重载，前两个参数一致，后面的str可换成多个初始化的那种重载
    // string& replace (size_t pos, size_t len, const string& str);
    // 从 pos 位置，替换掉len长度的字符，换成第三个参数的str
    string base = "this is a test string";
    base.replace(9, 5, "n example");
    cout << "base: " << base << endl; // this is an example string
}

void teseOperation(){
    // c_str：返回内部的char*  和 data()一样
    string str("Please split this sentence into tokens");
    char* cstr = new char[str.length() + 1]; // 加一个用来放 \0
    strcpy(cstr, str.c_str());

    char* p  = strtok(cstr, " "); // todo
    while(p != 0){
        cout << p << endl;
        p = strtok(NULL, " ");
    }

    delete[] cstr;

    // copy: 从str的pos位置，复制len长度的字符到char* s中，要手动自己加 \0
    // size_t copy (char* s, size_t len, size_t pos = 0) const; // pos有默认值0，所以放在最后了
    char buffer[20];
    string test("Test string...");
    size_t length = test.copy(buffer, 6, 5);
    buffer[length] = '\0';
    cout << "buffer: " << buffer << endl;
}

void testFind(){
    // find: 4个重载, 都返回size_t, 即第一个找到的位置
    // 1.find(str, pos=0) 2.find(char* s, pos=0) 
    // 3.find(char*s, pos, n) 4.find(char, pos=0)
    string str("There are two needles in this haystack with needles.");
    string str2("needles");

    // 没有找到返回 static const size_t npos = -1;
    // npos = -1 ，表示len的时候，意味着 until the end of the string, 用于return value，表示no matches
    size_t found = str.find(str2);
    if(found != string::npos){
        cout << "first 'needle' found at: " << found << '\n';
    }

    // 只查找char* s的前几个字符: 这里从found+1 后面继续查找 needle
    found = str.find("needles are small", found+1, 6);
    cout << "second 'needle' found at: " << found << '\n';

    // 查找替换，很有用， 这里替换掉第一个needle !!!
    str.replace(str.find(str2), str2.length(), "preposition"); 
    cout << str << '\n'; // There are two preposition in this haystack with needles.

    // find_first_of 和find的4个重载一样，只是只要找到str中的一个字符就满足了
    str = "Please, replace the vowels in this sentence by asterisks.";
    found = str.find_first_of("aeiou");
    while (found != string::npos)
    {
        str[found]='*';
        found=str.find_first_of("aeiou", found+1); // 继续查找下一个元音字母
    }

    cout << str << '\n';
    // 还有类似的 find_last_of find_fist_not_of find_last_not_of
}

int main(){
    // string
    // Convert from strings
    testConvert();

    // Capacity: size, length, max_size, resize, capacity, reserve, clear, empty, shrink_to_fit
    testCapacity();

    //Modifiers: operator+=, append, push_back, assign, insert, erase, replace, swap, pop_back
    testModifiers();

    //Operation: c_str, data
    teseOperation();

    // string 查找
    testFind();
    return 0;
}