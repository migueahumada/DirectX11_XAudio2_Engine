#pragma once
#include "BaseGame.h"

class TestGame : public BaseGame {
public:
	TestGame() = default;
	virtual ~TestGame() = default;

	bool LoadContent();
	void UnloadContent();

	//void Update(float dt) = default;
	void Render();
private:

};