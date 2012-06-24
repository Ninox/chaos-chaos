##	2012-06-24
+ 基本完成了basepacker的功能，实现了基本的add, get, remove, rename, setpwd, update, create, load等功能
+ 新增能了basedef，用于自定义数据类型（typedef）
+ 新增了basedebug，用于输出简单的调试信息
+ qbase里面各个模块（见test），能够分别独立在Linux和Windows下面编译
+ 修改了目录结构下面的cmake文件，经过测试目前能够在 *windows* 平台下使用 *mingw* 成功编译qbase

		1.	接下来将对qbase整体进行一个测试（by gtest）,主要是功能性的测试和性能测试
		2.	尽量在7月初能够正式release第一个版本


##	2012-06-10
+ 基本完成了packer部分的压缩和解压功能
+ 解决了重复添加文件的bug
+ 之前无法还原文件是因为没有正确获取压缩前的大小，现在在数据结构中加入了old_size用于记录原来的大小
+ 解决了因为双向循环链表的结构问题导致pck_free出现死循环的问题

		1.	packer部分代码比较混乱，在整合之前需要重构一次
		2.	考虑在为qbase新增两个模块：baselog, basehandle, 用于日志记录和异常处理


##	2012-06-01
+ 完成了basescript的开发
+ 创建了分支lua_new_api，用于重新实现basescript
+ 通过测试，已经基本解决了内存泄露问题，吧lua_new_api分支和master分支合并

		1.	6月份目标是完成basepacker并且测试好，如果出现内存泄露则解决之
		2.	6月底，大约28号之后，尝试整体编译qbase并测试静态库


##	2012-05-20
+ 完成了qbase_script里面的call、reg 操作，能够简单地注册函数和调用lua里面的函数了
+ 更新了get操作和getfield，现在能够获取table的返回了（as a name）
+ 删除了qbase_tbl结构，暂时表格只是用一个全局的Name去保存。
+ 暂时没测试带参数call

		1.	get和getfield的重复代码比较多，下次需要重构一下这个部分
		2.	另外现在取消了原来的qbase_tbl的设计，暂时采用全局（在lua_State里面）的表名作为调用
		3.	下次重构将会实现一个真正的表结构(qbase_tbl)


##	2012-05-17
+ 完成了qbase里面的baseloader模块
+ 添加了关于baseloader的测试用例
+ 经过测试，截止到目前，basescript、basemath、baseloader都能够顺利在windows和ubuntu下面编译成功

		下一步将开发关于数据包的模块basepacker和修复basescript的残留问题
		
##	2012-05-03
+ 更新test当中qbase_math的接口
+ 修改了矩阵初始化的接口qbase_matrix_init的参数
+ 矩阵结构新增一个用于表示当前矩阵是以行向量为基础还是列向量为基础（行向量=0， 列向量=1）

		尽量在本周里面完成qbase的math，并且着手到packer

##	2012-05-02

+ 更新了qbase的数学库接口定义，完成部分接口的实现
+ 在test里面增加了qbase_lua和qbase_math的测试
+ 接下来会完成qbase_math的接口和qbase_lua还没有完成的reg和call