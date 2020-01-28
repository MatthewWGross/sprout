#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

namespace Engine
{
	class Window
	{
	public:
		Window();
		void init();
		void shutdown();
		bool update();
		virtual ~Window();
	private:
	};
}




#endif //ENGINE_WINDOW_H
