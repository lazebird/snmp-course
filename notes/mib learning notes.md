# net-snmp学习总结

## SNMP定义

<https://blog.csdn.net/bbwangj/article/details/80981098> 

<https://baike.baidu.com/item/%E7%AE%80%E5%8D%95%E7%BD%91%E7%BB%9C%E7%AE%A1%E7%90%86%E5%8D%8F%E8%AE%AE/2986113?fr=ge_ala>

[https://dandelioncloud.cn/article/details/1524339540726943746]()

<https://www.8a.hk/news/content/4856.html>

<https://help.aliyun.com/practice_detail/606932>

SNMP 是 Simple Network Management Protocol（简单网络管理协议），利用 SNMP 协议，网络管理员可以对网络上的节点进行信息查询、网络配置、故障定位、容量规划，网络监控和管理是 SNMP的基本功能。

## 安装

<https://zhuanlan.zhihu.com/p/75919688>

<https://www.cnblogs.com/quliuliu2013/p/7520185.html>

*   在[官网](https://sourceforge.net/projects/net-snmp/)下载源码，解码后进入其根目录
*   执行命令，设置默认安装位置，没有指定路径可能找不到snmp文件，./configure --prefix=/usr/local/snmp --with-mib-modules='ucd-snmp/diskio ip-mib/ipv4InterfaceTable'
*   源码安装netsnmp报错：/usr/bin/ld: cannot find -lperl，解决方法

要安装的库的名字一般为lib+(-l后面的东西，这里是perl)+(-dev) 所以要安装的是libperl-dev sudo apt-get install libperl-dev 将snmpd执行文件，和snmp工具加入默认路径，末尾加 gedit /etc/profile PATH="/usr/local/snmp/sbin/:\$PATH:/usr/local/snmp/bin/:\$PATH" export 保存退出，让path生效 source /etc/profile 生成和配置snmpd.conf文件 cd /usr/local/net-snmp-5.9.4 cp EXAMPLE.conf /usr/local/snmp/snmpd.conf vim /usr/local/snmp/snmpd.conf trap2sink 192.168.211.1 public //设置接收trap的管理站ip

## 运行

```c
指定配置文件运行
snmpd -c /usr/local/snmp/snmpd.conf
查看错误信息
snmpd -f -Le
```

## mib2c

编写测试MIB文件（如：[MY-TEST-MIB.txt](note://WEBa0938d8b6532e534ba80f51055582a61)）后，将MIB文件用来生成简单MIB变量的.c和.h文件

```c
查看进程 
ps -aux | grep snmpd 

编写MIB文件 
cd /root/projects/mib 
touch MY-TEST-MIB.txt 
gedit MY-TEST-MIB.txt 

测试自定义MIB是否添加成功 
snmptranslate -Tp -IR MY-TEST-MIB::Test 
出现错误"You didn't give mib2c a valid OID to start with." 
请检查库名对象名是否正确 使用mib2c只能在当前目录生成.c和.h文件 

cd /usr/local/snmp/share/snmp 
mib2c -c mib2c.old-api.conf MY-TEST-MIB::Test 

如果出现错误 Can't find a configuration file called mib2c.scalar.conf 
I looked in: 
			/usr/local/share/snmp/ 
			/usr/local/share/snmp/mib2c-data 
			/mib2c-conf.d 

终端输入 sudo nautilus 打开一个管理员权限的文件夹，搜索mib2c.conf文件，将所有相关的文件拷贝到上述任一 文件夹中，
再次执行mib2c命令,可在任意目录生成.c和.h文件 
cd /home/code/EmbeddedIntroduction/powerEthernetMIB 
mib2c -c mib2c.old-api.conf MY-TEST-MIB::Test / 
mib2c -c mib2c.scalar.conf MY-TEST-MIB::Test /
mib2c -c mib2c.notify.conf MY-TEST-MIB::Test 

可选：防止删除生成的main文件 
chattr -R -i +a +u /home/code/EmbeddedIntroduction/powerEthernetMIB/ 
chattr -R -i -a -u /home/code/EmbeddedIntroduction/powerEthernetMIB/ //取消防止删除 

编译和挂载子代理 
net-snmp-config --compile-subagent a.out pethNotifications.c powerEthernetMIB.c 
./a.out -f -Lo -x tcp:localhost:705 

编译链接失败，增加库文件地址 
vim /etc/ld.so.conf 
/usr/local/snmp/lib 

连接失败Warning: Failed to connect to the agentx master agent ([NIL]): 
vim /usr/local/snmp/snmpd.conf 
agentXSocket tcp:localhost:705 
agentxperms 777 777 

查询子代理进程 
netstat -tlnp | grep 7 
lsof -i:705
```

## C文件编写

[net-snmp函数、参数说明](https://github.com/haad/net-snmp/blob/c629882ba31aaf27c859de2d47a6401849661ccd/AGENT.txt#L2)

[header\_simple\_table源码](https://github.com/haad/net-snmp/blob/master/agent/mibgroup/util_funcs/header_simple_table.c)

[自定义mib的get、set函数编写](https://blog.csdn.net/Rong_Toa/article/details/108869743)

## 常用命令

```c
vim /usr/local/snmp/snmpd.conf 
cd /root/projects/mib

snmptranslate -Tp -IR Test 
mib2c -c mib2c.old-api.conf Test 
mib2c -c mib2c.notify.conf Test 

snmpd -c /usr/local/snmp/snmpd.conf 
net-snmp-config --compile-subagent a.out pethNotifications.c powerEthernetMIB.c 
./a.out -f -Lo -x tcp:localhost:705 

ps aux | grep snmpd 
lsof -i:705 kill -9 ID
```

## c开发练习存在的问题

*   在MIB Browser中🔑表示索引，是实际意义上的索引，不单是数据，如果有两个🔑就是双索引，.x.x表示一个实例
*   开发MIB时使用模板后按照自己需求修改，函数可删可大改，变量命名时应易懂
*   双索引时应注意OID写回
*   正式代码不能printf，用户不应该看到我们的调试信息，错误提示信息除外
*   全局变量少用，非必要不用，可以函数间调用传参代替
*   detstat时端口状态，应初次开发练习无法查询端口状态，现用admineable管理状态代替
*   简单操作不用回退，只用commit操作，模板上的多步处理操作是为了可实现多个实例的set操作
*   不可读对象的get函数不进行操作直接返回NULL，不可写对象的write函数直接删除
*   比较和计算变量时注意变量的类型和单位

## mib图标含义

*   绿叶：read-only
*   🖊：read-write
*   🔑：fileEntry index
*   绿＋：read-create
*   ⚡：notification

## traning-mib开发过程中出现的问题

*   开发要求读取文件名和文件长度， 开始使用readdir函数（非字母排序读取）时，只能getnext部分文件，且这文件时字母排序的。因为在getndata中使用的时strcmp函数(AB>A)，filename（name的表索引）只需一个值就能得到data的第一个值，然后根据strcmp遍历获得data，这就要求data是字母排序的，可以使用scandir(dir\_path, \&name\_list, 0, alphasort)，否则会漏掉部分数据。
*   注意编码规范，函数命名单词间使用下斜杠，或者使用2表示to来连接两个单词
*   不必要的初始化可以尽量减少，结构体的初始化可以在声明时 = {0}
*   类型强制转换
*   mib树一但建好，尽量不更改，扩展可以考虑兼容之前的，建议另建一树
*   在交换机编译时，支持netsnmp相关函数的头文件不同，函数也有差异，使用提供的脚本可修改大致差异

