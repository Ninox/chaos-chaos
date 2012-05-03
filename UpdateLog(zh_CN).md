## 2012-05-03
+ 更新test当中qbase_math的接口
+ 修改了矩阵初始化的接口qbase_matrix_init的参数
+ 矩阵结构新增一个用于表示当前矩阵是以行向量为基础还是列向量为基础（行向量=0， 列向量=1）

	尽量在本周里面完成qbase的math，并且着手到packer

## 2012-05-02

+ 更新了qbase的数学库接口定义，完成部分接口的实现
+ 在test里面增加了qbase_lua和qbase_math的测试

+ 接下来会完成qbase_math的接口和qbase_lua还没有完成的reg和call