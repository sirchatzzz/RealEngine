#pragma once
#include <glew.h>
#include <SDL_image.h>
#include <SDL.h>
#include "Actor.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"

class SkyboxActor : public Actor {
	const char* posXfilename;
	const char* negXfilename;
	const char* posYfilename;
	const char* negYfilename;
	const char* posZfilename;
	const char* negZfilename;
	GLuint textureID;
	Unq<MeshComponent> cube;
	Unq<ShaderComponent> shader;
public:
	SkyboxActor(Component* parent_, const char* posXfilename_, const char* negXfilename_, const char* posYfilename_, const char* negYfilename_, const char* posZfilename_, const char* negZfilename_);
	~SkyboxActor();

	bool OnCreate() override;
	void OnDestroy()override;
	void Update(const float deltaTime_);
	void Render() const;

	GLuint GetTextureID() const { return textureID; }
	Unq<ShaderComponent>& GetShader() { return shader; }
};

