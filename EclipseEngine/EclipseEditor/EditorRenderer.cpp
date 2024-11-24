#include "App.h"
#include "EditorRenderer.h"

EditorRenderer::EditorRenderer()
{
	fbo = new Framebuffer(core->window->GetWidth(), core->window->GetHeight());
}

EditorRenderer::~EditorRenderer()
{
}

bool EditorRenderer::Initialize()
{
	fbo->Initialize();
	grid = new Grid();
	return true;
}

bool EditorRenderer::PreUpdate()
{
	BeginFrame();
	return true;
}

void EditorRenderer::BeginFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->GetFBO());
	glViewport(0, 0, fbo->GetWidth(), fbo->GetHeight());
	glClearColor(0.2f, 0.2f, 0.2f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EditorRenderer::Render(Scene* scene, Camera* editorCamera)
{
	if (scene)
	{
		for (auto& object : scene->GetObjects())
		{
			object->Draw(*scene->defaultShader, *editorCamera);
		}
	}
	RenderGrid(grid, editorCamera);
	RenderGuizmo();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool EditorRenderer::CleanUp()
{
	return true;
}

void EditorRenderer::RenderGrid(Grid* grid, Camera* editorCamera)
{
	grid->Draw(editorCamera);
}

void EditorRenderer::RenderGuizmo()
{
}
