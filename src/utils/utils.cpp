/* Start Header ************************************************************************/
/*!
\file utils.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief common utility functions
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "utils.h"

int mouseX{}, mouseY{}, wMouseX{}, wMouseY{};
AEVec2 camOffset{};
DIFFICULTY_SETTINGS difficulty{};

/*utility functions*/

/* screen coordinates to world coordinates */
AEVec2 stow(float x, float y) {
	float wX = x - (AEGfxGetWindowWidth() / 2);
	float wY = (AEGfxGetWindowHeight() / 2) - y;  // Corrected this line
	return AEVec2{ wX, wY };
}

AEVec2 ston(float x, float y) {
	float nX = (2.0f * x / AEGfxGetWindowWidth()) - 1.0f;
	float nY = 1.0f - (2.0f * y / AEGfxGetWindowHeight());

	return AEVec2{ nX, nY };
}

AEVec2 wtos(float x, float y) {
	x = x + AEGfxGetWindowWidth() / 2.f;
	y = -y + AEGfxGetWindowHeight() / 2.f;

	return AEVec2{ x, y };
}


float precisionRound(float x, int precision) {
	int mult = static_cast<int>(pow(10, precision));
	int num = static_cast<int>(x * mult + 0.5f);
	return num / static_cast<f32>(mult);
}

float degToRad(float degrees) {
	return degrees * Math::m_PI / 180.f;
}

float lerp(float start, float end, float t) {
	return start + t * (end - start);
}


std::vector<std::string> split(const std::string& str, const char sep) {
	std::vector<std::string> out;
	std::string curr = "";

	for (const char c : str) {
		if (c == sep && curr.length() > 0) {
			out.push_back(curr);
			curr.clear();
			continue;
		}

		if (c != sep) {
			curr += c;
		}
	}
	if (curr.length() > 0) {
		out.push_back(curr);
	}

	return out;
}

void initGlobals() {
	updateGlobals();
	difficulty = static_cast<DIFFICULTY_SETTINGS>(Database::getInstance().data["game"]["difficulty"]);
}

void updateGlobals() {
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 wMouse = stow(static_cast<float>(mouseX), static_cast<float>(mouseY));
	wMouseX = static_cast<int>(wMouse.x);
	wMouseY = static_cast<int>(wMouse.y);
	AEGfxGetCamPosition(&camOffset.x, &camOffset.y);
}

std::string lower(const std::string& text) {
	constexpr char offset = 'a' - 'A';
	std::string out;
	out.reserve(text.size());
	for (const char c : text) {
		char u = c;
		if (c >= 'A' && c <= 'Z') {
			u += offset;
		}
		out += u;
	}
	return out;
}

std::string upper(const std::string& text) {
	constexpr char offset = 'a' - 'A';
	std::string out;
	out.reserve(text.size());
	for (const char c : text) {
		char u = c;
		if (c >= 'a' && c <= 'z') {
			u -= offset;
		}
		out += u;
	}
	return out;
}

namespace {
	namespace fs = std::filesystem;
	std::vector<std::string> getAllFiles(const std::string& path, const std::vector<std::string>& exclusions) {
		std::vector<std::string> files;
		files.reserve(100);         // for better performance, no need to keep reallocating

		for (const auto& dir : fs::directory_iterator(path)) {
			// stripped file or dir name
			const std::string filename = dir.path().filename().string();

			// recurse if is directory
			if (fs::is_directory(dir)) {
				// ensure directory is not in exclusion list
				if (std::find_if(
					exclusions.begin(),
					exclusions.end(),
					[filename](const std::string& e) {
						return upper(filename) == upper(e);
					})
					!=
						exclusions.end()) {
					continue;
				}

				// add recursed files to current files
				std::vector<std::string> recursedFiles;
				recursedFiles.reserve(20);    // just for better performance lol, no need to keep reallocating
				recursedFiles = getAllFiles(dir.path().string(), exclusions);

				// recursed file names
				for (std::string& rfn : recursedFiles) {
					// prepend relative path to filenames
					std::stringstream ss;
					ss << filename << "/" << rfn;
					rfn = ss.str();
				}

				// insert nested files to current filelist
				files.insert(files.end(), recursedFiles.begin(), recursedFiles.end());
			}
			else if (fs::is_regular_file(dir)) {
				// add file to files vector if is file
				files.push_back(filename);
			}
		}

		return files;
	}

}

void loadAllTextures() {
	std::vector<std::string> assets = getAllFiles("./Assets/", { "audio", "fonts"});
	// asset path
	for (const std::string& ap : assets) {
		RenderHelper::getInstance()->registerTexture("./Assets/" + ap, "./Assets/" + ap);
	}
}


