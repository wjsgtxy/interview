2022年1月12日10:03:28

#### 安装boost库

https://www.jb51.net/article/150380.htm

```
apt install libboost-all-dev
```



#### 安装libcsv3

```
apt install libcsv3
```



#### 安装liblog4cxx10v5

```
apt install liblog4cxx10v5
```



#### 安装postgrelsql

```bash
# focal上面默认安装v12，bionic上面默认安装v10
apt install postgresql
```

查询ubuntu上面的包，还可以指定ubuntu版本bionic https://packages.ubuntu.com/

https://packages.ubuntu.com/search?keywords=postgresql&searchon=names&suite=bionic&section=all

#### [Ubuntu通过apt-get安装指定版本和查询指定软件有多少个版本](https://www.cnblogs.com/EasonJim/p/7144017.html)



```bash
# pgsql 的一些包
apt install libpqxx-6.4

# google protobuf(focal 只有17版本，需要的是libprotobuf9v5)
apt install libprotobuf17

# 安装失败，提示libgflags2v5无法安装
apt install librocksdb5

apt insatll librocksdb5.17

# 下面这个包已经安装了，应该是前面某个包顺带安装的
apt install libssl1.1

# OK
apt install python-crypto
```

#### intel包 inte-mkl-64bit缺失

https://www.intel.com/content/www/us/en/developer/articles/guide/installing-free-libraries-and-python-apt-repo.html

```bash
# 这个包没有，应该要在scidb提供的Intel那个源才能有这个包
apt install intel-mkl-64bit

# 先安装秘钥
cd /tmp
wget --no-check-certifcate https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB
apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB
rm GPG-PUB-KEY-INTEL-SW-PRODUCTS-2019.PUB

# 再安装repo: Intel® oneAPI Math Kernel Library (oneMKL):
sudo sh -c 'echo deb https://apt.repos.intel.com/mkl all main > /etc/apt/sources.list.d/intel-mkl.list'
apt update
```





2022-5-22 20:48:38

Ubuntu18.04 源码包安装

```bash
# 基础软件包
sudo apt-get install build-essential
```



```bash
# 只能安装3.10版本的，不是最新版本的
apt install cmake 
snap install cmake

# 安装最新版cmake
# 添加签名密钥
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
# 稳定版
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
sudo apt-get update
# 候选发布版
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic-rc main'
sudo apt-get update
```

```bash
# 查看所有版本
apt-cache show cmake

# 查看指定环境变量
echo $CXX
echo $CC
echo $FC
```





```
# 安装gfortran库
apt install gfortran
```

cmake依赖

**find_package(MPI)**



find_package(**LibRT** REQUIRED)

`Found LibRT: /usr/lib/x86_64-linux-gnu/librt.so`



find_package(**Boost** 1.54 REQUIRED)

```bash
CMake Error at /usr/share/cmake-3.23/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find Boost (missing: Boost_INCLUDE_DIR date_time filesystem
  program_options regex serialization system thread atomic) (Required is at
  least version "1.54")
Call Stack (most recent call first):
  /usr/share/cmake-3.23/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
  /usr/share/cmake-3.23/Modules/FindBoost.cmake:2375 (find_package_handle_standard_args)
  CMakeLists.txt:256 (find_package)

# 安装boost
apt install libboost-all-dev  
```



find_package(**Protobuf** REQUIRED)

```bash
CMake Error at /usr/share/cmake-3.23/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find Protobuf (missing: Protobuf_LIBRARIES Protobuf_INCLUDE_DIR)
Call Stack (most recent call first):
  /usr/share/cmake-3.23/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
  /usr/share/cmake-3.23/Modules/FindProtobuf.cmake:650 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  CMakeLists.txt:266 (find_package)

# 查找
apt search libprotobuf
# 安装protobuf
apt install libprotobuf10
# 开发库 只用安装这个就可以了
apt install libprotobuf-dev 

-- Found Protobuf: /usr/lib/x86_64-linux-gnu/libprotobuf.so;-lpthread (found version "3.0.0")
```



find_package(**Log4CXX** REQUIRED)



find_package(**Doxygen**)

find_package(**LibPQXX** REQUIRED)

find_package(**LibPQ** REQUIRED)

find_package(**OpenSSL** 0.9.8 REQUIRED)



**find_package(PkgConfig)**



find_package(**FLEX** 2.5.35 REQUIRED)

find_package(**BISON** 2.4 REQUIRED)

find_package(**SED** REQUIRED)

find_package(**ZLIB** REQUIRED)

find_package(**BZip2** REQUIRED)

find_package(**Threads** REQUIRED)

find_package(**PythonInterp** REQUIRED)

find_package(**EditLine** REQUIRED)



**find_package(MKL_BLAS)**

没找到上面这个mkl_blas，则需要

find_package(**BLAS** REQUIRED)

find_package(**LAPACK** REQUIRED)