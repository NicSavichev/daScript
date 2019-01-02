#pragma once

#include "vectypes.h"
#include "arraytype.h"
#include "rangetype.h"

namespace yzg
{
    using namespace std;
    
    union ValueVariant {
        __m128      dataVec;
        bool        dataBool;
        int32_t     dataInt;
        uint32_t    dataUInt;
        int64_t     dataInt64;
        uint64_t    dataUInt64;
        char *      dataPtr;
    };
    
    template <typename TT>
    struct cast;
    
    template <typename TT>
    struct cast <TT *> {
        static __forceinline TT * to ( __m128 a )               { return ((TT **)&a)[0]; }
        static __forceinline __m128 from ( TT * p )             { __m128 x; ((TT **)&x)[0] = p; return x; }
    };
    
    template <typename TT>
    struct cast <TT &> {
        static __forceinline TT & to ( __m128 a )               { return *((TT **)&a)[0]; }
        static __forceinline __m128 from ( TT & p )             { __m128 x; ((TT **)&x)[0] = &p; return x; }
    };
    
    template <>
    struct cast <string> {
        static __forceinline string to ( __m128 x )             { return cast<char *>::to(x); }
        static __forceinline __m128 from ( const string & x )   { return cast<char *>::from((char *)x.c_str()); }
    };
    template<> struct cast <string &> : cast<string> {};
    template<> struct cast <const string &> : cast<string> {};
    
    template <>
    struct cast <bool> {
        static __forceinline bool to ( __m128 x )               { return *((int32_t *)&x) != 0; }
        static __forceinline __m128 from ( bool x )             { __m128 a; *((int32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <int32_t> {
        static __forceinline int32_t to ( __m128 x )            { return *((int32_t *)&x); }
        static __forceinline __m128 from ( int32_t x )          { __m128 a; *((int32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <uint32_t> {
        static __forceinline uint32_t to ( __m128 x )           { return *((uint32_t *)&x); }
        static __forceinline __m128 from ( uint32_t x )         { __m128 a; *((uint32_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <int64_t> {
        static __forceinline int64_t to ( __m128 x )            { return *((int64_t *)&x); }
        static __forceinline __m128 from ( int64_t x )          { __m128 a; *((int64_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <uint64_t> {
        static __forceinline uint64_t to ( __m128 x )           { return *((uint64_t *)&x); }
        static __forceinline __m128 from ( uint64_t x )         { __m128 a; *((uint64_t *)&a) = x; return a; }
    };
    
    template <>
    struct cast <float> {
        static __forceinline float to ( __m128 x )              { return *((float *)&x); }
        static __forceinline __m128 from ( float x )            { __m128 a; *((float *)&a) = x; return a; }
    };
    
    template <>
    struct cast <range> {
        static __forceinline range to ( __m128 x )              { return *((range *)&x); }
        static __forceinline __m128 from ( range x )            { __m128 a; *((range *)&a) = x; return a; }
    };
    
    template <>
    struct cast <urange> {
        static __forceinline urange to ( __m128 x )             { return *((urange *)&x); }
        static __forceinline __m128 from ( urange x )           { __m128 a; *((urange *)&a) = x; return a; }
    };
    
    template <>
    struct cast <Block> {
        static __forceinline Block to ( __m128 x )             { return *((Block *)&x); }
        static __forceinline __m128 from ( Block x )           { __m128 a; *((Block *)&a) = x; return a; }
    };
    
    template <typename TT>
    struct cast_fVec {
        static __forceinline TT to ( __m128 x )                 { return *((TT *)&x); }
        static __forceinline __m128 from ( const TT & x )       { return _mm_loadu_ps(&x.x); }
    };
    
    template <> struct cast <float2>  : cast_fVec<float2> {};
    template <> struct cast <float3>  : cast_fVec<float3> {};
    template <> struct cast <float4>  : cast_fVec<float4> {};
    
    template <typename TT>
    struct cast_iVec {
        static __forceinline TT to ( __m128 x )                 { return *((TT *)&x); }
        static __forceinline __m128 from ( const TT & x )       { return  _mm_castsi128_ps(_mm_loadu_si128((__m128i*)&x.x)); }
    };
    
    template <> struct cast <int2>  : cast_iVec<int2> {};
    template <> struct cast <int3>  : cast_iVec<int3> {};
    template <> struct cast <int4>  : cast_iVec<int4> {};
    
    template <> struct cast <uint2>  : cast_iVec<uint2> {};
    template <> struct cast <uint3>  : cast_iVec<uint3> {};
    template <> struct cast <uint4>  : cast_iVec<uint4> {};
}
