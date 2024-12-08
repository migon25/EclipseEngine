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
	outliningShader = new Shader("Shaders/outline.vert", "Shaders/outline.frag");
	glEnable(GL_CULL_FACE); // Backface culling testing
	glEnable(GL_STENCIL_TEST); // Stencil testing
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void EditorRenderer::Render(Scene* scene, Camera* editorCamera, std::shared_ptr<GameObject> selectedObject)
{
	if (scene)
	{
		// Normal object rendering
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Write 1 to the stencil buffer for all objects
		glStencilMask(0xFF); // Enable writing to the stencil buffer

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
				optionShader->SetFloat("near", 0.1f);
				optionShader->SetFloat("far", 100.f);
				optionShader = depthShader;
			}
			else if (glfwGetKey(core->window->GetWindow(), GLFW_KEY_U) == GLFW_PRESS)
			{
				optionShader = normalShader;
			}
			object->Draw(*optionShader, *editorCamera, identity);
		}

		if (selectedObject != nullptr)
		{
			// Outlining: Second pass (draw outline)
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Only draw where stencil buffer is not 1
			glStencilMask(0x00); // Disable writing to the stencil buffer
			glDisable(GL_DEPTH_TEST); // Disable depth testing for stencil write

			selectedObject->Draw(*outliningShader, *editorCamera, identity); // Draw outline
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Reset to fill mode
		}

		glStencilMask(0xFF); // Re-enable writing to stencil buffer for next frame
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Reset stencil function
		glEnable(GL_DEPTH_TEST); // Re-enable depth testing

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
