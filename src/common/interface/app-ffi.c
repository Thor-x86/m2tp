/**
 * @file app-ffi.c
 * @brief Implementation of "include/m2tp/interface/app-ffi.h"
 **/

#include "m2tp/interface/app-ffi.h"
#include "m2tp/interface/app.h"

void m2tp_setReceivedListener(m2tp_ReceivedListener listener)
{
  m2tp_receivedListener = listener;
}

void m2tp_setNewMemberListener(m2tp_NewMemberListener listener)
{
  m2tp_onNewMemberListener = listener;
}

void m2tp_setAnotherMemberQuitListener(m2tp_AnotherMemberQuitListener listener)
{
  m2tp_onAnotherMemberQuitListener = listener;
}
