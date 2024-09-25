#pragma once

#include "Level.h"

class MainLevel final : public Level
{
public:
	MainLevel();
	~MainLevel() override;

protected:
	void BeginPlay() override;
	void EndPlay() override;

};

