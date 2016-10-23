#include "App.h"
#include <GameEngine\ScreenList.h>


App::App()
{
}


App::~App()
{
}

// called on initialization
void App::OnInit()
{
		//set up the variables for the window creation
		m_gameName = "AI Game";
		m_screenWidth = 1408;
		m_screenHeight = 792;
		m_currentFlags = 0;
}
// called when exiting
void App::OnExit()
{
		//empty
}
// used to add screens
void App::AddScreens()
{
		m_gameScreen = std::make_unique<GameScreen>(&m_window);

		m_screenList->AddScreen(m_gameScreen.get());

		m_screenList->SetScreen(m_gameScreen->GetScreenIndex());
}
