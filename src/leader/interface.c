/**
 * @file interface.c
 * @brief Implementation of "include/m2tp-leader.h"
**/

#include "m2tp-leader.h"

#include "./NetworkState.h"

//////// Variables /////////////////////////////////////

// TODO: Write stuffs here...

////////////////////////////////////////////////////////

//////// Functions /////////////////////////////////////

void enableEchoTransmit(bool value)
{
  if (value)
    NetworkState_isEcho = 1;
  else
    NetworkState_isEcho = 0;
}

////////////////////////////////////////////////////////
