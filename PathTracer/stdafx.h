// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define _USE_MATH_DEFINES 
#include <cmath>

// Custom defines
#define MIN_T (0.0001f)
#define RAND_FLOAT ((float)rand()/(float)((unsigned int)RAND_MAX+1))
