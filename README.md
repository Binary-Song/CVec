CVec 文档
------
## 1. CVec
CVec是一个C语言实现的线性表库。它能在运行时改变容器大小，因此比数组更加灵活；它支持编译期的类型检查，因此比未模板化的数据结构更加安全。CVec只包含一个文件：vec.h。使用时`#include`它即可。

## 2. 使用CVec
CVec使用起来十分方便。它的基本原理是利用宏生成结构体和函数的声明和定义。因此，您在使用CVec前要先调用宏来生成它们。

### 2.1 生成代码

首先，用`DECL_VEC(vtype, type, prefix)`宏生成结构体和函数原型声明。`vtype`代表线性表的结构体名称及相关函数名的开头部分，你可以取一个自己喜欢的名字（只要它符合C的标识符规范）。`type`代表线性表中存储的数据类型，比如`int`、`float`或者`char*`。建议不要在这里加上`const`限定符，否则编译器会报告warning。`prefix`是所有生成出的函数的前缀。比如填写`static`将使所有生成出来的函数带上`static`前缀，从而使得它们具有内部链接。如果没有这方面的需求，可以留空此参数或用`EMPTY_PREF`占位。

例如，这样可以生成一个存放`int`的线性表的相关声明：

`DECL_VEC(IntList, int, EMPTY_PREF)`

接下来，在`DECL_VEC`的后面调用`DEF_VEC(vtype, type, prefix)`宏来生成相关函数的定义。三个参数的含义与`DECL_VEC`相同，其中`vtype`和`type`应该与`DECL_VEC`保持一致。`prefix`则依旧根据你的需求填写。

例如，生成刚才`IntList`线性表的相关函数定义：

`DEF_VEC(IntList, int, EMPTY_PREF)`

可以将`DECL_VEC`放在头文件中，`DEF_VEC`放在源文件中以分离接口和实现。

### 2.2 调用接口

提示：接下来提到的`{vtype}`和`{type}`都应当用前两个宏的对应*实参*来替换。比如对于之前的例子，应该用`IntList`替换`{vtype}`，用`int`替换`{type}`。

所有的接口函数都以`{vtype}`开头。刚才我们已经用
```
DECL_VEC(IntList, int, EMPTY_PREF)
DEF_VEC(IntList, int, EMPTY_PREF)
```
生成了 IntList 相关代码，现在可以用`{vtype}_init`函数创建一个线性表：
```
IntList *my_list = IntList_init(IntList_default_copy, IntList_default_deinit);
```
第一个参数是一个函数指针，类似C++的拷贝构造函数。它在向列表添加元素时被调用，该函数的参数类型和返回值类型都是`{type}`。我们会为您生成默认的拷贝函数，是一个简单的`=`赋值。如果您认为这样就足够，可以用`{vtype}_default_copy`来作为此处的实参。

第二个参数也是一个函数指针，类似C++的析构函数。它在删除列表元素时调用。同样地，也存在默认的析构函数，函数体为空。如果您认为这样就足够，可以用`{vtype}_default_deinit`来作为此处的实参。

返回值是表的指针。

接下来我们用一个循环，用`{vtype}_push_back`向my_list里面添加1-10共十个元素。

```
for (int n = 1; n <= 10; ++n)
{
    IntList_push_back(my_list, n);
}
```
`{vtype}_push_back`向表的尾部添加元素。第一个参数是表的指针，第二个参数是要添加的对象，类型为`{type}`。

然后，我们用`FOREACH`宏来遍历每个元素，打印出它们的值，看看是否成功添加。

```
int a;
FOREACH(a, IntList, my_list)
{
    printf("%d\n", a);
}
```
`FOREACH(elem, vtype, vp)`宏的第一个参数`elem`是迭代变量，用它来表示每个元素的值。第二个参数`vtype`是需要遍历的表的`vtype`。第三个参数`vp`是表的指针。

最后，不要忘记释放表占用的内存。
```
IntList_deinit(my_list);
```
`{vtype}_deinit`能释放表占用的内存，并且一一调用之前指定的析构函数，确保没有内存泄漏。它的唯一参数是表的指针。
