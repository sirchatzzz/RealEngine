#include "SaveSystem.h"

SaveSystem::SaveSystem() : assetsData(nullptr), rootData(nullptr)
{
	LoadXML();
}

void SaveSystem::SaveVec3(const char* name, Vec3 vector)
{
	tinyxml2::XMLElement* newChildElement;
	const tinyxml2::XMLAttribute* attribute;

	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%f,%f,%f", vector.x, vector.y, vector.z);

		rootData = XML.RootElement();
	assetsData = rootData->FirstChildElement("Data");
	attribute = assetsData->FindAttribute(name);

	tinyxml2::XMLElement* childElement = assetsData->FirstChildElement(name);

	if (childElement == nullptr) {
		assetsData->InsertNewChildElement(name);
		childElement = assetsData->FirstChildElement(name);
		childElement->SetAttribute(name, buffer);
	}
	else {
		childElement->SetAttribute(name, buffer);
	}

	XML.SaveFile("XMLs/SaveFile.xml");
}

void SaveSystem::SaveVec4(const char* name, Vec4 vector)
{
	tinyxml2::XMLElement* newChildElement;
	const tinyxml2::XMLAttribute* attribute;

	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%f,%f,%f,%f", vector.x, vector.y, vector.z, vector.w);

	rootData = XML.RootElement();
	assetsData = rootData->FirstChildElement("Data");
	attribute = assetsData->FindAttribute(name);

	tinyxml2::XMLElement* childElement = assetsData->FirstChildElement(name);

	if (childElement == nullptr) {
		assetsData->InsertNewChildElement(name);
		childElement = assetsData->FirstChildElement(name);
		childElement->SetAttribute(name, buffer);
	}
	else {
		childElement->SetAttribute(name, buffer);
	}

	XML.SaveFile("XMLs/SaveFile.xml");
}

void SaveSystem::SaveQuaternion(const char* name, Quaternion quat)
{
	tinyxml2::XMLElement* newChildElement;
	const tinyxml2::XMLAttribute* attribute;

	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%f,%f,%f,%f", quat.ijk.x, quat.ijk.y, quat.ijk.z, quat.w);

	rootData = XML.RootElement();
	assetsData = rootData->FirstChildElement("Data");
	attribute = assetsData->FindAttribute(name);

	tinyxml2::XMLElement* childElement = assetsData->FirstChildElement(name);

	if (childElement == nullptr) {
		assetsData->InsertNewChildElement(name);
		childElement = assetsData->FirstChildElement(name);
		childElement->SetAttribute(name, buffer);
	}
	else {
		childElement->SetAttribute(name, buffer);
	}

	XML.SaveFile("XMLs/SaveFile.xml");
}

void SaveSystem::SaveFloat(const char* name, float flt)
{
	tinyxml2::XMLElement* newChildElement;
	const tinyxml2::XMLAttribute* attribute;

	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%f", flt);

	rootData = XML.RootElement();
	assetsData = rootData->FirstChildElement("Data");
	attribute = assetsData->FindAttribute(name);

	tinyxml2::XMLElement* childElement = assetsData->FirstChildElement(name);

	if (childElement == nullptr) {
		assetsData->InsertNewChildElement(name);
		childElement = assetsData->FirstChildElement(name);
		childElement->SetAttribute(name, buffer);
	}
	else {
		childElement->SetAttribute(name, buffer);
	}

	XML.SaveFile("XMLs/SaveFile.xml");
}

void SaveSystem::SaveSkybox(const char* name, const char* skyBox)
{
	tinyxml2::XMLElement* newChildElement;
	const tinyxml2::XMLAttribute* attribute;

	rootData = XML.RootElement();
	assetsData = rootData->FirstChildElement("Data");
	attribute = assetsData->FindAttribute(name);

	tinyxml2::XMLElement* childElement = assetsData->FirstChildElement(name);

	if (childElement == nullptr) {
		assetsData->InsertNewChildElement(name);  // Add the new child element with the specified name.
		childElement = assetsData->FirstChildElement(name);
		childElement->SetAttribute(name, skyBox);
	}
	else {
		childElement->SetAttribute(name, skyBox);
	}
	XML.SaveFile("XMLs/SaveFile.xml");
}

void SaveSystem::LoadXML()
{
	XML.LoadFile("XMLs/SaveFile.xml");
	bool status = XML.Error();
	if (status) {
		std::cout << XML.ErrorIDToName(XML.ErrorID()) << std::endl;
		return;
	}
}
