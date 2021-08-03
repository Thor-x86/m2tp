/**
 * @file cpp_compat.h
 * @brief Provides syntactic sugars for C++ compatibility
**/

#ifndef IS_M2TP_CPP_COMPAT_DEFINED
#define IS_M2TP_CPP_COMPAT_DEFINED

#ifdef __cplusplus

#define EXTERN_C_START \
  extern "C"           \
  {

#define EXTERN_C_STOP }

#else // #ifdef __cplusplus

#define EXTERN_C_START

#define EXTERN_C_STOP

#endif // #ifdef __cplusplus

#endif // #ifndef IS_M2TP_CPP_COMPAT_DEFINED
