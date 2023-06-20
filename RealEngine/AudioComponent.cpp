#include "AudioComponent.h"
AudioComponent::AudioComponent(Component* parent_, const char* filename_) :Component(parent_), isCreated(false), filename(nullptr){
    filename = filename_;
}

AudioComponent::~AudioComponent() { 
	OnDestroy();
}

bool AudioComponent::OnCreate() {
    if (isCreated) return isCreated;
    if (SDL_Init(SDL_INIT_AUDIO)< 0) {
        printf("Error. Failed to initialize audio. \n");
        isCreated = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error. Failed to open audio. \n");
        isCreated = false;
    }

    else {
        isCreated = true;
    }
    Mix_LoadWAV(filename);
    return isCreated;
}

void AudioComponent::OnDestroy() {/*making it concrete*/ }
