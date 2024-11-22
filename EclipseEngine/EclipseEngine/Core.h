#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <string>
#include "Window.h"
#include "Renderer.h"

class Core 
{
public:
    Core();
    ~Core();

    bool Initialize();

    bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void CleanUp();

public:
    Renderer* renderer;
    Window* window;
};

extern Core* core;

#endif // CORE_H