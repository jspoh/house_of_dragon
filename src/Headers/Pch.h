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
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <cctype>
#include <sstream>
#include <time.h>
#include <list>
#include <sstream>
#include <memory>
#include "AEEngine.h"
#include "json.hpp"

#include "utils.h"
#include "db.h"
#include "Event.h"
#include "CollisionChecker.h"
#include "Vector3.h"
#include "SingletonTemplate.h"
#include "MyMath.h"
#include "SoundManager.h"
#include "SoundPlayer.h"
#include "Pause.h"

extern bool gGameRunning;
extern bool DEBUG;

class NullStream {
public:
    NullStream() = default;
    ~NullStream() = default;

    template <typename T>
    NullStream& operator<<(T _) {
        return *this;
    }
};
extern NullStream nullstream;

#ifndef NDEBUG
#define cout std::cout
#define endl std::endl
#else
#define cout nullstream
#endif

#endif //PCH_H