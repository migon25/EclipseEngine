#include "App.h"
#include "EditorRenderer.h"

EditorRenderer::EditorRenderer()
{
	fbo = new Framebuffer(core->window->GetWidth(), core->window->GetHeight());
}

EditorRenderer::~EditorRenderer()
{
	delete fbo;
	delete defaultShader;
	delete posShader;
	delete depthShader;
	delete normalShader;
	delete grid;
}

bool EditorRenderer::Initialize()
{
	fbo->Initialize();
	optionShader = defaultShader = new Shader("Shaders/default.vert", "Shaders/default.frag");
	posShader = new Shader("Shaders/position.vert", "Shaders/position.frag");
	depthShader = new Shader("Shaders/depth.vert", "Shaders/depth.frag");
	normalShader = new Shader("Shaders/normal.vert", "Shaders/normal.frag");

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
		glm::mat4 identity = glm::mat4(1.0f);
		for (auto& object : scene->GetObjects())
		{
			if (glfwGetKey(core->window->GetWindow(), GLFW_KEY_P) == GLFW_PRESS)
			{
				optionShader = posShader;
			}
			else if (glfwGetKey(core->window->GetWindow(), GLFW_KEY_O) == GLFW_PRESS)
			{
				optionShader = defaultShader;
			}
			else if (glfwGetKey(core->window->GetWindow(), GLFW_KEY_I) == GLFW_PRESS)
			{
				optionShader->setFloat("near", 0.1f);
				optionShader->setFloat("far", 100.f);
				optionShader = depthShader;
			}
			else if (glfwGetKey(core->window->GetWindow(), GLFW_KEY_U) == GLFW_PRESS)
			{
				optionShader = normalShader;
			}
			object->Draw(*optionShader, *editorCamera, identity);
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
