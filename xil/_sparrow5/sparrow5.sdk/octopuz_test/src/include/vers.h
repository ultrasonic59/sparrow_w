#ifndef _VERS_H_
#define _VERS_H_
#ifdef BM09
#define VERS          "rook_4e_1.0[BM09]"
#else
  #ifdef EMW3165
  #define VERS          "rook_4e_1.0[3165]"
  #else
  #define VERS          "rook_4e_1.0[1081]"
  #endif
#endif

#endif