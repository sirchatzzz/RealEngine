#pragma once
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include <Vector.h>
#include <VMath.h>
#include <Quaternion.h>

using namespace tinyxml2;

class ReadXML {
	XMLDocument XML;

	MATH::Vec3 boardPosition;
	MATH::Quaternion boardOrientation;
	MATH::Vec3 boardScale;

public:
	void ReadConfig();
	void LoadConfig();

	MATH::Vec3 GetBoardPosition() { return boardPosition; }
	MATH::Quaternion GetBoardOrientation() { return boardOrientation; }
	MATH::Vec3 GetBoardScale() { return boardScale; }

	void SetBoardPosition(MATH::Vec3 position_) { boardPosition = position_; }
	void SetBoardOrientation(MATH::Quaternion orientation_) { boardOrientation = orientation_; }
	void SetBoardScale(MATH::Vec3 scale_) { boardScale = scale_; }
};

