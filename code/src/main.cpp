#include "winplatform.h"

int main(void)
{
	Platform::InitPlatformAndWindow();

	bool shouldUpdate = true;
	while(shouldUpdate)
	{
		shouldUpdate = Platform::UpdateInput();
		Platform::UpdateRender();
	}
	return 0;
}