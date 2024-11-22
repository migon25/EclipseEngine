#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <list>
#include "GameObject.h"
#include "Camera.h"

class Scene
{
public:
	Scene();
	~Scene();

	std::list<std::unique_ptr<GameObject >> gameObjects;
	std::unique_ptr<Camera> camera;
};

#endif // SCENE_H