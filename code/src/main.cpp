#include "winplatform.h"
#include "input.h"

int main(void)
{
	sproutInputState inputState;
	inputState.clear();

	Platform::InitPlatformAndWindow();

	while(Platform::ShouldShutdown() == false)
	{
		Platform::UpdateInput(inputState);
		Platform::UpdateRender();

		if(inputState.isKeyTriggered(KEY_ESCAPE))
			Platform::SetShouldShutdown();
	}
	return 0;
}