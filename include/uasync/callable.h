#ifndef CALLABLE_H_
#define CALLABLE_H_

#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef VA_LENGTH

#define VA_LENGTH(...) \
            VA_LENGTH_(__VA_ARGS__, VA_LENGTH_RSEQ_N())

#define VA_LENGTH_(...) \
            VA_LENGTH_N(__VA_ARGS__)

#define VA_LENGTH_N( \
            _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
            _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
            _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
            _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
            _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
            _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
            _61,_62,_63,N,...) N

#define VA_LENGTH_RSEQ_N() \
            63,62,61,60, \
            59,58,57,56,55,54,53,52,51,50, \
            49,48,47,46,45,44,43,42,41,40, \
            39,38,37,36,35,34,33,32,31,30, \
            29,28,27,26,25,24,23,22,21,20, \
            19,18,17,16,15,14,13,12,11,10, \
            9,8,7,6,5,4,3,2,1,0

#define VA_LENGTH_LESS_1(...) \
            VA_LENGTH_LESS_1_(__VA_ARGS__, 63, 62, 61, \
            60, 59, 58, 57, 56, 55, 54, 53, 52, 51,		 \
            50, 49, 48, 47, 46, 45, 44, 43, 42, 41,		 \
            40, 39, 38, 37, 36, 35, 34, 33, 32, 31,		 \
            30, 29, 28, 27, 26, 25, 24, 23, 22, 21,		 \
            20, 19, 18, 17, 16, 15, 14, 13, 12, 11,		 \
            10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define VA_LENGTH_LESS_1_(  \
            _ignored,       \
            _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,		\
            _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,	\
            _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,	\
            _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,	\
            _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,	\
            _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,	\
            _61, _62, N, ...) N

#define VA_IGNORE_ONE(...) VA_IGNORE_ONE_(__VA_ARGS__)
#define VA_IGNORE_ONE_(_1, ...) __VA_ARGS__

#endif

#define ARG_CAT(a,...) ARG_CAT_(a,__VA_ARGS__)
#define ARG_CAT_(a,...) a##__VA_ARGS__


/* arg pair macros */
#define ARG_FIRST(arg1, arg2) arg1
#define ARG_SECOND(arg1, arg2) arg2
#define ARG_PASS(arg1, arg2) arg1 arg2

#define ARG_ODD(...) ARG_PAIR(ARG_FIRST, __VA_ARGS__)
#define ARG_EVEN(...) ARG_PAIR(ARG_SECOND, __VA_ARGS__)
#define ARG_PAIR(...) ARG_PAIR_N(VA_LENGTH_LESS_1(__VA_ARGS__), __VA_ARGS__)
#define ARG_PAIR_N(N, ...) ARG_CAT(ARG_PAIR_, N)(__VA_ARGS__)

#define ARG_PAIR_1(...)
#define ARG_PAIR_2(F, arg1, arg2, ...)  F(arg1, arg2)
#define ARG_PAIR_4(F, arg1, arg2, ...)  F(arg1, arg2), ARG_PAIR_2(F, __VA_ARGS__)
#define ARG_PAIR_6(F, arg1, arg2, ...)  F(arg1, arg2), ARG_PAIR_4(F, __VA_ARGS__)
#define ARG_PAIR_8(F, arg1, arg2, ...)  F(arg1, arg2), ARG_PAIR_6(F, __VA_ARGS__)
#define ARG_PAIR_10(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_8(F, __VA_ARGS__)
#define ARG_PAIR_12(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_10(F, __VA_ARGS__)
#define ARG_PAIR_14(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_12(F, __VA_ARGS__)
#define ARG_PAIR_16(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_14(F, __VA_ARGS__)
#define ARG_PAIR_18(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_16(F, __VA_ARGS__)
#define ARG_PAIR_20(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_18(F, __VA_ARGS__)
#define ARG_PAIR_30(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_20(F, __VA_ARGS__)
#define ARG_PAIR_32(F, arg1, arg2, ...) F(arg1, arg2), ARG_PAIR_30(F, __VA_ARGS__)


/* arg expension macro */
#define ARG_APPEND(...) \
            ARG_APPEND_N(VA_LENGTH_LESS_1(__VA_ARGS__), __VA_ARGS__)

#define ARG_APPEND_N(N, F, ...) \
            ARG_CAT(ARG_APPEND_, N)(F, __VA_ARGS__)

#define ARG_APPEND_0(...)
#define ARG_APPEND_1(F, arg, ...) F(arg, 1)
#define ARG_APPEND_2(F, arg, ...) F(arg, 2) ARG_APPEND_1(F, __VA_ARGS__)
#define ARG_APPEND_3(F, arg, ...) F(arg, 3) ARG_APPEND_2(F, __VA_ARGS__)
#define ARG_APPEND_4(F, arg, ...) F(arg, 4) ARG_APPEND_3(F, __VA_ARGS__)
#define ARG_APPEND_5(F, arg, ...) F(arg, 5) ARG_APPEND_4(F, __VA_ARGS__)
#define ARG_APPEND_6(F, arg, ...) F(arg, 6) ARG_APPEND_5(F, __VA_ARGS__)
#define ARG_APPEND_7(F, arg, ...) F(arg, 7) ARG_APPEND_6(F, __VA_ARGS__)
#define ARG_APPEND_8(F, arg, ...) F(arg, 8) ARG_APPEND_7(F, __VA_ARGS__)
#define ARG_APPEND_9(F, arg, ...) F(arg, 9) ARG_APPEND_8(F, __VA_ARGS__)
#define ARG_APPEND_10(F, arg, ...) F(arg, 10) ARG_APPEND_9(F, __VA_ARGS__)
#define ARG_APPEND_11(F, arg, ...) F(arg, 11) ARG_APPEND_10(F, __VA_ARGS__)
#define ARG_APPEND_12(F, arg, ...) F(arg, 12) ARG_APPEND_11(F, __VA_ARGS__)
#define ARG_APPEND_13(F, arg, ...) F(arg, 13) ARG_APPEND_12(F, __VA_ARGS__)
#define ARG_APPEND_14(F, arg, ...) F(arg, 14) ARG_APPEND_13(F, __VA_ARGS__)
#define ARG_APPEND_15(F, arg, ...) F(arg, 15) ARG_APPEND_14(F, __VA_ARGS__)


/* callable struct definition */
typedef struct callable_obj_base {
    void (*invoke)(void*);
} callable_obj_base;

#define COMMA_UNWRAP(__VA_ARGS__) VA_IGNORE_ONE(0 __VA_ARGS__)
#define PASS_ARG(arg, N) arg
#define COMMA_ARG(arg, N) ,arg
#define TYPE_PLACE_ARG(type, N) type ARG_CAT(_, N);
#define COMMA_PLACE(_ignored, N) , _ ## N
#define COMMA_PLACE_ARG(arg, N) , arg _ ## N
#define COMMA_INVOKE_ARG(_ignored, N) , ARG_CAT(obj->_, N)
#define INIT_PLACE_ARG(_ignored, N) ARG_CAT(obj->_, N) = ARG_CAT(_, N);

/* callable invoke function definition */
#define CALLABLE_DECALRE(name, ...) \
    typedef struct { \
        callable_obj_base base; \
        void (*fn)(__VA_ARGS__); \
        ARG_APPEND(TYPE_PLACE_ARG, __VA_ARGS__); \
    } callable_obj_ ## name; \
    static void callable_obj_ ## name ## _handler (__VA_ARGS__); \
    static void callable_obj_ ## name ## _invoke (void* ptr) { \
        callable_obj_ ## name * obj = (callable_obj_ ## name *) ptr; \
        obj->fn(COMMA_UNWRAP(ARG_APPEND(COMMA_INVOKE_ARG, __VA_ARGS__))); \
    } \
    static void callable_obj_ ## name ## _init(callable_obj_## name * obj \
                            ARG_APPEND(COMMA_PLACE_ARG, __VA_ARGS__)) \
    { \
        obj->base.invoke = callable_obj_ ## name ## _invoke; \
        obj->fn = callable_obj_ ## name ## _handler; \
        ARG_APPEND(INIT_PLACE_ARG, __VA_ARGS__); \
    } \
    static inline callable_obj_base * callable_obj_ ## name ## _new( \
                COMMA_UNWRAP(ARG_APPEND(COMMA_PLACE_ARG, __VA_ARGS__))) { \
        callable_obj_ ## name * obj = (callable_obj_ ## name *)malloc(sizeof (callable_obj_ ## name )); \
        callable_obj_ ## name ## _init(obj ARG_APPEND(COMMA_PLACE, __VA_ARGS__)); \
        return (callable_obj_base *)obj; \
    }

