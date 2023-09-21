#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map> 
#include "Component.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "MaterialComponent.h"
#include "AudioComponent.h"
#include "SkyboxActor.h"
#include "CameraActor.h"
#include "LightActor.h"
#include "Debug.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class AssetManager {
private:
	bool isCreated;
	std::unordered_map<std::string, Ref<Component> > assetsCatalog;
	XMLDocument XML;
	XMLElement* rootData;
	XMLElement* assetsData;

public:
	AssetManager();
	~AssetManager();
	bool OnCreate();
	void OnDestroy();

	void RemoveAllComponents();
	void ListAllComponents() const;

	void LoadXML();
	void ReadXML();

	template<typename ComponentTemplate, typename ... Args>
	void AddComponent(std::string name, Args&& ... args_) {
		Ref<ComponentTemplate> t = std::make_shared<ComponentTemplate>(std::forward<Args>(args_)...);
		assetsCatalog[name] = t;
	}

	template<typename ComponentTemplate, typename ... Args>
	Ref<ComponentTemplate> GetComponent(std::string name, Args&& ... args_) const {
		auto id = assetsCatalog.find(name);
#ifdef _DEBUG
		if (id == assetsCatalog.end()) {
			Debug::Error("Can't fint requested component", __FILE__, __LINE__);
			return Ref<ComponentTemplate>(nullptr);
		}
#endif 
		return std::dynamic_pointer_cast<ComponentTemplate>(id->second);
	}

};
