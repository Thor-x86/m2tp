/**
 * @file ffi.c
 * @brief Implementation of "include/m2tp-member-ffi.h"
 **/

#include "m2tp-member-ffi.h"
#include "m2tp-member.h"

void m2tp_setOnConnected(m2tp_OnConnected listener)
{
  m2tp_onConnected = listener;
}

void m2tp_setOnDisconnected(m2tp_OnDisconnected listener)
{
  m2tp_onDisconnected = listener;
}
