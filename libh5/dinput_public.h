#pragma once

#include "commons.h"

#define DIRECTINPUT_VERSION 0x0800

#define DINPUT_SAFE_RELEASE(ptr) if (ptr) { ptr->Unacquire(); ptr->Release(); ptr = nullptr; }
