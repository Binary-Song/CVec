
# 目录
- [目录](#目录)
- [CVec](#cvec)
- [使用速览](#使用速览)
  - [生成代码](#生成代码)
  - [调用接口](#调用接口)
- [配置](#配置)
- [API 参考](#api-参考)
  - [DECL_VEC](#decl_vec)
  - [DEF_VEC](#def_vec)
  - [DECL_DEF_VEC](#decl_def_vec)
  - [FOREACH](#foreach)
  - [init](#init)
  - [init_by_array](#init_by_array)
  - [deinit](#deinit)
  - [push_back](#push_back)
  - [get](#get)
  - [erase](#erase)
  - [erase_range](#erase_range)
  - [pop](#pop)
  - [begin](#begin)
  - [end](#end)
# CVec

CVec 是一个 C 语言实现的线性列表容器，类似于 C++ 的 vector。它能在运行时改变容器大小，因此比数组更加灵活；它支持编译期的类型检查，因此比未模板化的数据结构更加安全。它还支持可开关的类型检查功能。

# 使用速览

CVec 只包含一个文件：vec.h，使用时`#include`它即可。CVec 的基本原理是用宏生成结构体和函数的声明和定义。因此，您在使用 CVec 前要先调用宏来生成它们。

## 生成代码

首先，用`DECL_VEC(vtype, type, prefix, rqc, rqd)`宏生成结构体和函数原型声明。

`vtype`代表列表的结构体名称及相关函数名的开头部分，你可以取一个自己喜欢的名字（合法的 C 标识符）。

`type`代表列表中存储的数据类型，比如`int`、`float`或者`char*`。建议不要在这里加上`const`限定符，否则编译器会报告 warning。

`prefix`是所有生成出的函数的前缀。比如填写`static`将使所有生成出来的函数带上`static`前缀，从而使得它们具有内部链接。如果没有这方面的需求，可以留空此参数或用`EMPTY_PREFIX`占位。

`rqc`的值只能是`0`或`1`。它表示列表中的元素是需要一个自定义拷贝函数来完成拷贝呢，还是只需`=`运算符即可完成拷贝。`0`表示需要，`1`表示不需要。拷贝会在以下情况执行：

- 用`push_back`向列表末尾添加元素时
- 用`pop`弹出末尾元素时

如果您想存储字符串（`char*`），您可能会想将它设置为`1`。

`rqd`的值只能是`0`或`1`。它表示列表中的元素在被删除时是否需要自定义的析构函数来处理。`0`表示需要，`1`表示不需要。如果您将`rqc`的值设置为`1`，这个值一般也应该是`1`。

例如，这样可以生成一个存放`char*`的列表的相关声明：
```
DECL_VEC(StrLs, char*, EMPTY_PREFIX ,1 ,1)
```
接下来，在`DECL_VEC`的后面调用`DECL_VEC(vtype, type, prefix, rqc, rqd)`宏来生成相关函数的定义。参数的含义与`DECL_VEC`相同。

例如，生成刚才`StrLs`列表的相关函数定义：
```
DEF_VEC(StrLs, char*, EMPTY_PREFIX ,1 ,1)
```
可以将`DECL_VEC`放在头文件中，`DEF_VEC`放在源文件中以分离接口和实现。也可以用`DECL_VEC(vtype, type, prefix, rqc, rqd)`来合并两个宏调用。

## 调用接口

所有的API函数都以`vtype`开头。刚才我们已经用

```
DECL_VEC(StrLs, char*, EMPTY_PREFIX ,1 ,1)
DEF_VEC(StrLs, char*, EMPTY_PREFIX ,1 ,1)
```

生成了`StrLs`相关代码，现在可以用生成出的`StrLs_init`函数创建一个列表。但在此之前，我们需要编写拷贝和析构`char*`的函数，因为我们之前将`rqc`和`rqd`都指定为`1`了。

拷贝函数的形参和返回值类型都是传给宏的`type`参数，这里就是`char*`。

```
char* copy_str(char* src)
{
    char* dest = malloc(strlen(src)+1);
    strcpy(dest,src);
    return dest;
}
```
 
析构函数的形参的类型也是传给宏的`type`参数，返回值是`void`。

```
void deinit_str(char* str)
{
    free(str);
}
```
有了它们，我们来调用`StrLs_init`创建一个列表。

```
StrLs *my_list = StrLs_init(copy_str, deinit_str);
```
`StrLs_init`的参数列表由`rqc`和`rqd`参数决定。这里二者都为`1`，所以分别需要`copy`和`deinit`的函数指针。假如`rqc`和`rqd`都为`0`，这里将不需要参数。

返回值是表的指针。

接下来我们用`StrLs_push_back`向`my_list`里面添加一些元素。

```
StrLs_push_back(my_list, (char*)"Hello");
StrLs_push_back(my_list, (char*)"CVec");
StrLs_push_back(my_list, (char*)"!");
```

`StrLs_push_back`向表的尾部添加元素。第一个参数是表的指针，第二个参数是要添加的对象，类型与`type`相同（`char*`）。p.s. 只要在`copy_str`函数里不修改指针指向的内存，就可以放心强转。

然后，我们用`FOREACH`宏来遍历每个元素，打印出它们的值，看看是否成功添加。

```
char* element;
FOREACH(element, StrLs, my_list)
{
    printf("%s\n", element);
}
```

`FOREACH(elem, vtype, vp)`宏的第一个参数`elem`是迭代变量，用它来表示每个元素的值。第二个参数`vtype`是需要遍历的表的结构体名。第三个参数`vp`是表的指针。

最后，不要忘记释放表占用的内存。

```
StrLs_deinit(my_list);
```

`StrLs_deinit`能释放表占用的内存，并且一一调用之前指定的析构函数，确保没有内存泄漏。它的唯一参数是表的指针。

完整代码是这样的：

```
#include "vec.h"

DECL_VEC(StrLs, char*, EMPTY_PREFIX ,1 ,1)//声明函数原型、定义结构体
DEF_VEC(StrLs, char*, EMPTY_PREFIX ,1 ,1)//定义函数

char* copy_str(char* src);
void deinit_str(char* str);

int main(void)
{
    StrLs *my_list = StrLs_init(copy_str, deinit_str);//创建列表
    
    StrLs_push_back(my_list, (char*)"Hello");//添加元素
    StrLs_push_back(my_list, (char*)"CVec");
    StrLs_push_back(my_list, (char*)"!");
    
    char* element;
    FOREACH(element, StrLs, my_list)//遍历打印
    {
        printf("%s\n", element);
    }

    StrLs_deinit(my_list);//回收资源
    return 0;
}

char* copy_str(char* src)
{
    char* dest = malloc(strlen(src)+1);
    strcpy(dest,src);
    return dest;
}

void deinit_str(char* str)
{
    free(str);
}

```

# 配置

CVec可以通过以下宏进行配置。可以在vec.h的前部找到它们并按需修改。

| 宏                 | 描述             | 合法值域 | 默认值 |
| ------------------ | ---------------- | -------- | ------ |
| `INITIAL_CAPACITY` | 列表初始容量     | 数       | `5`    |
| `GROWTH_RATE`      | 列表容量增长倍率 | 数       | `2`    |
| `BOUND_CHECK`      | 是否检查边界     | `0`或`1` | `0`    |


# API 参考


## DECL_VEC

**简介**

生成列表结构体的定义以及API函数原型声明。

**宏**

```
DECL_VEC(vtype, type, prefix, rqc, rqd)
```

**参数**
| 参数     | 描述                                                             |
| -------- | ---------------------------------------------------------------- |
| `vtype`  | 列表结构体的名字hi和所有API函数名的前缀                          |
| `type`   | 列表存放的数据类型                                               |
| `prefix` | 所有API函数声明的前缀                                            |
| `rqc`    | 值为`0`或`1`，分别代表使用普通赋值的拷贝方式和使用自定义拷贝函数 |
| `rqd`    | 值为`0`或`1`，分别代表不进行析构和使用自定义析构函数             |
 
## DEF_VEC

**简介**

生成API函数的定义。

**宏**

```
DEF_VEC(vtype, type, prefix, rqc, rqd)
```

**参数**
| 参数     | 描述                                                             |
| -------- | ---------------------------------------------------------------- |
| `vtype`  | 列表结构体的名字hi和所有API函数名的前缀                          |
| `type`   | 列表存放的数据类型                                               |
| `prefix` | 所有API函数声明的前缀                                            |
| `rqc`    | 值为`0`或`1`，分别代表使用普通赋值的拷贝方式和使用自定义拷贝函数 |
| `rqd`    | 值为`0`或`1`，分别代表不进行析构和使用自定义析构函数             |

## DECL_DEF_VEC

**简介**

用同样的参数调用`DECL_VEC`和`DEF_VEC`。

**宏**

```
DECL_DEF_VEC(vtype, type, prefix, rqc, rqd)
```

**参数**
| 参数     | 描述                                                             |
| -------- | ---------------------------------------------------------------- |
| `vtype`  | 列表结构体的名字hi和所有API函数名的前缀                          |
| `type`   | 列表存放的数据类型                                               |
| `prefix` | 所有API函数声明的前缀                                            |
| `rqc`    | 值为`0`或`1`，分别代表使用普通赋值的拷贝方式和使用自定义拷贝函数 |
| `rqd`    | 值为`0`或`1`，分别代表不进行析构和使用自定义析构函数             |

**下面出现的 `vtype`、`type`、`rqc`和`rqd` 应该用 `DECL_VEC`和 `DEF_VEC`的对应实参替换。**


## FOREACH

**简介**

遍历列表。

**宏**

```
FOREACH(elemi, vtype, vp)
```

**参数**

| 参数  | 描述                                                 |
| ----- | ---------------------------------------------------- |
| elemi | 迭代变量，每次遍历赋给它当前元素的值（不调用`copy`） |
| vtype | 列表的`vtype`                                        |
| vp    | 指向列表的指针                                       |

**提示**

用 FOREACH 可以方便地遍历列表，具体可以参照[使用速览](#使用速览)中给出的例子。
 


## init

**简介**

创建一个空列表。

**函数签名**

根据`rqc`和`rqd`有以下变体： 
```
vtype *vtype_init()
``` 
```
vtype *vtype_init(vtype_copy_t copy)
``` 
```
vtype *vtype_init(vtype_deinit_t deinit)
``` 
```
vtype *vtype_init(vtype_copy_t copy, vtype_deinit_t deinit)
```
**回调函数**

```
type (*vtype_copy_t)(type obj)
```

```
void (*vtype_deinit_t)(type obj)
```

**参数**

| 参数     | 描述                           |
| -------- | ------------------------------ |
| `copy`   | 用来拷贝`type`类型对象的函数。 |
| `deinit` | 用来回收`type`类型对象的函数。 |

**提示**

如果`rqc`为`1`，会调用`copy`来拷贝。

对于基本类型或只含有基本类型的结构体的，默认的拷贝方式已经够用。对于需要深拷贝的情况，则需要自定义拷贝函数。例如有一个存放`char*`的列表，每个元素都指向 C 风格字符串。如果希望每次添加元素都对元素进行深拷贝，可以定义如下函数作为`copy`的参数。



## init_by_array

**简介**

创建列表，拷贝数组中的每一个元素到此列表中。

**函数签名**

根据`rqc`和`rqd`有以下变体：
```
vtype *vtype_init_by_array(const type *array, size_t count)
```

```
vtype *vtype_init_by_array(const type *array, size_t count, vtype_copy_t copy)
```

```
vtype *vtype_init_by_array(const type *array, size_t count, vtype_deinit_t deinit)
```

```
vtype *vtype_init_by_array(const type *array, size_t count, vtype_copy_t copy, vtype_deinit_t deinit)
```

**回调函数**

```
type (*vtype_copy_t)(type obj)
```

```
void (*vtype_deinit_t)(type obj)
```

**参数**

| 参数     | 描述                                       |
| -------- | ------------------------------------------ |
| `array`  | 创建列表的依据。将会拷贝其中的每一个元素。 |
| `count`  | 数组的元素个数。                           |
| `copy`   | 用来拷贝`type`类型对象的函数。             |
| `deinit` | 用来回收`type`类型对象的函数。             |

**提示**

如果`rqc`为`1`，会调用`copy`来拷贝。

## deinit

**简介**

根据数组创建列表。

**函数签名**

```
void vtype_deinit(vtype *v)
```

**参数**

| 参数 | 描述             |
| ---- | ---------------- |
| `v`  | 指向列表的指针。 |

**提示**

如果`rqd`为`1`，会对每一个元素调用`deinit`。

## push_back

**简介**

向列表尾部添加元素。

**函数签名**

```
void vtype_push_back(vtype *v, type obj)
```

**参数**

| 参数  | 描述                                     |
| ----- | ---------------------------------------- |
| `v`   | 指向列表的指针。                         |
| `obj` | 添加的元素。将用`copy`函数拷贝它再添加。 |

**提示**

如果`rqc`为`1`，会调用`copy`来拷贝。

## get

**简介**

返回列表中的第 `index-1` 个元素。

**函数签名**

```
type vtype_get(const vtype *v, size_t index);
```

**参数**

| 参数    | 描述                    |
| ------- | ----------------------- |
| `v`     | 列表的指针。            |
| `index` | 取第 `index`-1 个元素。 |
 
**返回值**

列表中的第 `index-1` 个元素。

**提示**

不会调用`copy`函数。

## erase

**简介**

从列表中删除指针指向的元素。

**函数签名**

```
vtype_iter vtype_erase(vtype *v, vtype_iter i)
```

**参数**

| 参数 | 描述                   |
| ---- | ---------------------- |
| `v`  | 列表的指针。           |
| `i`  | 指向被删除元素的指针。 |

**返回值**

指向被删除元素下一个元素的指针。如果被删除的元素在末尾，就返回尾后指针。

**提示**

如果`rqd`为`1`，会对被删除元素调用`deinit`。

## erase_range

**简介**

从列表中删除连续的元素。

**函数签名**

```
vtype_iter vtype_erase_range(vtype *v, vtype_iter first, vtype_iter last)
```

**参数**

| 参数    | 描述                                       |
| ------- | ------------------------------------------ |
| `first` | 指针，指向被删除的第一个元素。             |
| `last`  | 指针，指向被删除最后一个元素的下一个元素。 |

**返回值**

指向被删除的最后一个元素的后一个元素的指针。

**提示**

如果`rqd`为`1`，会对被删除元素调用`deinit`。

## pop

**简介**

返回列表末尾的元素的拷贝并从列表中删除此元素。

**函数签名**

```
type vtype_pop(vtype *v)
```

**参数**

| 参数 | 描述             |
| ---- | ---------------- |
| `v`  | 指向列表的指针。 |

**返回值**

列表末尾的元素的副本。

**提示**

如果`rqc`为`1`，会调用`copy`来拷贝。

如果`rqd`为`1`，会对被删除元素调用`deinit`。

## begin

**简介**

取得指向第一个元素的指针。

**函数签名**

```
vtype_iter vtype_begin(const vtype *v)
```

**参数**

| 参数 | 描述             |
| ---- | ---------------- |
| `v`  | 指向列表的指针。 |

**返回值**

指向第一个元素的指针。

## end

**简介**

取得尾后指针。

**函数签名**

```
vtype_iter vtype_end(const vtype *v)
```
**参数**

| 参数 | 描述         |
| ---- | ------------ |
| `v`  | 列表的指针。 |

**返回值**

指向的列表最后一个元素后一个位置的指针，即尾后指针。

**提示**

不要对此指针进行解引用操作，结果将是未定义的。

