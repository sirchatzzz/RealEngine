#include "SkyboxActor.h"

SkyboxActor::SkyboxActor(Component* parent_, const char* posXfilename_, const char* negXfilename_, const char* posYfilename_, const char* negYfilename_, const char* posZfilename_, const char* negZfilename_):
Actor(parent_), textureID(0), cube(nullptr), shader(nullptr), posXfilename(nullptr), negXfilename(nullptr), posYfilename(nullptr), negYfilename(nullptr), posZfilename(nullptr), negZfilename(nullptr){
	posXfilename = posXfilename_;
	negXfilename = negXfilename_;
	posYfilename = posYfilename_;
	negYfilename = negYfilename_;
	posZfilename = posZfilename_;
	negZfilename = negZfilename_;
}

SkyboxActor::~SkyboxActor() {
	OnDestroy();
}

bool SkyboxActor::OnCreate() {
	cube = std::make_shared<MeshComponent>(nullptr, "meshes/Cube.obj");
	bool status = cube->OnCreate();

	shader = std::make_shared<ShaderComponent>(nullptr, "shaders/skyboxVert.glsl", "shaders/skyboxFrag.glsl");
	status = shader->OnCreate();

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	SDL_Surface* textureSurface;
	int mode;

	/// /////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posXfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	/// /////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negXfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	/// /////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posYfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	/// /////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negYfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	/// /////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(posZfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);

	/// /////////////////////////////////////////////////////////////////
	textureSurface = IMG_Load(negZfilename);
	if (textureSurface == nullptr) {
		return false;
	}
	mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
	SDL_FreeSurface(textureSurface);


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return true;
}

void SkyboxActor::OnDestroy() {
	glDeleteTextures(1, &textureID);
}

void SkyboxActor::Update(const float deltaTime_) { /*making it concrete*/ }

void SkyboxActor::Render() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	cube->Render(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
