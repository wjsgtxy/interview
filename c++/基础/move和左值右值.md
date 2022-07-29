# [一文带你详细介绍c++中的Move函数](http://www.manongjc.com/detail/23-dciaudkgcqpkulj.html)

本文章向大家介绍一文带你详细介绍c++中的Move函数，主要包括一文带你详细介绍c++中的Move函数使用实例、应用技巧、基本知识点总结和需要注意事项，具有一定的参考价值，需要的朋友可以参考一下。

### 前言

- 在探讨c++11中的Move函数前，先介绍两个概念（左值和右值）

### 左值和右值

首先区分左值和右值

==左值是表达式结束后依然存在的持久对象==(代表一个在内存中占有确定位置的对象)

==右值是表达式结束时不再存在的临时对象==(不在内存中占有确定位置的表达式）

便携方法：对表达式取地址，如果能，则为左值，否则为右值

```c++
int val;
val = 4; // 正确 ①
4 = val; // 错误 ②
```

上述例子中，由于在之前已经对变量val进行了定义，故在栈上会给val分配内存地址，运算符=要求等号左边是可修改的左值，4是临时参与运算的量，一般在寄存器上暂存，运算结束后移除寄存器，故①是对的，②是错的

#### 左值引用

#### 右值引用

### std::move函数

- std::move作用主要可以将一个左值转换成右值引用，从而可以调用C++11右值引用的拷贝构造函数
- std::move应该是针对你的对象中有在堆上分配内存这种情况而设置的，如下

### remove_reference源码剖析

在分析`std::move()`与`std::forward()`之前，先看看`remove_reference`，下面是`remove_reference`的实现：

```c++
template<typename _Tp>
struct remove_reference
{ typedef _Tp   type; };
 
// 特化版本
template<typename _Tp>
struct remove_reference<_Tp&>
{ typedef _Tp   type; };
 
template<typename _Tp>
struct remove_reference<_Tp&&>
{ typedef _Tp   type; };
```

`remove_reference`的作用是去除`T`中的引用部分，只获取其中的类型部分。无论`T`是左值还是右值，最后只获取它的类型部分。

### std::forward源码剖析

- 转发左值

```c++
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{ return static_cast<_Tp&&>(__t); }
```

先通过获得类型type，定义`_t`为左值引用的左值变量，通过`static_cast`进行强制转换。`_Tp&&`会发生引用折叠，当`_Tp`推导为左值引用，则折叠为`_Tp& &&`，即`_Tp&`，当推导为右值引用，则为本身`_Tp&&`，即`forward`返回值与`static_cast`处都为`_Tp&&`。

- 转发右值

```c++
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
{
  static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
        " substituting _Tp is an lvalue reference type");
  return static_cast<_Tp&&>(__t);
}
```

不同于转发左值，`_t`为右值引用的左值变量，除此之外中间加了一个断言，表示当不是左值的时候，也就是右值，才进行`static_cast`转换。

#### std::move()源码剖析

```c++
// FUNCTION TEMPLATE move
template <class _Ty>
_NODISCARD constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
    return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}
```

std::move的功能是：

- 传递的是左值，推导为左值引用，仍旧static_cast转换为右值引用。
- 传递的是右值，推导为右值引用，仍旧static_cast转换为右值引用。
- 在返回处，直接范围右值引用类型即可。还是通过renive_reference获得_Tp类型，然后直接type&&即可。

所以std::remove_reference<_Tp>::type&&，就是一个右值引用，我们就知道了std::move干的事情了。

#### 小结

- 在《Effective Modern C++》中建议：**对于右值引用使用std::move，对于万能引用使用std::forward。**
- std::move()与std::forward()都仅仅做了**类型转换(可理解为static_cast转换)**而已。真正的移动操作是在移动构造函数或者移动赋值操作符中发生的
- 在类型声明当中， “&&” 要不就是一个 rvalue reference ，要不就是一个 universal reference – 一种可以解析为lvalue reference或者rvalue reference的引用。对于某个被推导的类型T，universal references 总是以 T&& 的形式出现。
- 引用折叠是 会让 universal references (其实就是一个处于引用折叠背景下的rvalue references ) 有时解析为 lvalue references 有时解析为 rvalue references 的根本机制。引用折叠只会在一些特定的可能会产生"引用的引用"场景下生效。这些场景包括模板类型推导，auto 类型推导， typedef 的形成和使用，以及decltype 表达式。

### std::move使用场景

- 在实际场景中，右值引用和std::move被广泛用于在STL和自定义类中**实现移动语义，避免拷贝，从而提升程序性能**。 在没有右值引用之前，一个简单的数组类通常实现如下，有`构造函数`、`拷贝构造函数`、`赋值运算符重载`、`析构函数`等。深拷贝/浅拷贝在此不做讲解。

```c++
class Array {
public:
    Array(int size) : size_(size) {
        data = new int[size_];
    }
     
    // 深拷贝构造
    Array(const Array& temp_array) {
        size_ = temp_array.size_;
        data_ = new int[size_];
        for (int i = 0; i < size_; i ++) {
            data_[i] = temp_array.data_[i];
        }
    }
     
    // 深拷贝赋值
    Array& operator=(const Array& temp_array) {
        delete[] data_;
 
        size_ = temp_array.size_;
        data_ = new int[size_];
        for (int i = 0; i < size_; i ++) {
            data_[i] = temp_array.data_[i];
        }
    }
 
    ~Array() {
        delete[] data_;
    }
 
public:
    int *data_;
    int size_;
};
```

该类的拷贝构造函数、赋值运算符重载函数已经通过使用左值引用传参来避免一次多余拷贝了，但是内部实现要**深拷贝**，无法避免。 这时，有人提出一个想法：是不是可以提供一个`移动构造函数`，把被拷贝者的数据移动过来，被拷贝者后边就不要了，这样就可以避免深拷贝了，如：

```c++
class Array {
public:
    Array(int size) : size_(size) {
        data = new int[size_];
    }
     
    // 深拷贝构造
    Array(const Array& temp_array) {
        ...
    }
     
    // 深拷贝赋值
    Array& operator=(const Array& temp_array) {
        ...
    }
 
    // 移动构造函数，可以浅拷贝
    Array(const Array& temp_array, bool move) {
        data_ = temp_array.data_;
        size_ = temp_array.size_;
        // 为防止temp_array析构时delete data，提前置空其data_      
        temp_array.data_ = nullptr;
    }
     
 
    ~Array() {
        delete [] data_;
    }
 
public:
    int *data_;
    int size_;
};
```

这么做有2个问题：

- 不优雅，表示移动语义还需要一个额外的参数(或者其他方式)。
- 无法实现！`temp_array`是个const左值引用，无法被修改，所以`temp_array.data_ = nullptr;`这行会编译不过。当然函数参数可以改成非const：`Array(Array& temp_array, bool move){...}`，这样也有问题，由于左值引用不能接右值，`Array a = Array(Array(), true);`这种调用方式就没法用了。

可以发现左值引用真是用的很不爽，**右值引用的出现解决了这个问题**，在STL的很多容器中，都实现了以**右值引用为参数**的`移动构造函数`和`移动赋值重载函数`，或者其他函数，最常见的如std::vector的`push_back`和`emplace_back`。参数为左值引用意味着拷贝，为右值引用意味着移动。

```c++
class Array {
public:
    ......
 
    // 优雅
    Array(Array&& temp_array) {
        data_ = temp_array.data_;
        size_ = temp_array.size_;
        // 为防止temp_array析构时delete data，提前置空其data_      
        temp_array.data_ = nullptr;
    }
     
 
public:
    int *data_;
    int size_;
};
```

如何使用：

```c++
// 例1：Array用法
int main(){
    Array a;
 
    // 做一些操作
    .....
     
    // 左值a，用std::move转化为右值
    Array b(std::move(a));
}
```

### 实例：vector::push_back使用std::move提高性能

```c++
// 例2：std::vector和std::string的实际例子
int main() {
    std::string str1 = "aacasxs";
    std::vector<std::string> vec;
     
    vec.push_back(str1); // 传统方法，copy
    vec.push_back(std::move(str1)); // 调用移动语义的push_back方法，避免拷贝，str1会失去原有值，变成空字符串
    vec.emplace_back(std::move(str1)); // emplace_back效果相同，str1会失去原有值
    vec.emplace_back("axcsddcas"); // 当然可以直接接右值
}
 
// std::vector方法定义
void push_back (const value_type& val);
void push_back (value_type&& val);
 
void emplace_back (Args&&... args);
```

在vector和string这个场景，加个`std::move`会调用到移动语义函数，避免了深拷贝。

除非设计不允许移动，STL类大都支持移动语义函数，即`可移动的`。 另外，编译器会**默认**在用户自定义的`class`和`struct`中生成移动语义函数，但前提是用户没有主动定义该类的`拷贝构造`等函数(具体规则自行百度哈)。 **因此，可移动对象在<需要拷贝且被拷贝者之后不再被需要>的场景，建议使用**`std::move`**触发移动语义，提升性能。**

还有些STL类是`move-only`的，比如`unique_ptr`，这种类只有移动构造函数，因此只能移动(转移内部对象所有权，或者叫浅拷贝)，不能拷贝(深拷贝)

```c++
std::unique_ptr<A> ptr_a = std::make_unique<A>();

std::unique_ptr<A> ptr_b = std::move(ptr_a); // unique_ptr只有‘移动赋值重载函数‘，参数是&& ，只能接右值，因此必须用std::move转换类型

std::unique_ptr<A> ptr_b = ptr_a; // 编译不通过
```

**std::move本身只做类型转换，对性能无影响。** **我们可以在自己的类中实现移动语义，避免深拷贝，充分利用右值引用和std::move的语言特性。**

```c++
std::vector<int> b(5);
b[0] = 2;
b[1] = 2;
b[2] = 2;
b[3] = 2;

// 此处用move就不会对b中已有元素重新进行拷贝构造然后再放到a中
std::vector<int> a = std::move(b);
```

> 将vector B赋值给另一个vector A，如果是拷贝赋值，那么显然要对B中的每一个元素执行一个copy操作到A，如果是移动赋值的话，只需要将指向B的指针拷贝到A中即可，试想一下如果vector中有相当多的元素，那是不是用move来代替copy就显得十分高效了呢？建议看一看Scott Meyers 的==Effective Modern C++==，里面对移动语义、右值引用以及类型推导进行了深入的探索

### 万能引用

首先，我们先看一个例子

```c++
#include <iostream>

using std::cout;
using std::endl;


template<typename T>
void func(T& param) {
    cout << param << endl;
}


int main() {
    int num = 2019;
    func(num);
    return 0;
}
```

这样例子的编译输出不存在什么问题，但是如果修改成下面的调用方式呢？

```c++
int main(){
    func(2019);
    return 0;
}
```

编译器会产生错误，因为上面的模板函数只能接受左值或者左值引用（左值一般是有名字的变量，可以取到地址的），我们当然可以重载一个接受右值的模板函数，如下也可以达到效果

```c++
template<typename T>
void func(T& param) {
    cout << "传入的是左值" << endl;
}
template<typename T>
void func(T&& param) {
    cout << "传入的是右值" << endl;
}

int main() {
    int num = 2019;
    func(num);
    func(2019);
    return 0;
}
```

输出结果

```html
传入的是左值
传入的是右值
```

第一次函数调用的是左值得版本，第二次函数调用的是右值版本。但是，有没有办法只写一个模板函数即可以接收左值又可以接收右值呢？

C++11中有万能引用（Universal Reference）的概念：使用`T&&`类型的形参既能绑定右值，又能绑定左值

但是注意了：**只有发生类型推导的时候，T&&才表示万能引用（如模板函数传参就会经过类型推导的过程）**；否则，表示右值引用

所以，上面的案例我们可以修改为

```c++
template<typename T>
void func(T&& param) {
    cout << param << endl;
}


int main() {
    int num = 2019;
    func(num);
    func(2019);
    return 0;
}
```

### 引用折叠

万能引用说完了，接着来聊引用折叠（Reference Collapse），因为完美转发（Perfect Forwarding）的概念涉及引用折叠。一个模板函数，根据定义的形参和传入的实参的类型，我们可以有下面四中组合：

- 左值-左值 T& & # 函数定义的形参类型是左值引用，传入的实参是左值引用

```c++
template<typename T>
void func(T& param) {
    cout << param << endl;
}

int main(){
    int num = 2021;
    int& val = num;
    func(val);
}
```

- 左值-右值 T& && # 函数定义的形参类型是左值引用，传入的实参是右值引用

```c++
template<typename T>
void func(T& param) {
    cout << param << endl;
}

int main(){
    int&& val = 2021;
    func(val);
}
```

- 右值-左值 T&& & # 函数定义的形参类型是右值引用，传入的实参是左值引用

```c++
template<typename T>
void func(T&& param) {
    cout << param << endl;
}

int main(){
    int num = 2021;
    int& val = num;
    func(val);
}
```

- 右值-右值 T&& && # 函数定义的形参类型是右值引用，传入的实参是右值引用

```c++
template<typename T>
void func(T&& param) {
    cout << param << endl;
}

int main(){
    int&& val = 4;
    func(val);
}
```

但是C++中不允许对引用再进行引用，对于上述情况的处理有如下的规则：

所有的折叠引用最终都代表一个引用，要么是左值引用，要么是右值引用。规则是：**如果任一引用为左值引用，则结果为左值引用。否则（即两个都是右值引用），结果才是右值引用**

即就是前面三种情况代表的都是左值引用，而第四种代表的右值引用

### 完美转发

下面接着说完美转发（Perfect Forwarding），首先，看一个例子

```c++
#include <iostream>

using std::cout;
using std::endl;

template<typename T>
void func(T& param) {
    cout << "传入的是左值" << endl;
}

template<typename T>
void func(T&& param) {
    cout << "传入的是右值" << endl;
}

template<typename T>
void warp(T&& param) {
    func(param);
}

int main() {
    int num = 2019;
    warp(num);
    warp(2019);
    return 0;
}
```

输出的结果

```c++
传入的是左值
传入的是左值
```

是不是和预期的不一样，下面我们来分析一下原因：

`warp()`函数本身的形参是一个万能引用，即可以接受左值又可以接受右值；第一个`warp()`函数调用实参是左值，所以，`warp()`函数中调用`func()`中传入的参数也应该是左值；第二个`warp()`函数调用实参是右值，根据上面所说的引用折叠规则，warp()`函数接收的参数类型是右值引用，那么为什么却调用了调用`func()的左值版本了呢？这是因为在`warp()`函数内部，右值引用类型变为了左值，因为参数有了名称，我们也通过变量名取得变量地址

那么问题来了，怎么保持函数调用过程中，变量类型的不变呢？这就是我们所谓的“变量转发”技术，在C++11中通过`std::forward()`函数来实现。我们来修改我们的`warp()`函数如下：

```c++
template<typename T>
void warp(T&& param) {
    func(std::forward<T>(param));
}
```

则可以输出预期的结果

```c++
传入的是左值
传入的是右值
```

### 参考博文

现代C++之万能引用、完美转发、引用折叠(万字长文)：https://blog.csdn.net/guangcheng0312q/article/details/103572987

C++ 中的「移动」在内存或者寄存器中的操作是什么，为什么就比拷贝赋值性能高呢？：https://www.zhihu.com/question/55735384

一文读懂C++右值引用和std::move：https://zhuanlan.zhihu.com/p/335994370

原文地址：https://www.cnblogs.com/shadow-lr/p/14748272.html