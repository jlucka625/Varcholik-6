#include "pch.h"

#define OPEN_GL 1

#include "ActionSpriteDraw.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Sprite.h"
#include "EntityInputHandler.h"
#include "EntityText.h"
#include "PostProcessor.h"
#include <memory>

using namespace LibraryDesktop;
using namespace GameDesktopShared;

//Coordinates for the orthographic projection frustum
const float ORTHO_TOP = 600.0f;
const float ORTHO_BOTTOM = 0.0f;
const float ORTHO_LEFT = 0.0f;
const float ORTHO_RIGHT = 800.0f;
const glm::mat4 projectionMatrix = glm::ortho(ORTHO_LEFT, ORTHO_RIGHT, ORTHO_TOP, ORTHO_BOTTOM, -1.0f, 1.0f);

GameClock timeClock;
GameTime gameTime;

//The global processor reference to use
std::shared_ptr<PostProcessor> globalProcessor;
EntityInputHandler* globalInputHandler;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(scancode);
	UNREFERENCED_PARAMETER(mode);

	if (globalInputHandler != nullptr)
	{
		globalInputHandler->HandleKey(key, action);
	}	
}

void onResize(GLFWwindow* window, int width, int height)
{
	UNREFERENCED_PARAMETER(window);
	glViewport(0, 0, width, height);	
	
	globalProcessor->BindFramebuffer(width, height);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(previousInstance);
	UNREFERENCED_PARAMETER(commandLine);
	UNREFERENCED_PARAMETER(showCommand);

	ReactionAttributedFactory raf;
	ActionMovePlayerFactory ampf;
	ActionFlipPlayerFactory afpf;

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int returnVal = 1;

	const glm::vec4 CornflowerBlue = glm::vec4(SCREEN_COLOR.red, SCREEN_COLOR.green, SCREEN_COLOR.blue, SCREEN_COLOR.alpha);
	
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT, APP_NAME.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, onResize);
	glfwSetKeyCallback(window, key_callback);

	if (gl3wInit() != 0)
	{
		return -1;
	}

	glViewport(0, 0, WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT);
	
	//Done for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Load shaders here
	ResourceManager::LoadShader("Content/SpriteShader.vs", "Content/SpriteShader.ps", "SpriteShader");
	ResourceManager::LoadShader("Content/TextShader.vs", "Content/TextShader.ps", "TextShader");
	ResourceManager::LoadShader("Content/PostProcess.vs", "Content/PostProcess.ps", "PostProcess");

	ResourceManager::GetShader("SpriteShader").Use().SetInteger("image", 0);
	ResourceManager::GetShader("SpriteShader").SetMatrix("projection", projectionMatrix);

	ResourceManager::GetShader("TextShader").Use().SetInteger("text", 0);
	ResourceManager::GetShader("TextShader").SetMatrix("projection", projectionMatrix);

	ResourceManager::GetShader("PostProcess").SetMatrix("projection", projectionMatrix);

	ResourceManager::LoadTexture("Content/characters.png", GL_TRUE, "characters");
	ResourceManager::LoadTexture("Content/Tiles.png", GL_TRUE, "Tiles");
	ResourceManager::LoadTexture("Content/title.png", GL_TRUE, "Title");

	SpriteRenderer spriteRenderer(ResourceManager::GetShader("SpriteShader"));

	Texture2D sceneTexture;
	PostProcessor postProcessor(ResourceManager::GetShader("PostProcess"), WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT);
	globalProcessor = std::make_shared<PostProcessor>(postProcessor);
	globalProcessor->SetIsWaveUsed(false);

	EntityInputHandler inputHandler;
	inputHandler.GenerateHandlers();

	//Test making a text entity

	//ActionSpriteDrawFactory drawFactory;
	//ActionSpriteUpdateFactory updateFactory;
	
	EntityText textEntity;
	textEntity.SetName("Text");
	textEntity.SetText("Entity - THE POTATO");
	textEntity.SetColor(1.0f, 0.0f, 0.0f);
	textEntity.SetSizePositionInfo(0.0f, 200.0f, 2.0f, 30.0f);
	textEntity.Load();
	textEntity.CreateAction("DrawAction", "ActionSpriteDraw", "Actions");


	globalInputHandler = &inputHandler;

	Sprite characterSprite;
	characterSprite.SetTexture(ResourceManager::GetTexture("characters"));
	characterSprite.SetSize(glm::vec2(32.0f, 64.0f));
	characterSprite.SetPosition(glm::vec2(200.0f, 200.0f));
	//characterSprite.SetTranslationSpeed(glm::vec2(1.0f, 0.0f));
	characterSprite.SetAnimationTime(0.07f);
	//characterSprite.SetRotationSpeed(50.0f);

	ResourceManager::GetTexture("characters").SetTotalXFrames(8.0f);
	ResourceManager::GetTexture("characters").SetTotalYFrames(2.0f);
	ResourceManager::GetTexture("Tiles").SetTotalXFrames(8.0f);
	ResourceManager::GetTexture("Tiles").SetTotalYFrames(4.0f);
	ResourceManager::GetTexture("Title").SetTotalXFrames(1.0f);
	ResourceManager::GetTexture("Title").SetTotalYFrames(1.0f);

	World world(b2Vec2(0.0f, 9.8f));

	LevelParser lp;
	Sector* level = &lp.LoadLevel();

	world.Adopt(*level, "Sectors");
	auto currentTime = gameTime.CurrentTime();
	world.GetWorldState().SetGameTime(gameTime);
	inputHandler.SetWorldState(world.GetWorldState());

	//Load sounds
	SoundManager* Sound = SoundManager::CreateInstance();
	Sound->Play("BGM", "PushingOnwards", 3);
	Sound->SetVolume("BGM", .2f);
	Sound->Loop("BGM", -1);
	
	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, &CornflowerBlue[0]);
		globalProcessor->BeginRender();

		timeClock.UpdateGameTime(gameTime);
		float deltaSeconds = static_cast<float>(gameTime.ElapsedGameTime().count()) / 1000.0f;
		world.GetWorldState().deltaTime = deltaSeconds;

		ContactListener contactListener;
		world.GetB2DWorld()->SetContactListener(&contactListener);

		if (deltaSeconds <= 0.1f)
		{
			world.Update(timeClock);
		}

		currentTime = gameTime.CurrentTime();

		globalProcessor->EndRender();
		globalProcessor->Render(static_cast<GLfloat>(glfwGetTime()));

		glfwSwapBuffers(window);
		glfwPollEvents();
		Sound->Update();

		if (world.GetWorldState().GetUnloadSector() || world.GetWorldState().GetReloadSector())
		{
			level->Orphan();
			Event<EventMessageAttributed>::UnsubscribeAll();
			world.GetEventQueue().Clear();
			delete level;

			if(world.GetWorldState().GetUnloadSector())
				lp.NextLevel();

			level = &lp.LoadLevel();
			world.Adopt(*level, "Sectors");
			world.GetWorldState().SetUnloadSector(false);
			world.GetWorldState().SetReloadSector(false);
		}
	}

	//delete &level;

	//delete &level;
	delete Sound;
	glfwDestroyWindow(window);
	glfwTerminate();

	return returnVal;
}
