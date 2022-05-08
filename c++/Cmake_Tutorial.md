# Cmake Tutorial

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



## Step3: 添加库的usage requirements

使用依赖可以更好的控制一个库的传递属性，相关命令：

- [`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions)
- [`target_compile_options()`](https://cmake.org/cmake/help/latest/command/target_compile_options.html#command:target_compile_options)
- [`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories)
- [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries)



INTERFACE表示库的使用者需要，但是我们自己不需要

```bash
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
```

在math库添加了上面这个之后，库的使用者就不用自己添加库的头文件依赖了，库的使用者中target_include_directories就可以删除掉math库的头文件目录了。



## Step4：install and test

对于一个库来说，我们要install 库文件lib和头文件header file，对于可执行文件来说，要安装可执行文件bin和配置文件configure header。

```bash
# 库文件CmakeLists.txt
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)

# 可执行文件
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include
  )
```

```bash
# install命令
cmake --install .
# 更改安装目录
cmake --install . --prefix "/home/myuser/installdir"
```



test跳过 TODO



## Step5: 系统能力检查

```bash
# does this system provide the log and exp functions?
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if(NOT (HAVE_LOG AND HAVE_EXP))
  unset(HAVE_LOG CACHE)
  unset(HAVE_EXP CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m") # 然后看库libm里面有没有需要的符号
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(HAVE_LOG AND HAVE_EXP)
    target_link_libraries(MathFunctions PRIVATE m)
  endif()
endif()
```

如果找到了log和exp的符号，就可以定义宏了：

```bash
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()
```



## Step6:添加自定义命令

```bash
add_executable(MakeTable MakeTable.cxx) # 添加了可执行文件
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h # 输出文件
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h # 执行的命令，注意MakeTable是可执行文件，后面的是它的参数（这里表示MakeTale产生的文件的名字和位置）
  DEPENDS MakeTable # 依赖
  )
```

```bash
add_library(MathFunctions
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h # 添加对Table.h的依赖，这样才会去执行上面添加的命令
            )
```

```bash
# MathFunctions/CMakeLists.txt
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE ${CMAKE_CURRENT_BINARY_DIR} # private引入上面的MakeTable的可执行文件目录
          )
```



## Step7: 打包

使用CPack打包，这个要写在CMakeLists.txt的最下面

```bash
include(InstallRequiredSystemLibraries) #这个会添加当前平台所有需要的运行时库
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}") # 设置变量
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)
```

```bash
cpack
```



## Step9:选择静态库还是动态库

```bash
  # library that just does sqrt
  add_library(SqrtLibrary STATIC # 指定创建一个静态库
              mysqrt.cxx
              ${CMAKE_CURRENT_BINARY_DIR}/Table.h # 依赖的文件
              )

  # state that we depend on our binary dir to find Table.h
  target_include_directories(SqrtLibrary PRIVATE
                             ${CMAKE_CURRENT_BINARY_DIR}
                             )
  # state that SqrtLibrary need PIC when the default is shared libraries
  set_target_properties(SqrtLibrary PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS} #BUILD_SHARED_LIBS这个参数是ON todo
                        )
                        
  target_link_libraries(MathFunctions PRIVATE SqrtLibrary) #mathF对sqrt库的依赖
```



## Step10: 添加生成表达式

