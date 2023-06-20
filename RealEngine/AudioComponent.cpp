#include "AudioComponent.h"
AudioComponent::AudioComponent(Component* parent_, const char* filename_) :Component(parent_), isCreated(false), filename(nullptr){
    filename = filename_;
	soundEffect = std::make_unique<Mix_Chunk*>(nullptr);
}

AudioComponent::~AudioComponent() { 
	OnDestroy();
}

bool AudioComponent::OnCreate() {
    if (isCreated) return isCreated;

	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		printf("Error. Failed to initialize audio. \n");
		isCreated = false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("Error. Failed to open audio. \n");
		isCreated = false;
	}

    *soundEffect = Mix_LoadWAV(filename);
	if (soundEffect == nullptr) printf("Cannot open sound file \n");
    return isCreated;
}

void AudioComponent::OnDestroy() {/*making it concrete*/ }
