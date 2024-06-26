#pragma once
#include <string>
#include <unordered_map>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "tinyxml2.h"
#include "Component.h"

using namespace tinyxml2;

class AudioComponent : public Component {
	bool isCreated;
	Mix_Chunk* soundEffect;
	const char* filename;

public:
	AudioComponent(Component* parent_, const char* filename_);
	~AudioComponent();
	Mix_Chunk* GetSoundEffect() { return soundEffect; }
	bool OnCreate() override;
	void OnDestroy() override;
};

