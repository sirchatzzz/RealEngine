#include "ReadXML.h"

void ReadXML::LoadConfig() {
	XML.LoadFile("XMLs/Config.xml");
	bool status = XML.Error();
	if (status) {
		std::cout << XML.ErrorIDToName(XML.ErrorID()) << std::endl;
		return;
	}
}

void ReadXML::ReadConfig() {
	LoadConfig();

	XMLElement* rootData = XML.RootElement();

	//location
	XMLElement* boardData = rootData->FirstChildElement("CheckerBoard");
	XMLElement* position = boardData->FirstChildElement("Position");

	SetBoardPosition(MATH::Vec3(position->FloatAttribute("x"),
								position->FloatAttribute("y"),
								position->FloatAttribute("z")));

	//orientation
	XMLElement* orientation = boardData->FirstChildElement("Orientation");

	SetBoardOrientation(MATH::Quaternion(orientation->FloatAttribute("w"),
							  MATH::Vec3(orientation->FloatAttribute("yaw"),
										 orientation->FloatAttribute("pitch"),
										 orientation->FloatAttribute("roll"))));

	//scale
	XMLElement* scale = boardData->FirstChildElement("Scale");

	SetBoardScale(MATH::Vec3(scale->FloatAttribute("x"),
							 scale->FloatAttribute("y"),
							 scale->FloatAttribute("z")));
}
