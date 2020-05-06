#pragma once

struct sproutInputState;

namespace Platform
{
void InitPlatformAndWindow();
void UpdateInput(sproutInputState& input);
void UpdateRender();
void Shutdown();

void SetShouldShutdown();
bool ShouldShutdown();
}