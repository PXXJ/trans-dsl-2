//
// Created by Darwin Yuan on 2020/6/20.
//

#ifndef TRANS_DSL_2_CONCEPTHELPER_H
#define TRANS_DSL_2_CONCEPTHELPER_H

#include <trans-dsl/tsl_config.h>

#if __CONCEPT_ENABLED
#define VOID_PLACEHOLDER
#define VOID_CONCEPT

#define CONCEPT_C(c, t) c t
#define ENABLE_C(c, t)
#define DEF_CONCEPT(c, ...) concept c = __VA_ARGS__
#define CONCEPT(c) c
#else
#define VOID_PLACEHOLDER , void
#define VOID_CONCEPT , typename = void
#define CONCEPT_C(c, t) typename t
#define ENABLE_C(c, ...) , std::enable_if_t<c<__VA_ARGS__>>
#define DEF_CONCEPT(c, ...) constexpr bool c = __VA_ARGS__
#define CONCEPT(c) typename
#endif

#endif //TRANS_DSL_2_CONCEPTHELPER_H