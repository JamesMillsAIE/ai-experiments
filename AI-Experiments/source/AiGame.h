#pragma once

#include <Core/Game.h>

class Font;

class AiGame final : public Game
{
public:
	AiGame();
	~AiGame() override;

public:
	bool Startup() override;
	void Shutdown() override;
	void Tick(float deltaTime) override;
	void Render() override;

private:
	Font* m_font;

};