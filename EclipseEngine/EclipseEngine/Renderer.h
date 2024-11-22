#ifndef RENDERER_H
#define RENDERER_H

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "Window.h"
#include "FrameBuffer.h"

class Renderer
{
public: 
	Renderer(Window* window);
	~Renderer();

	void Initialize();
	void BeginFrame();
	void EndFrame();

	void CleanUp();

	Framebuffer* GetFramebuffer() { return fbo; }

private:
	Window* window;
	Framebuffer* fbo;
};

#endif // RENDERER_H