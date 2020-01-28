#ifndef ENGINE_APPLICATION_H
#define ENGINE_APPLICATION_H

namespace Engine
{
	class Window;
	class Application final
	{
	public:
		Application();
		~Application();
		void run();
	private:
		Window* m_window = nullptr;
	};
}

#endif // ENGINE_APPLICATION_H