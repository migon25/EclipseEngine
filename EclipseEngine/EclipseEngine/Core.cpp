#include <GL/glew.h>
#include "Core.h"
#include "Logger.h"

Core::Core()
{
	window = new Window(1500, 844, "Eclipse Engine");
	renderer = new Renderer(window);

    Logger::Log("Initialized Engine");
}

Core::~Core()
{
	CleanUp();
}

bool Core::Initialize()
{
    window->Initialize();
	renderer->Initialize();
    return true;
}

bool Core::PreUpdate()
{
	renderer->BeginFrame();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

bool Core::Update(float dt)
{
	if (window->ShouldClose()) return false;
	return true;
}

bool Core::PostUpdate()
{
	renderer->EndFrame();
	glfwSwapBuffers(window->GetWindow());
	glfwPollEvents();
	return true;
}

void Core::CleanUp()
{
	window->CleanUp();
	renderer->CleanUp();
}