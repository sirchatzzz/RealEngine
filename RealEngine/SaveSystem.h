#pragma once

#include <vector>
#include "Vector.h"
#include "Quaternion.h"
#include "tinyxml2.h"

using namespace MATH;

class SaveSystem
{
private:

	tinyxml2::XMLDocument XML;
	tinyxml2::XMLElement* rootData;
	tinyxml2::XMLElement* assetsData;

public:

	SaveSystem();
	void SaveVec3(const char* name, Vec3 vector);
	void SaveVec4(const char* name, Vec4 vector);
	void SaveFloat(const char* name, float flt);
	void SaveSkybox(const char* name, const char* skyBox);
	void SaveInt(const char* name, int nmbr);
	void SaveChar(const char* name, const char* toSave);
	void DeleteMesh(const char* name);
	void LoadXML();

};

