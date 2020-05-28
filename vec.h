#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define EMPTY_PREFIX

/* 配置 */

#define INITIAL_CAPACITY 5 //列表初始容量
#define GROWTH_RATE 2      //列表容量增长倍率
#define BOUND_CHECK 0      //是否检查边界

/* end of 配置 */

#define DECL_DEF_VEC(vtype, type, prefix, rqc, rqd) \
    DECL_VEC(vtype, type, prefix, rqc, rqd)         \
    DEF_VEC(vtype, type, prefix, rqc, rqd)

#define DECL_VEC(vtype, type, prefix, rqc, rqd)                                                                         \
    typedef type (*vtype##_copy_t)(type obj);                                                                           \
    typedef void (*vtype##_deinit_t)(type obj);                                                                         \
    typedef struct vtype                                                                                                \
    {                                                                                                                   \
        type *array;                                                                                                    \
        size_t size;                                                                                                    \
        size_t capacity;                                                                                                \
        STRUCT_FP_DEF(vtype, rqc, rqd)                                                                                  \
    } vtype;                                                                                                            \
    typedef type *vtype##_iter;                                                                                         \
                                                                                                                        \
    /*迭代器相关*/                                                                                                 \
    prefix vtype##_iter vtype##_begin(const vtype *v);                                                                  \
    prefix vtype##_iter vtype##_end(const vtype *v);                                                                    \
    prefix vtype##_iter vtype##_next(vtype##_iter *i);                                                                  \
    prefix vtype##_iter vtype##_prev(vtype##_iter *i);                                                                  \
    prefix type vtype##_deref(vtype##_iter i);                                                                          \
    prefix int vtype##_eqi(vtype##_iter ia, vtype##_iter ib);                                                           \
    /*vector相关*/                                                                                                    \
    prefix vtype *vtype##_init(INIT_PARAM_LIST(vtype, rqc, rqd));                                                       \
    prefix vtype *vtype##_init_by_array(const type *array, size_t count, vtype##_copy_t copy, vtype##_deinit_t deinit); \
    prefix void vtype##_deinit(vtype *v);                                                                               \
    prefix void vtype##_push_back(vtype *v, type obj);                                                                  \
    prefix type vtype##_get(const vtype *v, size_t index);                                                              \
    prefix vtype##_iter vtype##_erase(vtype *v, vtype##_iter i);                                                        \
    prefix vtype##_iter vtype##_erase_range(vtype *v, vtype##_iter first, vtype##_iter last);                           \
    prefix vtype##_iter vtype##_insert(vtype *v, vtype##_iter pos, type obj);
/*rqc = Requrie Copy Func*/
/*rqd = Require Deinit Func*/
#define DEF_VEC(vtype, type, prefix, rqc, rqd)                                                                        \
    /* begin 取得头指针*/                                                                                        \
    prefix vtype##_iter vtype##_begin(const vtype *v)                                                                 \
    {                                                                                                                 \
        return v->array;                                                                                              \
    }                                                                                                                 \
    /* end 取得尾后指针*/                                                                                       \
    prefix vtype##_iter vtype##_end(const vtype *v)                                                                   \
    {                                                                                                                 \
        return v->array + v->size;                                                                                    \
    }                                                                                                                 \
    /* next 指针后移1*/                                                                                           \
    prefix vtype##_iter vtype##_next(vtype##_iter *i)                                                                 \
    {                                                                                                                 \
        return ++(*i);                                                                                                \
    }                                                                                                                 \
    /* prev 指针前移1*/                                                                                           \
    prefix vtype##_iter vtype##_prev(vtype##_iter *i)                                                                 \
    {                                                                                                                 \
        return --(*i);                                                                                                \
    }                                                                                                                 \
    /* get_by_iter 取得指针指向的元素*/                                                                      \
    prefix type vtype##_deref(vtype##_iter i)                                                                         \
    {                                                                                                                 \
        return *(i);                                                                                                  \
    }                                                                                                                 \
    /* eqi 指针是否相等*/                                                                                       \
    prefix int vtype##_eqi(vtype##_iter ia, vtype##_iter ib)                                                          \
    {                                                                                                                 \
        return ia == ib;                                                                                              \
    }                                                                                                                 \
    /* init 创建vector*/                                                                                            \
    prefix vtype *vtype##_init(INIT_PARAM_LIST(vtype, rqc, rqd))                                                      \
    {                                                                                                                 \
        vtype *v = malloc(sizeof(vtype));                                                                             \
        INIT_ASSIGN_FP(rqc, rqd)                                                                                      \
        v->capacity = INITIAL_CAPACITY;                                                                               \
        v->array = malloc(sizeof(type) * v->capacity);                                                                \
        v->size = 0;                                                                                                  \
        return v;                                                                                                     \
    }                                                                                                                 \
    /* init_by_array 从数组创建vector*/                                                                          \
    prefix vtype *vtype##_init_by_array(const type *array, size_t size, vtype##_copy_t copy, vtype##_deinit_t deinit) \
    {                                                                                                                 \
        vtype *v = malloc(sizeof(vtype));                                                                             \
        INIT_ASSIGN_FP(rqc, rqd)                                                                                      \
        v->capacity = (size_t)(size * GROWTH_RATE);                                                                   \
        v->size = size;                                                                                               \
        v->array = malloc(sizeof(type) * v->capacity);                                                                \
        for (size_t i = 0; i < size; ++i)                                                                             \
        {                                                                                                             \
            v->array[i] = COPY##rqc(v->copy, (type)array[i]);                                                         \
        }                                                                                                             \
        return v;                                                                                                     \
    }                                                                                                                 \
    /* deinit 析构vector*/                                                                                          \
    prefix void vtype##_deinit(vtype *v)                                                                              \
    {                                                                                                                 \
        for (size_t i = 0; i < v->size; ++i)                                                                          \
        {                                                                                                             \
            DEINIT##rqd(v->deinit, v->array[i]);                                                                      \
        }                                                                                                             \
        free(v->array);                                                                                               \
        free(v);                                                                                                      \
    }                                                                                                                 \
    /* push_back 尾部添加元素*/                                                                                 \
    prefix void vtype##_push_back(vtype *v, type obj)                                                                 \
    {                                                                                                                 \
        if (v->size < v->capacity)                                                                                    \
        {                                                                                                             \
            v->array[v->size] = COPY##rqc(v->copy, obj);                                                              \
        }                                                                                                             \
        else                                                                                                          \
        { /*大小不够，数据搬家*/                                                                             \
            v->capacity *= GROWTH_RATE;                                                                               \
            type *new_mem = malloc(sizeof(type) * v->capacity);                                                       \
            memcpy(new_mem, v->array, sizeof(type) * v->size);                                                        \
            for (size_t i = 0; i < v->size; ++i)                                                                      \
            {                                                                                                         \
                DEINIT##rqd(v->deinit, v->array[i]);                                                                  \
            }                                                                                                         \
            free(v->array);                                                                                           \
            v->array = new_mem;                                                                                       \
            v->array[v->size] = COPY##rqc(v->copy, obj);                                                              \
        }                                                                                                             \
        v->size++;                                                                                                    \
    }                                                                                                                 \
    /* get 取得元素*/                                                                                             \
    prefix type vtype##_get(const vtype *v, size_t index)                                                             \
    {                                                                                                                 \
        CONCAT(CHK_IDX, BOUND_CHECK)                                                                                  \
        (v, index);                                                                                                   \
        return v->array[index];                                                                                       \
    }                                                                                                                 \
    /* erase 删除元素 */                                                                                          \
    prefix vtype##_iter vtype##_erase(vtype *v, vtype##_iter i)                                                       \
    {                                                                                                                 \
        CONCAT(CHK_PTR, BOUND_CHECK)                                                                                  \
        (vtype, type, v, i);                                                                                          \
        type *p = i;                                                                                                  \
        type *ep = vtype##_end(v);                                                                                    \
        DEINIT##rqd(v->deinit, *p);                                                                                   \
        memmove(p, p + 1, (ep - (p + 1)) * sizeof(type));                                                             \
        v->size--;                                                                                                    \
        return i;                                                                                                     \
    }                                                                                                                 \
    /* erase_range 删除一些元素*/                                                                               \
    prefix vtype##_iter vtype##_erase_range(vtype *v, vtype##_iter first, vtype##_iter last)                          \
    {                                                                                                                 \
        CONCAT(CHK_PTR, BOUND_CHECK)                                                                                  \
        (vtype, type, v, first);                                                                                      \
        CONCAT(CHK_PTR, BOUND_CHECK)                                                                                  \
        (vtype, type, v, last - 1);                                                                                   \
        memmove(first, last, (vtype##_end(v) - last) * sizeof(type));                                                 \
        v->size -= last - first;                                                                                      \
        return first;                                                                                                 \
    }                                                                                                                 \
    /* pop 删除最后的元素，复制一份返回*/                                                               \
    prefix type vtype##_pop(vtype *v)                                                                                 \
    {                                                                                                                 \
        CONCAT(CHK_SIZE, BOUND_CHECK)                                                                                 \
        (v);                                                                                                          \
        type *p = vtype##_end(v) - 1;                                                                                 \
        type copied = COPY##rqc(v->copy, *p);                                                                         \
        DEINIT##rqd(v->deinit, *p);                                                                                   \
        v->size--;                                                                                                    \
        return copied;                                                                                                \
    }                                                                                                                 \
    /* insert 插入元素*/                                                                                          \
    prefix vtype##_iter vtype##_insert(vtype *v, vtype##_iter p, type obj)                                            \
    {                                                                                                                 \
        CONCAT(CHK_PTR, BOUND_CHECK)                                                                                  \
        (vtype, type, v, p);                                                                                          \
        DEINIT##rqd(v->deinit, *p);                                                                                   \
        if (v->size >= v->capacity)                                                                                   \
        { /*大小不够，数据搬家*/                                                                             \
            v->capacity *= GROWTH_RATE;                                                                               \
            type *new_mem = malloc(sizeof(type) * v->capacity);                                                       \
            memcpy(new_mem, v->array, sizeof(type) * v->size);                                                        \
            free(v->array);                                                                                           \
            p = p - v->array + new_mem;                                                                               \
            v->array = new_mem;                                                                                       \
        }                                                                                                             \
        memmove(p + 1, p, (vtype##_end(v) - (p)) * sizeof(type));                                                     \
        *p = COPY##rqc(v->copy, obj);                                                                                 \
        v->size++;                                                                                                    \
        return p;                                                                                                     \
    }

#define FOREACH(elemi, vtype, cp)               \
    elemi = vtype##_deref(vtype##_begin(cp));   \
    for (vtype##_iter iter = vtype##_begin(cp); \
         !vtype##_eqi(iter, vtype##_end(cp));   \
         vtype##_next(&iter), elemi = vtype##_deref(iter))

/*
    CONCAT 能连接预处理 token 并确保展开宏

*/
#define CONCAT(a, b) DOUBLE_HASHTAG(a, b)
#define DOUBLE_HASHTAG(a, b) a##b

/*
    以下：末尾数字为 BOUND_CHECK 的值。决定是否检查边界。
    0:yes 1:no
    对于不同的情况分为检查指针、检查下标和检查size
*/
#define CHK_IDX0(v, index)
#define CHK_IDX1(v, index)             \
    if (index >= v->size || index < 0) \
    OUT_OF_BOUND_ERROR

#define CHK_PTR0(vtype, type, v, i)
#define CHK_PTR1(vtype, type, v, i)                  \
    if (i >= vtype##_end(v) || i < vtype##_begin(v)) \
    OUT_OF_BOUND_ERROR

#define CHK_SIZE0(v)
#define CHK_SIZE1(v)  \
    if (v->size <= 0) \
    OUT_OF_BOUND_ERROR

#define OUT_OF_BOUND_ERROR                        \
    {                                             \
        fprintf(stderr, "Access out of bounds!"); \
        exit(EXIT_FAILURE);                       \
    }
/*
    以下：末尾数字为宏参数 rqc 的值。决定拷贝的方式。
    0:直接赋值 1:调用copy函数 
*/
#define COPY0(cp_func, obj) obj
#define COPY1(cp_func, obj) cp_func(obj)
/*
    以下：末尾数字为宏参数 rqd 的值。决定是否 deinit。
    0:无动作 1:调用deinit函数
*/
#define DEINIT0(de_func, obj)
#define DEINIT1(de_func, obj) de_func(obj)
/*
    以下：末尾两位数字分别代表 rqc 和 rqd 的值。
    INIT_PARAM_LIST: init 函数的参数列表生成器
    rqc : require copy func
    rqd : require deinit func
*/
#define INIT_PARAM_LIST(vtype, rqc, rqd) INIT_PARAM_LIST_##rqc##_##rqd(vtype)
#define INIT_PARAM_LIST_0_0(vtype) void
#define INIT_PARAM_LIST_0_1(vtype) vtype##_copy_t copy
#define INIT_PARAM_LIST_1_0(vtype) vtype##_deinit_t deinit
#define INIT_PARAM_LIST_1_1(vtype) vtype##_copy_t copy, vtype##_deinit_t deinit
/*
    INIT_ASSIGN_FP: init 函数初始化 copy 和 deinit 函数指针的代码。
*/
#define INIT_ASSIGN_FP(rqc, rqd) INIT_ASSIGN_FP_##rqc##_##rqd
#define INIT_ASSIGN_FP_0_0
#define INIT_ASSIGN_FP_0_1 v->deinit = deinit;
#define INIT_ASSIGN_FP_1_0 v->copy = copy;
#define INIT_ASSIGN_FP_1_1 \
    v->deinit = deinit;    \
    v->copy = copy;
/*
    STRUCT_FP_DEF : vec 结构体中函数指针的定义
*/
#define STRUCT_FP_DEF(vtype, rqc, rqd) STRUCT_FP_DEF_##rqc##_##rqd(vtype)
#define STRUCT_FP_DEF_0_0(vtype)
#define STRUCT_FP_DEF_0_1(vtype) vtype##_deinit_t deinit;
#define STRUCT_FP_DEF_1_0(vtype) vtype##_copy_t copy;
#define STRUCT_FP_DEF_1_1(vtype) \
    vtype##_copy_t copy;         \
    vtype##_deinit_t deinit;

#endif