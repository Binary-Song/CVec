#ifndef VEC_H
#define VEC_H

#if __STDC_VERSION__ < 199901L //NOT C99
#define inline
#endif

#include <stdlib.h>
#include <string.h>
#define INITIAL_CAPACITY 5
#define GROWTH_RATE 2
#define EMPTY_PREF
#define DECL_VEC(vtype, type, prefix)                                                                                   \
    typedef type (*vtype##_copy_t)(type obj);                                                                           \
    typedef void (*vtype##_deinit_t)(type obj);                                                                         \
    typedef struct vtype                                                                                                \
    {                                                                                                                   \
        type *array;                                                                                                    \
        size_t size;                                                                                                    \
        size_t capacity;                                                                                                \
        vtype##_copy_t copy;                                                                                            \
        vtype##_deinit_t deinit;                                                                                        \
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
    prefix vtype *vtype##_init(vtype##_copy_t, vtype##_deinit_t);                                                       \
    prefix vtype *vtype##_init_by_array(const type *array, size_t count, vtype##_copy_t copy, vtype##_deinit_t deinit); \
    prefix void vtype##_deinit(vtype *v);                                                                               \
    prefix void vtype##_push_back(vtype *v, type obj);                                                                  \
    prefix type vtype##_get(const vtype *v, size_t index);                                                              \
    prefix vtype##_iter vtype##_erase(vtype *v, vtype##_iter i);

#define DEF_VEC(vtype, type, prefix) /* begin 取得头指针*/                                                       \
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
    /* smart_copy 不为NULL才调用copy（就这？） */                                                           \
    static inline type vtype##_smart_copy(vtype##_copy_t copy, type obj)                                              \
    {                                                                                                                 \
        return copy ? copy((type)obj) : obj;                                                                          \
    }                                                                                                                 \
    /* smart_deinit 不为NULL才调用deinit（就这？） */                                                       \
    static inline void vtype##_smart_deinit(vtype##_deinit_t deinit, type obj)                                        \
    {                                                                                                                 \
        if (deinit)                                                                                                   \
            deinit(obj);                                                                                              \
    }                                                                                                                 \
    /* init 创建vector*/                                                                                            \
    prefix vtype *vtype##_init(vtype##_copy_t copy, vtype##_deinit_t deinit)                                          \
    {                                                                                                                 \
        vtype *v = malloc(sizeof(vtype));                                                                             \
        v->copy = copy;                                                                                               \
        v->deinit = deinit;                                                                                           \
        v->capacity = INITIAL_CAPACITY;                                                                               \
        v->array = malloc(sizeof(type) * v->capacity);                                                                \
        v->size = 0;                                                                                                  \
        return v;                                                                                                     \
    }                                                                                                                 \
    /* init_by_array 从数组创建vector*/                                                                          \
    prefix vtype *vtype##_init_by_array(const type *array, size_t size, vtype##_copy_t copy, vtype##_deinit_t deinit) \
    {                                                                                                                 \
        vtype *v = malloc(sizeof(vtype));                                                                             \
        v->copy = copy;                                                                                               \
        v->deinit = deinit;                                                                                           \
        v->capacity = (size_t)(size * GROWTH_RATE);                                                                   \
        v->size = size;                                                                                               \
        v->array = malloc(sizeof(type) * v->capacity);                                                                \
        for (size_t i = 0; i < size; ++i)                                                                             \
        {                                                                                                             \
            v->array[i] = vtype##_smart_copy(v->copy, (type)array[i]);                                                \
        }                                                                                                             \
        return v;                                                                                                     \
    }                                                                                                                 \
    /* deinit 析构vector*/                                                                                          \
    prefix void vtype##_deinit(vtype *v)                                                                              \
    {                                                                                                                 \
        for (size_t i = 0; i < v->size; ++i)                                                                          \
        {                                                                                                             \
            vtype##_smart_deinit(v->deinit, v->array[i]);                                                             \
        }                                                                                                             \
        free(v->array);                                                                                               \
        free(v);                                                                                                      \
    }                                                                                                                 \
    /* push_back 尾部添加元素*/                                                                                 \
    prefix void vtype##_push_back(vtype *v, type obj)                                                                 \
    {                                                                                                                 \
        if (v->size < v->capacity)                                                                                    \
        {                                                                                                             \
            v->array[v->size] = vtype##_smart_copy(v->copy, obj);                                                     \
        }                                                                                                             \
        else                                                                                                          \
        { /*大小不够，数据搬家*/                                                                             \
            v->capacity *= GROWTH_RATE;                                                                               \
            type *new_mem = malloc(sizeof(type) * v->capacity);                                                       \
            memcpy(new_mem, v->array, sizeof(type) * v->size);                                                        \
            free(v->array);                                                                                           \
            v->array = new_mem;                                                                                       \
            v->array[v->size] = vtype##_smart_copy(v->copy, obj);                                                     \
        }                                                                                                             \
        v->size++;                                                                                                    \
    }                                                                                                                 \
    /* get 取得元素*/                                                                                             \
    prefix type vtype##_get(const vtype *v, size_t index)                                                             \
    {                                                                                                                 \
        return v->array[index];                                                                                       \
    }                                                                                                                 \
    /* erase 删除元素 */                                                                                          \
    prefix vtype##_iter vtype##_erase(vtype *v, vtype##_iter i)                                                       \
    {                                                                                                                 \
        type *p = i;                                                                                                  \
        type *ep = vtype##_end(v);                                                                                    \
        vtype##_smart_deinit(v->deinit, *p);                                                                          \
        memmove(p, p + 1, (ep - (p + 1)) * sizeof(type));                                                             \
        v->size--;                                                                                                    \
        return i;                                                                                                     \
    }                                                                                                                 \
    /* erase_range 删除一些元素*/                                                                               \
    prefix vtype##_iter vtype##_erase_range(vtype *v, vtype##_iter first, vtype##_iter last)                          \
    {                                                                                                                 \
        for (vtype##_iter i = first; i != last; ++i)                                                                  \
        {                                                                                                             \
            vtype##_smart_deinit(v->deinit, *i);                                                                      \
        }                                                                                                             \
        memmove(first, last, (vtype##_end(v) - last) * sizeof(type));                                                 \
        v->size -= last - first;                                                                                      \
        return first;                                                                                                 \
    }                                                                                                                 \
    /* pop 删除最后的元素，复制一份返回*/                                                               \
    prefix type vtype##_pop(vtype *v)                                                                                 \
    {                                                                                                                 \
        type *p = vtype##_end(v) - 1;                                                                                 \
        type copied = vtype##_smart_copy(v->copy, *p);                                                                \
        vtype##_smart_deinit(v->deinit, *p);                                                                          \
        v->size--;                                                                                                    \
        return copied;                                                                                                \
    }

#define FOREACH(elemi, vtype, cp)               \
    elemi = vtype##_deref(vtype##_begin(cp));   \
    for (vtype##_iter iter = vtype##_begin(cp); \
         !vtype##_eqi(iter, vtype##_end(cp));   \
         vtype##_next(&iter), elemi = vtype##_deref(iter))

#endif