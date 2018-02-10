/*****************************************************************************
Copyright: 1988-1999, Huawei Tech. Co., Ltd.
File name: 文件名
Description: 用于详细说明此程序文件完成的主要功能，与其他模块或函数的接口，输出值、取值范围、含义及参数间的控制、顺序、独立或依赖等关系
Author: 作者
Version: 版本
Date: 完成日期
History: 修改历史记录列表， 每条修改记录应包括修改日期、修改者及修改内容简述。
*****************************************************************************/

/*************************************************
Function: // 函数名称
Description: // 函数功能、性能等的描述
Calls: // 被本函数调用的函数清单
Called By: // 调用本函数的函数清单
Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
Table Updated: // 被修改的表（此项仅对于牵扯到数据库操作的程序）
Input: // 输入参数说明，包括每个参数的作// 用、取值说明及参数间关系。
Output: // 对输出参数的说明。
Return: // 函数返回值的说明
Others: // 其它说明
*************************************************/

/*****************************************************************************
Copyright: 2017, Ting
File name: parsing.h
Description: The wrapper parsing header to invoke each step's parser respectively.
Author: Ting
Version: 1.0
Date: 11.26
History: 11.26 - 1,
*****************************************************************************/

/*
      1)  get function name and find in map
          if not found, INVALID
      2)  if found: get and store stepsize (make sure this is valid)
      3)  continue getting min and max limit arguments 

    */


std::cout << "Success: We are here!" << std::endl;

todolist

- 貌似在生成表达式的时候需要考虑非全是数的情况，此时需要修改makeExpr

- 还需要在预处理部分添加删除空格的操作，已经在helpers里面实现

- 要不要在function里面加入fname - 不需要，已经抽象出来为map的pair的第一项了

- 初始化是否需要非完全参数的情况？可以在输入的时候就给判断掉

- 如果var_list是空的，是否需要异常处理？现在是就当没看见，不执行任何操作

- 何处加const 重新检查

pow函数里面如果变量是函数的话，是否需要限制大小？例如：pow(g(3,4), h(5,6))

把helper的内容整理到cpp文件中

考虑parsing时的一个evil case: 括号数量匹配但位置不对，没有被输入检查查出而忽略
例如 
写入vector of var_pair时检测是否已经存在

整理函数中iterator和直接传list的操作，只用一种风格

- 查表还是查不到，复杂变量名仍旧有问题

define中右侧单括号不算错

引用的iterator，引用不能赋值但能调用方法？

调整判错退出顺序，如函数变量个数不同，提前退出
function.h 59 行的assert干掉