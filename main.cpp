#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>

// some ideas on camera shake (TODO: implement when ball hit pad
// https://gamedev.stackexchange.com/questions/1828/realistic-camera-screen-shake-from-explosion/47565#47565
// https://youtu.be/tu-Qe66AvtY?si=mrsSRHY_6W3Lhkac
// https://youtu.be/Fy0aCDmgnxg?si=7ZtQ-texrgYVUDAQ
int main()
{
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Yet Another Pong Clone", Style::Default);
	window.setFramerateLimit(60);
	int score = 0;
	int lives = 3;

	Bat bat(1920 / 2, 1080 - 20);
	Ball ball(1920 / 2, 0);
	Text hud;
	Font font;
	font.loadFromFile("fonts/DS-DIGIT.TTF");
	hud.setFont(font);
	hud.setCharacterSize(75);
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	Clock clock;
	while (window.isOpen()) {

		// 1. Handle input
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			bat.moveLeft();
		}
		else {
			bat.stopLeft();
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			bat.moveRight();
		}
		else {
			bat.stopRight();
		}
		// 2. Update objects and HUD
		Time dt = clock.restart();
		bat.update(dt);
		ball.update(dt);
		std::stringstream ss;
		ss << "Score: " << score << " Lives: " << lives;
		hud.setString(ss.str());

		// Ball Collision
		if (ball.getPosition().top > window.getSize().y) {
			// reverse ball direction
			ball.reboundBottom();
			lives--;
			if (lives < 1) {
				score = 0;
				lives = 3;
			}
		}
		if (ball.getPosition().top < 0) {
			ball.reboundBatOrTop();
			score++;
		}
		if (ball.getPosition().left < 0 ||
			ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
			ball.reboundSides();
		}
		if (ball.getPosition().intersects(bat.getPosition())) {
			ball.reboundBatOrTop();
		}
		// 3. Draw objects and HUD

		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
		window.draw(ball.getShape());
		window.display();

	}

	return 0;
}