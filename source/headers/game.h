#pragma once

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw();
	void update(float elapsedTime);
};