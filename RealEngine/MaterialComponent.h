#pragma once
#include "Component.h"
#include <SDL_image.h>
class MaterialComponent : public Component {
private:
	const char* filename;
	unsigned int textureID;
public:
	MaterialComponent(Component* parent_, const char* filename_);
	~MaterialComponent();
	unsigned int getTextureID() { return textureID; }
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
};

