#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <time.h>
#include <ctime>
#include "tetromino.cpp"

using namespace sf;

float const delay_const{ 0.001 };

////////// GAME /////////////

class Game {
private:
	RenderWindow* window;
	Event ev;
	VideoMode videoMode;

	Clock clock;
	float time;
	float timer{ 0 };
	float delay = delay_const;

	void initVariables() {
		window = nullptr;
		time = clock.getElapsedTime().asSeconds();
		clock.restart();
	}

	void initWindow() {
		videoMode.height = 720;
		videoMode.width = 1080;
		window = new RenderWindow(videoMode, "Tetromino");
		window->setVerticalSyncEnabled(true);
	}
public:
	Game();
	virtual ~Game();

	const bool running() const { return window->isOpen(); }

	void pollEvents();
	void update();
	void render();

	Tetromino *tt;
};

Game::Game() {
	srand(std::time(0));
	initVariables();
	initWindow();
	tt = new Tetromino(rand() % 8, rand() % 7);
};

Game::~Game() { delete tt; delete window; }

bool down_pressed = false;

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
			else if (ev.key.code == Keyboard::Up) { tt->rotate(); }
			else if (ev.key.code == Keyboard::Down) { down_pressed = true; delay = delay_const / 3; }
			break;
		case Event::KeyReleased:
			if (ev.key.code == Keyboard::Down and down_pressed) { delay = delay_const; }
			break;
		}
	}
}

void Game::update() {
	pollEvents();
	timer += time;

	if (timer > delay) 
		timer = tt->move(timer, delay);
	
};

void Game::render() {
	window->clear(Color::Black);

	tt->draw(*window);

	if (tt->timeToDie) { 
		delete tt;
		tt = new Tetromino(rand()%8, rand() % 7);
	}

	window->display();
};

