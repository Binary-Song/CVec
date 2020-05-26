# CVec 文档

<!-- vscode-markdown-toc -->

- 1. [CVec](#CVec)
- 2. [使用 CVec](#CVec-1)
     - 2.1. [生成代码](#)
     - 2.2. [调用接口](#-1)
- 3. [API](#API)
     - 3.1. [_vtype_\_init](#vtype_init)
     - 3.2. [_vtype_\_init_by_array](#vtype_init_by_array)
     - 3.3. [_vtype_\_deinit](#vtype_deinit)
     - 3.4. [_vtype_\_push_back](#vtype_push_back)
     - 3.5. [_vtype_\_get](#vtype_get)
     - 3.6. [_vtype_\_erase](#vtype_erase)
     - 3.7. [_vtype_\_erase_range](#vtype_erase_range)
     - 3.8. [_vtype_\_pop](#vtype_pop)
     - 3.9. [_vtype_\_begin](#vtype_begin)
     - 3.10. [_vtype_\_end](#vtype_end)
     - 3.11. [_vtype_\_next](#vtype_next)
     - 3.12. [_vtype_\_prev](#vtype_prev)
     - 3.13. [_vtype_\_deref](#vtype_deref)
     - 3.14. [_vtype_\_eqi](#vtype_eqi) 
     - 3.15. [FOREACH](#FOREACH)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

## 1. <a name='CVec'></a>CVec

CVec 是一个 C 语言实现的线性表容器。它能在运行时改变容器大小，因此比数组更加灵活；它支持编译期的类型检查，因此比未模板化的数据结构更加安全。

## 2. <a name='CVec-1'></a>使用 CVec

CVec 只包含一个文件：vec.h，使用时`#include`它即可。CVec 的基本原理是用宏生成结构体和函数的声明和定义。因此，您在使用 CVec 前要先调用宏来生成它们。

### 2.1. <a name=''></a>生成代码

首先，用`DECL_VEC(vtype, type, prefix)`宏生成结构体和函数原型声明。`vtype`代表列表的结构体名称及相关函数名的开头部分，你可以取一个自己喜欢的名字（只要它符合 C 的标识符规范）。`type`代表列表中存储的数据类型，比如`int`、`float`或者`char*`。建议不要在这里加上`const`限定符，否则编译器会报告 warning。`prefix`是所有生成出的函数的前缀。比如填写`static`将使所有生成出来的函数带上`static`前缀，从而使得它们具有内部链接。如果没有这方面的需求，可以留空此参数或用`EMPTY_PREF`占位。

例如，这样可以生成一个存放`int`的列表的相关声明：

`DECL_VEC(IntList, int, EMPTY_PREF)`

接下来，在`DECL_VEC`的后面调用`DEF_VEC(vtype, type, prefix)`宏来生成相关函数的定义。三个参数的含义与`DECL_VEC`相同，其中`vtype`和`type`应该与`DECL_VEC`保持一致。`prefix`则依旧根据你的需求填写。

例如，生成刚才`IntList`列表的相关函数定义：

`DEF_VEC(IntList, int, EMPTY_PREF)`

可以将`DECL_VEC`放在头文件中，`DEF_VEC`放在源文件中以分离接口和实现。

### 2.2. <a name='-1'></a>调用接口

_用斜体书写的 `vtype` 和 `type` ，应该用 `DECL_VEC`和 `DEF_VEC`的对应实参替换。_

所有的接口函数都以 _`vtype`_ 开头。刚才我们已经用

```
DECL_VEC(IntList, int, EMPTY_PREF)
DEF_VEC(IntList, int, EMPTY_PREF)
```

生成了 `IntList` 相关代码，现在可以用 _`vtype`_\_init 函数创建一个列表：

```
IntList *my_list = IntList_init(NULL, NULL);
```

第一个参数是一个函数指针，类似 C++的拷贝构造函数。它在向列表添加元素时被调用，该函数的参数类型和返回值类型都是 _`type`_。如果您认为默认的拷贝方式（简单的`=`号赋值）就足够，可以用`NULL`来作为此处的实参。

第二个参数也是一个函数指针，类似 C++的析构函数。它在删除列表元素时调用。如果您认为不需要特别的析构处理，可以用`NULL`来作为此处的实参。

返回值是表的指针。

接下来我们用一个循环，用 _`vtype`_`_push_back`向`my_list`里面添加 1-10 共十个元素。

```
for (int n = 1; n <= 10; ++n)
{
    IntList_push_back(my_list, n);
}
```

_`vtype`_`_push_back`向表的尾部添加元素。第一个参数是表的指针，第二个参数是要添加的对象，类型为 _`type`_。

然后，我们用`FOREACH`宏来遍历每个元素，打印出它们的值，看看是否成功添加。

```
int a;
FOREACH(a, IntList, my_list)
{
    printf("%d\n", a);
}
```

`FOREACH(elem, vtype, vp)`宏的第一个参数`elem`是迭代变量，用它来表示每个元素的值。第二个参数`vtype`是需要遍历的表的 _`vtype`_。第三个参数`vp`是表的指针。

最后，不要忘记释放表占用的内存。

```
IntList_deinit(my_list);
```

_`vtype`_`_deinit`能释放表占用的内存，并且一一调用之前指定的析构函数，确保没有内存泄漏。它的唯一参数是表的指针。

完整代码是这样的：

```
#include "vec.h"

DECL_VEC(IntList, int, EMPTY_PREF)//声明函数原型和结构体
DEF_VEC(IntList, int, EMPTY_PREF)//定义函数

int main(void)
{
    IntList *my_list = IntList_init(NULL, NULL);//创建列表
    for (int n = 1; n <= 10; ++n)
    {
        IntList_push_back(my_list, n);//添加1-10
    }
    int a;
    FOREACH(a, IntList, my_list)//遍历，打印
    {
        printf("%d\n", a);
    }
    IntList_deinit(my_list);//回收资源
    return 0;
}
```

## 3. <a name='API'></a>API

_用斜体书写的 `vtype` 和 `type` ，应该用 `DECL_VEC`和 `DEF_VEC`的对应实参替换。_

### 3.1. <a name='vtype_init'></a>_vtype_\_init

##### 简介：

创建一个空列表。

##### 函数签名：

> _prefix_ _vtype_ \*_vtype_\_init(_vtype_\_copy*t copy, \_vtype*\_deinit_t deinit)

##### 回调函数：

> _type_ (_vtype_\_copy*t)(\_type* obj)

> void (_vtype_\_deinit*t)(\_type* obj)

##### 参数：

| 参数   | 描述                                                                  |
| ------ | --------------------------------------------------------------------- |
| copy   | 用来拷贝*type*类型对象的回调函数。如果为 NULL，则使用默认的拷贝方式。 |
| deinit | 用来回收*type*类型对象的回调函数。如果为 NULL，则使用默认的拷贝方式。 |

##### 说明：

对于基本类型或只含有基本类型的结构体的，默认的拷贝方式已经够用。对于需要深拷贝的情况，则需要自定义拷贝函数。例如有一个存放`char*`的列表，每个元素都指向 C 风格字符串。如果希望每次添加元素都对元素进行深拷贝，可以定义如下函数作为 copy 的参数。

```
char* copy_cstr(char* src)
{
    char* dest = malloc(strlen(src)+1);
    strcpy(dest,src);
    return dest;
}
```

再定义以下函数作为 deinit 的参数。

```
void free_cstr(char* str)
{
    free(str);
}
```

### 3.2. <a name='vtype_init_by_array'></a>_vtype_\_init_by_array

##### 简介：

根据数组创建列表。

##### 函数签名：

> _prefix_ _vtype_ \*_vtype_\_init*by_array(const \_type* \*array, size*t count, \_vtype*\_copy*t copy, \_vtype*\_deinit_t deinit);

##### 回调函数：

> _type_ (_vtype_\_copy*t)(\_type* obj)

> void (_vtype_\_deinit*t)(\_type* obj)

##### 参数：

| 参数   | 描述                                                                  |
| ------ | --------------------------------------------------------------------- |
| array  | 创建列表的依据。将会拷贝其中的每一个元素。                            |
| count  | 数组的元素个数。                                                      |
| copy   | 用来拷贝*type*类型对象的回调函数。如果为 NULL，则使用默认的拷贝方式。 |
| deinit | 用来回收*type*类型对象的回调函数。如果为 NULL，则使用默认的拷贝方式。 |

##### 说明：

创建列表，并拷贝数组中的每一个元素到此列表中。

### 3.3. <a name='vtype_deinit'></a>_vtype_\_deinit

##### 简介：

根据数组创建列表。

##### 函数签名：

> _prefix_ void _vtype_\_deinit(_vtype_ \*v)

##### 参数：

| 参数 | 描述         |
| ---- | ------------ |
| v    | 列表的指针。 |

##### 说明：

销毁列表，对每一个元素调用 deinit 回调函数（NULL 则无动作）。

### 3.4. <a name='vtype_push_back'></a>_vtype_\_push_back

##### 简介：

向列表尾部添加元素。

##### 函数签名：

> _prefix_ void vtype\*\_push_back(vtype \*v, type obj)

##### 参数：

| 参数 | 描述                                         |
| ---- | -------------------------------------------- |
| v    | 列表的指针。                                 |
| obj  | 添加的元素。将用 copy 回调函数拷贝它再添加。 |

##### 说明：

向列表尾部添加元素，添加时用 copy 回调函数进行拷贝（NULL 则使用`=`直接赋值）。

### 3.5. <a name='vtype_get'></a>_vtype_\_get

##### 简介：

取得元素。

##### 函数签名：

> _prefix_ _type_ _vtype_\_get(const _vtype_ \*v, size_t index);

##### 参数：

| 参数  | 描述                  |
| ----- | --------------------- |
| v     | 列表的指针。          |
| index | 取第 index-1 个元素。 |

##### 说明：

返回列表中的第 index-1 个元素，不会调用 copy 回调函数。

### 3.6. <a name='vtype_erase'></a>_vtype_\_erase

##### 简介：

删除元素。

##### 函数签名：

> _prefix_ _vtype_\_iter _vtype_\_erase(_vtype_ \*v, _vtype_\_iter i)

##### 参数：

| 参数 | 描述                     |
| ---- | ------------------------ |
| v    | 列表的指针。             |
| i    | 指向被删除元素的迭代器。 |

##### 返回值：

指向被删除元素下一个元素的迭代器。如果被删除的元素在末尾，就返回尾后迭代器。

##### 说明：

删除迭代器指向的元素，调用 deinit 回调函数进行析构（NULL 则无动作）。

### 3.7. <a name='vtype_erase_range'></a>_vtype_\_erase_range

##### 简介：

删除一些元素。

##### 函数签名：

> _prefix_ _vtype_\_iter _vtype_\_erase*range(\_vtype* \*v, _vtype_\_iter first, _vtype_\_iter last)

##### 参数：

| 参数  | 描述                                           |
| ----- | ---------------------------------------------- |
| first | 迭代器，指向被删除的第一个元素。               |
| last  | 迭代器，指向被删除的最后一个元素的下一个元素。 |

##### 返回值：

指向被删除序列后一个元素的迭代器。

##### 说明：

删除前会调用 deinit 函数。

### 3.8. <a name='vtype_pop'></a>_vtype_\_pop

##### 简介：

返回列表末尾的元素的副本并从列表中删除。

##### 函数签名：

> _prefix_ _type_ _vtype_\_pop(_vtype_ \*v)

##### 参数：

| 参数 | 描述           |
| ---- | -------------- |
| v    | 指向列表的指针 |

##### 返回值：

列表末尾的元素的副本。

##### 说明：

副本由调用 copy 而来。删除前会调用 deinit 函数。

### 3.9. <a name='vtype_begin'></a>_vtype_\_begin

##### 简介：

取得指向第一个元素的迭代器。

##### 函数签名：

> _prefix_ _vtype_\_iter _vtype_\_begin(const _vtype_ \*v)

##### 参数：

| 参数 | 描述         |
| ---- | ------------ |
| v    | 列表的指针。 |

##### 返回值：

指向第一个元素的迭代器。

### 3.10. <a name='vtype_end'></a>_vtype_\_end

##### 简介：

取得指向的前一个元素是列表最后一个元素的迭代器。

##### 函数签名：

> _prefix_ _vtype_\_iter _vtype_\_end(const _vtype_ \*v)

##### 参数：

| 参数 | 描述         |
| ---- | ------------ |
| v    | 列表的指针。 |

##### 返回值：

指向的前一个元素是列表最后一个元素的迭代器，即尾后迭代器。

##### 说明：

不要对此迭代器进行解引用操作，结果将是未定义的。

### 3.11. <a name='vtype_next'></a>_vtype_\_next

##### 简介：

将迭代器后移一个元素。

##### 函数签名：

> _prefix_ _vtype_\_iter _vtype_\_next(_vtype_\_iter \*i)

##### 参数：

| 参数 | 描述               |
| ---- | ------------------ |
| i    | 指向迭代器的指针。 |

##### 返回值：

移动后的迭代器。

##### 说明：

不会进行边界检查。可以用`++i`代替。

### 3.12. <a name='vtype_prev'></a>_vtype_\_prev

##### 简介：

将迭代器前移一个元素。

##### 函数签名：

> _prefix_ _vtype_\_iter _vtype_\_prev(_vtype_\_iter \*i)

##### 参数：

| 参数 | 描述               |
| ---- | ------------------ |
| i    | 指向迭代器的指针。 |

##### 返回值：

移动后的迭代器。

##### 说明：

不会进行边界检查。可以用`--i`代替。

### 3.13. <a name='vtype_deref'></a>_vtype_\_deref

##### 简介：

解引用迭代器。

##### 函数签名：

> _prefix_ _type_\_iter _vtype_\_deref(_vtype_\_iter \*i)

##### 参数：

| 参数 | 描述     |
| ---- | -------- |
| i    | 迭代器。 |

##### 返回值：

迭代器指向的元素。

##### 说明：

不会调用 copy 回调函数，可以用作为右值的`*i`代替。

### 3.14. <a name='vtype_eqi'></a>_vtype_\_eqi

##### 简介：

返回两个迭代器是否指向同一个元素。

##### 函数签名：

> _prefix_ int _vtype_\_eqi(_vtype_\_iter ia, _vtype_\_iter ib)

##### 参数：

| 参数 | 描述             |
| ---- | ---------------- |
| ia   | 要比较的迭代器。 |
| ib   | 要比较的迭代器。 |

##### 返回值：

迭代器是否指向同一个元素。否，返回 0；是，返回非零。

##### 说明：

可以用`ia == ib`代替。

### 3.15. <a name='FOREACH'></a>FOREACH

##### 简介：

遍历列表。

##### 宏：

> FOREACH(elemi, vtype, vp)

##### 参数：

| 参数  | 描述                                                           |
| ----- | -------------------------------------------------------------- |
| elemi | 迭代变量，每次遍历赋给它当前元素的值（不调用 copy 回调函数）。 |
| vtype | 列表的*vtype*                                                  |
| vp    | 指向列表的指针                                                 |

##### 说明：

用 FOREACH 可以方便地遍历列表，具体可以参照 2.2. [调用接口](#-1) 中给出的例子。
