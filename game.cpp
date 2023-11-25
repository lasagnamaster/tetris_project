#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <time.h>
#include <ctime>
#include "tetromino.cpp"

using namespace sf;

float const delay_const{ 5 };

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

	Texture texture[2];
	Sprite pic[2];

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

	void initTextures() {
		texture[0].loadFromFile("images/background.png");
		texture[1].loadFromFile("images/tiles_4x4.png");
	}

	void initSprites() {
		pic[0].setTexture(texture[0]);
		pic[0].setPosition(40,40);
		pic[1].setTexture(texture[1]);
		pic[1].setPosition(400, 40);
		
	}

public:
	Game();
	virtual ~Game();

	const bool running() const { return window->isOpen(); }

	void pollEvents();
	void update();
	void render();

	Tetromino *tt;
	Tetromino* tt_next;
};

Game::Game() {
	srand(std::time(0));
	initVariables();
	initWindow();
	initTextures();
	initSprites();
	tt = new Tetromino(rand() % 8, rand() % 7);
	tt_next = new Tetromino(rand() % 8, rand() % 7, 432, 40);
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
	window->draw(pic[0]);
	window->draw(pic[1]);
	tt->draw(*window);
	tt_next->draw(*window);

	if (tt->timeToDie) { 
		delete tt;
		tt = new Tetromino(tt_next->getNum(), tt_next->getNum());
		delete tt_next;
		tt_next = new Tetromino(1, rand()%7, 432, 40);
	}

	window->display();
};

