#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <time.h>
#include <vector>
#include "tetromino.cpp"

using namespace sf;

std::vector<Tetromino*> tetrominoes;

////////// GAME /////////////

class Game {
private:
	RenderWindow* window;
	Event ev;
	VideoMode videoMode;

	Clock clock;
	float time;
	float timer{ 0 };
	float delay{ 0.0007 };

	void initVariables() {
		window = nullptr;
		time = clock.getElapsedTime().asSeconds();
		clock.restart();
	}

	void initWindow() {
		videoMode.height = 720;
		videoMode.width = 1080;
		window = new RenderWindow(videoMode, "Tetromino");
	}
public:
	Game();
	virtual ~Game();

	const bool running() const { return window->isOpen(); }

	void pollEvents();
	void update();
	void render();

	Tetromino *tt;
	Tetromino* tt_mem;
};

Game::Game() {
	initVariables();
	initWindow();
	tt = new Tetromino(rand()%7, rand() % 7);
	Tetromino* tt_mem = tt;
	tetrominoes.push_back(tt);
};

Game::~Game() { delete tt; delete window; }

void Game::pollEvents() {
	while (window->pollEvent(ev))
	{
		switch (ev.type) {
		case Event::Closed:
			window->close();
			break;
		case Event::KeyPressed:
			if (ev.key.code == Keyboard::Escape) window->close();
			else if (ev.key.code == Keyboard::Left) { tt->move(-1); }
			else if (ev.key.code == Keyboard::Right) { tt->move(1); }
			else if (ev.key.code == Keyboard::Up) { tt->rotate(); };
			break;
		}
	}
}

void Game::update() {
	pollEvents();
	timer += time;
	timer = tt->move(timer, delay);
	
};

void Game::render() {
	window->clear(Color::Black);

	tt_mem->draw(*window);

	if (tt->timeToDie) { 
		tt = new Tetromino(rand() % 7, rand() % 7);
	}

	window->display();
};

