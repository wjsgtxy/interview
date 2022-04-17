## Cmake Tutorial

### Step1

最简单的cmake文件，只要三行就可以了 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
# set the project name
project(Tutorial)
# add the executable
add_executable(Tutorial tutorial.cxx)
```

去Help/guide/tutorial目录 mkdir build，将构建的项目放在build目录

```bash
mkdir build
cd build
# 这个构建的目录要有CMakeLists.txt文件才可以
cmake ../ 
#直接在本目录构建也可以
cmake --build .
```

添加version

```
project(Tutorial VERSION 1.0)
```

向源代码传输版本号的方法：

```
# 添加配置文件
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

在 CMakeLists.txt最后添加搜索目录，因为配置文件会被放到 binary tree（TODO）

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

源文件目录创建文件 TutorialConfig.h.in, cmake的时候 @XXX@ 就会被替换，生成上面定义的TutorialConfig.h头文件，然后源文件中引用这个头文件，就可以使用这两个define变量了

```cmake
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```



#### 设置C++版本

```cmake
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```



### Step2

子目录 MathFunctions/CMakeLists.txt

```cmake
# 注意，这里添加的是库文件 add_library,不是executable，编译之后，Math子目录里面会生成libMathFunctions.a的库文件
# 默认应该是STATIC静态库
add_library(MathFunctions mysqrt.cxx) 

# 添加动态库
add_library(MathFunctions SHARED mysqrt.cxx)
```

```cmake
# add the MathFunctions library 添加子目录
add_subdirectory(MathFunctions)

# dz, 注意要链接上需要的库
target_link_libraries(Tutorial PUBLIC MathFunctions)

# 添加头文件的目录
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/MathFunctions"
                          )
```



添加option可选项，在cmake-gui中到时候也会让选择

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)
```

有了这个选项，cmake可以使用这个选项来选择性的加载库和头文件，这在多模块多组件的开发中很有用

list命令介绍：https://www.jianshu.com/p/89fb01752d6f

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions) # list子命令APPEND用于将元素追加到列表
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()
# 动态添加库
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES} # 添加头文件
                           )
```

TutorialConfig.h.in文件中：这样生成的TutorialConfig.h头文件中就会有 #define USE_MYMATH 宏

```cmake
#cmakedefine USE_MYMATH
```



cmake的时候，如果是命令行，-D配置option可选项的值

```bash
cmake ../Step2 -DUSE_MYMATH=OFF
```



其他：

target_link_libraries 的 PRIVATE，PUBLIC 和 INTERFACE 的区别

| 情况                             | 使用参数  |
| -------------------------------- | --------- |
| 只有源文件（.cpp）中包含了库文件 | PRIVATE   |
| 只有头文件（.hpp）中包含了库文件 | INTERFACE |
| 源文件和头文件都包含了库文件     | PUBLIC    |

https://stackoverflow.com/questions/26037954/cmake-target-link-libraries-interface-dependencies

> If you are creating a shared library and your source `cpp` files `#include` the headers of another library (Say, `QtNetwork` for example), but your header files don't include `QtNetwork` headers, then `QtNetwork` is a `PRIVATE` dependency.
>
> If your source files and your headers include the headers of another library, then it is a `PUBLIC` dependency.
>
> If your header files other than your source files include the headers of another library, then it is an `INTERFACE` dependency.
>
> // PUBLIC 和 INTERFACE 依赖项的其他生成属性将传播到使用库。
>
> Other build properties of `PUBLIC` and `INTERFACE` dependencies are propagated to consuming libraries. <http://www.cmake.org/cmake/help/v3.0/manual/cmake-buildsystem.7.html#transitive-usage-requirements>