/**example
 *
 *      CALLABLE_DEFINE(mycallable,
 *                    CALLABLE_ARG(int, a),
 *                    CALLABLE_ARG(int, b),
 *                    CALLABLE_ARG(char*, c))
 *      {
 *          printf ("mycallable %d, %d, %s \n", a, b, c);
 *      }
 *
 *      ...
 *
 *      CALLABLE_OBJ(mycallable) obj = CALLABLE_INITIALIZER(mycallable, 1, 2, "hello");
 *      callable_invoke(&obj);
 *
 *      ...
 *
 *      CALLABLE_OBJ(mycallable)* obj_ptr = (CALLABLE_OBJ(mycallable)*)malloc (sizeof (CALLABLE_OBJ(mycallable));
 *      CALLABLE_INIT(mycallable, obj_ptr, 10, 20, "ptr init");
 *
 *      ...
 *
 *      callable_obj_base* callable = CALLABLE_NEW(mycallable, 100, 200, "new obj");
 *      callable_invoke(callable);
 *      callable_free(callable);
 *
 */
#define CALLABLE_DEFINE(name, ...) \
    CALLABLE_DECALRE(name, ARG_ODD(__VA_ARGS__)) \
    static void callable_obj_ ## name ## _handler (ARG_PAIR(ARG_PASS, __VA_ARGS__))

#define CALLABLE_ARG(type, var) type, var
#define CALLABLE_OBJ(name) callable_obj_##name
#define CALLABLE_INIT(name, obj, ...) callable_obj_##name##_init(obj, __VA_ARGS__)
#define CALLABLE_NEW(name, ...) callable_obj_ ## name ## _new(__VA_ARGS__)

#define CALLABLE_INITIALIZER(name, ...) \
      { {callable_obj_ ## name ## _invoke}, \
        callable_obj_ ## name ## _handler \
        ARG_APPEND(COMMA_ARG, __VA_ARGS__) }

static inline void callable_free(callable_obj_base* obj)
{
    free (obj);
}

static inline void callable_invoke(callable_obj_base* obj)
{
    obj->invoke(obj);
}

#ifdef __cplusplus
}
#endif
#endif // CALLABLE_H_
