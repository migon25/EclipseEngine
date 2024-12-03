#include "EclipseEngine/Logger.h"
#include "App.h"

Core* core = NULL;

App::App(int argc, char* args[])
{
	core = new Core();
	panelHandler = new PanelHandler(this);
	editorCamera = new Camera(core->window->GetWidth(), core->window->GetHeight(), glm::vec3(7.0f, 4.0f, -7.0f));
	editorRenderer = new EditorRenderer();

	AddModule(editorRenderer, true);
	AddModule(panelHandler, true);
}

App::~App()
{
}

bool App::Initialize()
{
	ilInit();
	if(!core->Initialize()) return false;
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if (!(*it)->Initialize()) return false;
	}
	return true;
}

bool App::Start()
{
	Initialize();
	return true;
}

bool App::Update()
{
	if (!PreUpdate()) return false;
	if (!DoUpdate()) return false;
	if (!PostUpdate()) return false;
	return true;
}

bool App::CleanUp()
{
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		(*it)->CleanUp();
	}
	core->CleanUp();
	return true;
}

void App::AddModule(Module* module, bool activate)
{
	if (activate) module->Enable();
	modules.push_back(module);
}

bool App::PreUpdate()
{
	editorCamera->UpdateMatrix(0.1f, 100.0f);
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if ((*it)->active == false) continue;
		if (!(*it)->PreUpdate()) return false;
	}
	editorRenderer->Render(core->scene, editorCamera);
	if (!core->PreUpdate()) return false;
	return true;
}

bool App::DoUpdate()
{
	if (!core->Update(dt)) return false;
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if ((*it)->active == false) continue;
		if (!(*it)->Update(dt)) return false;
	}
	return true;
}

bool App::PostUpdate()
{
	if (!core->PostUpdate()) return false;
	for (std::list<Module*>::iterator it = modules.begin(); it != modules.end(); it++)
	{
		if ((*it)->active == false) continue;
		if (!(*it)->PostUpdate()) return false;
	}

	return true;
}
