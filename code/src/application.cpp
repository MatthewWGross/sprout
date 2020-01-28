#include "application.h"
#include "window.h"


namespace Engine
{
	Application::Application()
	{
		m_window = new Window();
		m_window->init();
	}
	Application::~Application()
	{
		m_window->shutdown();
		delete m_window;
	}

	void Application::run(void)
	{
		bool shouldRun = true;
		
		while (shouldRun)
		{
			shouldRun = m_window->update();
		}

	}
}