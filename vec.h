#ifndef VEC_H
#define VEC_H
#include <stdlib.h>
#define INITIAL_CAPACITY 5
#define GROWTH_RATE 2
#define EMPTY_PREF
#define DECL_VEC(vtype, type, prefix)                                                                                  \
    typedef type (*vtype##_copy_t)(type obj);                                                                          \
    typedef void (*vtype##_deinit_t)(type obj);                                                                        \
    typedef struct vtype                                                                                               \
    {                                                                                                                  \
        type *array;                                                                                                   \
        size_t size;                                                                                                   \
        size_t capacity;                                                                                               \
        vtype##_copy_t copy;                                                                                           \
        vtype##_deinit_t deinit;                                                                                       \
    } vtype;                                                                                                           \
    typedef struct vtype##_iter                                                                                        \
    {                                                                                                                  \
        type *ptr;                                                                                                     \
    } vtype##_iter;                                                                                                    \
                                                                                                                       \
    /*迭代器相关*/                                                                                                \
    prefix vtype##_iter vtype##_begin(const vtype *v);                                                                 \
    prefix vtype##_iter vtype##_end(const vtype *v);                                                                   \
    prefix void vtype##_next(vtype##_iter *i);                                                                         \
    prefix void vtype##_prev(vtype##_iter *i);                                                                         \
    prefix type vtype##_deref(vtype##_iter i);                                                                         \
    prefix int vtype##_eqi(vtype##_iter ia, vtype##_iter ib);                                                          \
    /*vector相关*/                                                                                                   \
    prefix vtype *vtype##_init(vtype##_copy_t, vtype##_deinit_t);                                                      \
    prefix vtype *vtype##_init_by_array(const type *array, size_t size, vtype##_copy_t copy, vtype##_deinit_t deinit); \
    prefix void vtype##_deinit(vtype *v);                                                                              \
    prefix void vtype##_push_back(vtype *v, type obj);                                                                 \
    prefix type vtype##_get(const vtype *v, size_t index);

#define DEF_VEC(vtype, type, prefix)                                                                                  \
    /* begin 取得头指针*/                                                                                        \
    prefix vtype##_iter vtype##_begin(const vtype *v)                                                                 \
    {                                                                                                                 \
        vtype##_iter iter;                                                                                            \
        iter.ptr = v->array;                                                                                          \
        return iter;                                                                                                  \
    }                                                                                                                 \
    /* end 取得尾后指针*/                                                                                       \
    prefix vtype##_iter vtype##_end(const vtype *v)                                                                   \
    {                                                                                                                 \
        vtype##_iter iter;                                                                                            \
        iter.ptr = v->array + v->size;                                                                                \
        return iter;                                                                                                  \
    }                                                                                                                 \
    /* next 指针后移1*/                                                                                           \
    prefix void vtype##_next(vtype##_iter *i)                                                                         \
    {                                                                                                                 \
        i->ptr++;                                                                                                     \
    }                                                                                                                 \
    /* prev 指针前移1*/                                                                                           \
    prefix void vtype##_prev(vtype##_iter *i)                                                                         \
    {                                                                                                                 \
        i->ptr--;                                                                                                     \
    }                                                                                                                 \
    /* get_by_iter 取得指针指向的元素*/                                                                      \
    prefix type vtype##_deref(vtype##_iter i)                                                                         \
    {                                                                                                                 \
        return *(i.ptr);                                                                                              \
    }                                                                                                                 \
    /* eqi 指针是否相等*/                                                                                       \
    prefix int vtype##_eqi(vtype##_iter ia, vtype##_iter ib)                                                          \
    {                                                                                                                 \
        return (ia.ptr == ib.ptr);                                                                                    \
    }                                                                                                                 \
    /* default_copy 自动生成的拷贝函数*/                                                                     \
    prefix type vtype##_default_copy(type obj)                                                                        \
    {                                                                                                                 \
        return obj;                                                                                                   \
    }                                                                                                                 \
    /* default_deinit 自动生成的析构函数*/                                                                   \
    prefix void vtype##_default_deinit(type obj) {}                                                                   \
    /* init 创建vector*/                                                                                            \
    prefix vtype *vtype##_init(vtype##_copy_t copy, vtype##_deinit_t deinit)                                          \
    {                                                                                                                 \
        vtype *v = malloc(sizeof(vtype));                                                                             \
        v->copy = copy ? copy : vtype##_default_copy;                                                                 \
        v->deinit = deinit ? deinit : vtype##_default_deinit;                                                         \
        v->capacity = INITIAL_CAPACITY;                                                                               \
        v->array = malloc(sizeof(type) * v->capacity);                                                                \
        v->size = 0;                                                                                                  \
        return v;                                                                                                     \
    }                                                                                                                 \
    /* init_by_array 从数组创建vector*/                                                                          \
    prefix vtype *vtype##_init_by_array(const type *array, size_t size, vtype##_copy_t copy, vtype##_deinit_t deinit) \
    {                                                                                                                 \
        vtype *v = malloc(sizeof(vtype));                                                                             \
        v->copy = copy ? copy : vtype##_default_copy;                                                                 \
        v->deinit = deinit ? deinit : vtype##_default_deinit;                                                         \
        v->capacity = (size_t)(size * GROWTH_RATE);                                                                   \
        v->size = size;                                                                                               \
        v->array = malloc(sizeof(type) * v->capacity);                                                                \
        for (size_t i = 0; i < size; ++i)                                                                             \
        {                                                                                                             \
            v->array[i] = v->copy((type)array[i]);                                                                    \
        }                                                                                                             \
        return v;                                                                                                     \
    }                                                                                                                 \
    /* deinit 析构vector*/                                                                                          \
    prefix void vtype##_deinit(vtype *v)                                                                              \
    {                                                                                                                 \
        for (size_t i = 0; i < v->size; ++i)                                                                          \
        {                                                                                                             \
            v->deinit(v->array[i]);                                                                                   \
        }                                                                                                             \
        free(v->array);                                                                                               \
        free(v);                                                                                                      \
    }                                                                                                                 \
    /* push_back 尾部添加元素*/                                                                                 \
    prefix void vtype##_push_back(vtype *v, type obj)                                                                 \
    {                                                                                                                 \
        if (v->size < v->capacity)                                                                                    \
        {                                                                                                             \
            v->array[v->size] = v->copy(obj);                                                                         \
        }                                                                                                             \
        else                                                                                                          \
        { /*大小不够，数据搬家*/                                                                             \
            v->capacity *= GROWTH_RATE;                                                                               \
            type *new_mem = malloc(sizeof(type) * v->capacity);                                                       \
            memcpy(new_mem, v->array, sizeof(type) * v->size);                                                        \
            free(v->array);                                                                                           \
            v->array = new_mem;                                                                                       \
            v->array[v->size] = v->copy(obj);                                                                         \
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
        type *p = i.ptr;                                                                                              \
        v->deinit(*p);                                                                                                \
        type *ep = vtype##_end(v).ptr;                                                                                \
        memmove(p, p + 1, (ep - (p + 1)) * sizeof(type));                                                             \
        v->size--;                                                                                                    \
        return i;                                                                                                     \
    }

#define FOREACH(elemi, vtype, cp)                \
    elemi = vtype##_deref(vtype##_begin(cp));   \
    for (vtype##_iter iter = vtype##_begin(cp); \
         !vtype##_eqi(iter, vtype##_end(cp));   \
         vtype##_next(&iter), elemi = vtype##_deref(iter))

#endif