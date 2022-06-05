# CMAKE





```bash
# 安装cmake图形界面
apt-get install cmake-qt-gui 
# 运行
cmake-gui
```





第一章



```bash
# 直接构建，不用make
cmake --build .

# 查看所有目标
cmake --build . --target help
```



# CMake之add_custom_target

https://blog.csdn.net/MacKendy/article/details/122693478



set_source_files_properties

https://cmake.org/cmake/help/latest/command/set_source_files_properties.html



常用语法

https://www.jianshu.com/p/8909efe13308







# [CMAKE_BUILD_TYPE](https://www.cnblogs.com/faithlocus/p/15551451.html)



1. 可选值包括：

   1. Debug：用于在没有优化的情况下，使用带有调试符号构建库或可执行文件
   2. Release：用于构建的优化的库或可执行文件，不包含调试符号
   3. RelWithDebInfo：由于构建较少的优化库或可执行文件，包含调试符号
   4. MinSizeRel：用于不增加目标代码大小的优化方式，来构建或可执行文件

2. 使用方法

   ```cmake
   set(CMAKE_BUILD_TYPE "Debug")     # CMakeLists.txt中指定
   ```

   或者

   ```shell
   cmake .. -D CMAKE_BUILD_TYPE="Debug"   # 生成时命令行指定
   ```

3. 常用配置

   ```cmake
   set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")       # 启用GDB
   set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")               # 启用优化（1～3）
   ```



**mark_as_advanced**: Mark cmake cached variables as advanced.

```
  mark_as_advanced([CLEAR|FORCE] VAR VAR2 VAR...)
```

标记已命名的缓冲变量为 advanced.  advanced 变量 不会在cmake 图形界面中显示 除非 advanced 选项开启. 如果第一个参数是 CLEAR  advanced 变量变为unadvanced. 如果 FORCE 是第一个参数, 变量变为 advanced. 如果 FORCE 和 CLEAR 都未选定, 新变量标记为 advanced, 但是如果变量已是 advanced/non-advanced 状态, 就不会发生变化.



```cmake
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif()
```

当然我也推荐在-Wextra后面添加-Werror或者在/W4后面添加/WX来将warning视为error直接中断编译，严于律己是一个好习惯。这里在补充一下-Wall和-Wextra的小差别，-Wall并不是指“all warning"而是指"all errors that everybody could agree on"，-Wextra则指那些"somebody agree on"的warning.

Cmake命令之add_subdirectory介绍

写的很好，详细易懂

https://www.jianshu.com/p/07acea4e86a3

https://cmake.org/cmake/help/latest/command/add_subdirectory.html



###### cmake使用教（二） install的使用

https://blog.csdn.net/weixin_42398658/article/details/121672529

https://blog.csdn.net/weixin_42398658/category_11513468.html



cmake 添加调试信息，才能有debug段

SET(CMAKE_BUILD_TYPE "Debug")

https://www.cnblogs.com/hshy/p/11379433.html
