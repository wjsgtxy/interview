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