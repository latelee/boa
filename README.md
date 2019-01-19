# boa
boa test. boa+cgi+html+css

## 设计
使用cgic库。不同的cgi响应，使用不同的文件实现。响应函数为小写+下划线（以示区别于其它函数方法）。  

## 依赖库
依赖 flex、bison
```
sudo apt-get install -y flex bison
```

## 编译
进入boa-0.94.13/src，输入`make`即可，如要交叉编译，则指定CROSS_COMPILE。  

## 使用
src目录生成的boa即为可执行文件，放到另外的仓库中运行即可（由于依赖配置文件和cgi文件，故单独仓库）。

李迟 2019.1月中旬