> [`Generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) are evaluated during build system generation to produce information specific to each build configuration.
>
> [`Generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) are allowed in the context of many target properties, such as [`LINK_LIBRARIES`](https://cmake.org/cmake/help/latest/prop_tgt/LINK_LIBRARIES.html#prop_tgt:LINK_LIBRARIES), [`INCLUDE_DIRECTORIES`](https://cmake.org/cmake/help/latest/prop_tgt/INCLUDE_DIRECTORIES.html#prop_tgt:INCLUDE_DIRECTORIES), [`COMPILE_DEFINITIONS`](https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_DEFINITIONS.html#prop_tgt:COMPILE_DEFINITIONS) and others. They may also be used when using commands to populate those properties, such as [`target_link_libraries()`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries), [`target_include_directories()`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories), [`target_compile_definitions()`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions) and others.
>
> [`Generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) may be used to enable conditional linking, conditional definitions used when compiling, conditional include directories and more. The conditions may be based on the build configuration, target properties, platform information or any other queryable information.
>
> There are different types of [`generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) including Logical, Informational, and Output expressions.
>
> Logical expressions are used to create conditional output. The basic expressions are the `0` and `1` expressions. A `$<0:...>` results in the empty string, and `<1:...>` results in the content of `...`. They can also be nested.
>
> A common usage of [`generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) is to conditionally add compiler flags, such as those for language levels or warnings. A nice pattern is to associate this information to an `INTERFACE` target allowing this information to propagate. Let's start by constructing an `INTERFACE` target and specifying the required C++ standard level of `11` instead of using [`CMAKE_CXX_STANDARD`](https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html#variable:CMAKE_CXX_STANDARD).



Generator Expressions是3.15之后才支持的功能

```bash
cmake_minimum_required(VERSION 3.15)
```



例子：

```bash
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
```



## Step11:export

导出之后其他的Cmake项目可以使用我们的项目了

TODO



## Step12:多配置Debug&Release





# Cmake笔记

https://cmake.org/cmake/help/latest/module/FindThreads.html

```cmake
# 项目中有这个依赖
target_link_libraries(rping LINK_PRIVATE ibverbs rdmacm ${CMAKE_THREAD_LIBS_INIT} rdmacm_tools)
```

我直接编译没有通过，后面看了文章，在这个参数前面添加了

```cmake
# 注意 threads不行，它会说找不到 Findthreads.cmake， THREADS也不行，只能Threads
find_package(Threads)
```

找到之后，就定义了变量 CMAKE_THREAD_LIBS_INIT了

一般引用第三方库是这么用的

```cmake
find_package(Threads)
if(Threads_FOUND)
    target_link_libraries(rping LINK_PRIVATE ibverbs rdmacm ${CMAKE_THREAD_LIBS_INIT} rdmacm_tools)
else(Threads_FOUND)
    message(FATAL_ERROR "threads lib not found")
endif(Threads_FOUND)    
```



##### include_guard

https://blog.csdn.net/zhizhengguan/article/details/118398244

3.10新版功能。

为CMake当前正在处理的文件提供一个包含保护。

```cpp
include_guard([DIRECTORY|GLOBAL])
```

为当前CMake文件设置一个include保护(参见CMAKE_CURRENT_LIST_FILE变量文档)。

如果当前文件已经在适用范围内被处理(见下文)，则CMake将在include_guard()命令所在的位置结束对当前文件的处理。它提供的功能类似于源代码头文件中常用的include保护或#pragma once指令。如果当前文件之前已经在适用范围内处理过，其效果就好像已经调用了return()。不要从当前文件中定义的函数内部调用此命令。



cmake 参数学习

```
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
set(CMAKE_EXECUTABLE_SUFFIX .out)
```



CMAKE_SKIP_INSTALL_ALL_DEPENDENCY

Don't make the `install` target depend on the `all` target.

By default, the `install` target depends on the `all` target. This has the effect, that when `make install` is invoked or `INSTALL` is built, first the `all` target is built, then the installation starts. If [`CMAKE_SKIP_INSTALL_ALL_DEPENDENCY`](https://cmake.org/cmake/help/latest/variable/CMAKE_SKIP_INSTALL_ALL_DEPENDENCY.html#variable:CMAKE_SKIP_INSTALL_ALL_DEPENDENCY) is set to `TRUE`, this dependency is not created, so the installation process will start immediately, independent from whether the project has been completely built or not.



add_subdirectory

https://www.jianshu.com/p/07acea4e86a3



cmake添加调试信息

