# 编译说明
如使用当前编译配置，环境必须有vcpkg和musl的交叉编译工具链需使用海思释放的安装方式。

配置好上述环境后和`CMakeUserPresets.json`后
1. 选择对应的编译架构
```shell
cmake --preset <你自己的配置>
```
2. 编译生成执行文件
```shell
cmake --build build
```
之后就会生成相应的可执行文件在build目录里