#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include "Debug.h"

class Window {
private:
	int width, height;
	SDL_Window* window;
	SDL_GLContext context;

public:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	Window();
	~Window();
	bool OnCreate(std::string name_, int width_, int height_);
	void OnDestroy();
	
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	SDL_Window* GetWindow() const { return window; }
	SDL_GLContext GetContext() const { return context; }

private: /// internal tools OpenGl versions. 
	void SetAttributes(int major_, int minor_);
	void GetInstalledOpenGLInfo(int *major, int *minor);
};
#endif /// !WINDOW_H