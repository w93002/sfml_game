#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Game");

    // Create the player shape (circle)
    sf::CircleShape player(40.f);
    player.setFillColor(sf::Color::Cyan);
    player.setPosition(300.f, 200.f);

    // Create the target (smaller circle)
    sf::CircleShape target(20.f);
    target.setFillColor(sf::Color::Green);
    target.setPosition(rand() % window.getSize().x, rand() % window.getSize().y); // Random initial position

    // Speed for the target movement
    sf::Vector2f targetVelocity(100.f, 100.f); // Target's velocity (move 100px per second in both directions)

    // Clock for controlling the frame rate and the countdown
    sf::Clock clock;
    float playerSpeed = 200.f; // Movement speed in pixels per second

    // Game variables
    bool gameOver = false;
    int score = 0; // Score counter
    sf::Clock countdownClock; // Countdown clock
    float countdownTime = 30.f; // 30 seconds countdown

    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Game loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart(); // Get the time elapsed since the last frame
        sf::Time elapsedTime = countdownClock.getElapsedTime(); // Time passed for countdown

        // Calculate remaining time and make sure it doesn't go below 0
        float remainingTime = std::max(0.f, countdownTime - elapsedTime.asSeconds());

        // If time is over, stop the game
        if (remainingTime <= 0.f) {
            gameOver = true;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Game logic (if game is not over)
        if (!gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                player.move(0.f, -playerSpeed * deltaTime.asSeconds()); // Move up
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                player.move(0.f, playerSpeed * deltaTime.asSeconds()); // Move down
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                player.move(-playerSpeed * deltaTime.asSeconds(), 0.f); // Move left
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                player.move(playerSpeed * deltaTime.asSeconds(), 0.f); // Move right
            }

            // Check for collision with the target
            if (player.getGlobalBounds().intersects(target.getGlobalBounds())) {
                // Increase score and move the target to a new random position
                score++;
                target.setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
            }

            // Bouncing target (reverse direction when hitting window boundaries)
            if (target.getPosition().x <= 0 || target.getPosition().x + target.getRadius() * 2 >= window.getSize().x) {
                targetVelocity.x = -targetVelocity.x; // Reverse horizontal direction
            }
            if (target.getPosition().y <= 0 || target.getPosition().y + target.getRadius() * 2 >= window.getSize().y) {
                targetVelocity.y = -targetVelocity.y; // Reverse vertical direction
            }

            // Move the target based on its velocity
            target.move(targetVelocity * deltaTime.asSeconds());
        }

        // Clear the window
        window.clear();

        // Draw the player and the target
        window.draw(player);
        window.draw(target);

        // Display the score
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return -1;
        }
        sf::Text scoreText("Score: " + std::to_string(score), font, 20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10.f, 10.f);
        window.draw(scoreText);

        // Display the remaining time
        sf::Text timeText("Time: " + std::to_string(static_cast<int>(remainingTime)), font, 20);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(10.f, 30.f);
        window.draw(timeText);

        // Display the window content
        window.display();

        // Display "game over" text if the game is over
        if (gameOver) {
            sf::Text gameOverText("Game Over! Final Score: " + std::to_string(score), font, 30);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(150.f, 200.f);
            window.draw(gameOverText);
        }
    }

    return 0;
}
