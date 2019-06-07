#include <engine/engine.h>
#include <city_tilemap.h>

class CityBuilderEngine : public neko::MainEngine
{
public:

	void Init() override
	{
		MainEngine::Init();
		environmentTilemap.Init(textureManager);
	}

	void Update() override
	{
		MainEngine::Update();
		environmentTilemap.PushCommand(graphicsManager);
	}

	void Destroy() override
	{
		MainEngine::Destroy();
	}

private:
	neko::EntityManager entityManager;
	neko::TextureManager textureManager;
	neko::CityBuilderTilemap environmentTilemap;

};


int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	CityBuilderEngine engine;
	engine.Init();
	engine.EngineLoop();
	return EXIT_SUCCESS;
}