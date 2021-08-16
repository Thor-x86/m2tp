/**
 * @file typedef.h
 * @brief Type definition should be here
**/

#ifndef IS_M2TP_POSIX_TYPEDEF_DEFINED
#define IS_M2TP_POSIX_TYPEDEF_DEFINED

// Type definition for connectMode variable
typedef enum ConnectMode
{
  MODE_OFFLINE,
  MODE_FILE,
  MODE_SOCKET
} ConnectMode;

#endif // #ifndef IS_M2TP_POSIX_TYPEDEF_DEFINED
