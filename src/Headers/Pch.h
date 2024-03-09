#pragma once
/* Start Header ************************************************************************/
/*!
\file pch.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date Jan 18, 2024
\brief  This is a precompiled header file.
        Files listed below are compiled only once, improving build performance for future builds.
        This also affects IntelliSense performance, including code completion and many code browsing features.
        However, files listed here are ALL re-compiled if any one of them is updated between builds.
        Do not add files here that you will be updating frequently as this negates the performance advantage.
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#ifndef PCH_H
#define PCH_H

#define _CRTDBG_MAP_ALLOC

// ----------------------------------------------------------------------------
// Add headers that you want to pre-compile here
// ----------------------------------------------------------------------------
#include "AEEngine.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "utils.h"
#include "db.h"

using namespace std;  // we shouldnt do this, crowds namespace

#endif //PCH_H