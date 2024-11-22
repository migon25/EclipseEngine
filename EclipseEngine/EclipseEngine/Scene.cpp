#include "Scene.h"

Scene::Scene(Camera& camera) : m_camera(camera)
{
}

Scene::~Scene()
{

}

bool Scene::PreUpdate()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->PreUpdate(); //there is nothing in her for now
	}
    return true;
}

bool Scene::Update()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->Update(); //there is nothing in her for now
	}
    return true;
}

bool Scene::PostUpdate()
{
	for (auto& gameObject : m_gameObjects)
	{
		gameObject->PostUpdate(); //there is nothing in her for now
	}
	return true;
}

void Scene::CleanUp()
{
	m_gameObjects.clear();
}

void Scene::Draw()
{

}

std::shared_ptr<GameObject> Scene::CreateGameObject(std::string name)
{
	auto cube = std::make_shared<GameObject>();

	Shader shader("Shaders/default.vert", "Shaders/default.frag");
	std::vector<Texture> catTexture
	{
		Texture("Assets/Baker_house.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
	};
	cube->AddComponent<Mesh>(name);
	cube->AddComponent<Material>(shader, catTexture);

	m_gameObjects.push_back(cube);
	return cube;
}

//std::shared_ptr<GameObject> Scene::CreateCube()
//{
//	std::vector<Vertex> vertices =
//	{ //               COORDINATES           /            NORMALS          /          COLOR           /      TEXTURE COORDINATES    //
//		Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},  // 0
//		Vertex{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},  // 1
//		Vertex{glm::vec3(1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	 // 2
//		Vertex{glm::vec3(-1.0f,  1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	 // 3
//		Vertex{glm::vec3(1.0f, -1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},	 // 4
//		Vertex{glm::vec3(1.0f,  1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},	 // 5
//		Vertex{glm::vec3(-1.0f,  1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},	 // 6
//		Vertex{glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)}	 // 7
//	};
//	std::vector<GLuint> indices =
//	{
//		0, 1, 2,
//		0, 2, 3,
//		1, 4, 5,
//		1, 5, 2,
//		4, 7, 6,
//		4, 6, 5,
//		7, 0, 3,
//		7, 3, 6,
//		0, 1, 4,
//		0, 4, 7,
//		3, 2, 5,
//		3, 5, 6
//	};
//	Shader shader("Shaders/default.vert", "Shaders/default.frag");
//	std::vector<Texture> catTexture
//	{
//		Texture("Assets/checkerboard.png","diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE)
//	};
//	std::shared_ptr<GameObject> cube = CreateGameObject("Cube");
//	cube->AddComponent<Mesh>(vertices, indices, catTexture);
//	cube->AddComponent<Material>(shader, catTexture);
//	return cube;
//}

Camera Scene::AddCamera(float aspectRatio, float nearPlane, float farPlane, ProjectionType projType, std::string name)
{
	auto camera = Camera(aspectRatio, nearPlane, farPlane, projType, name);
	return camera;
}