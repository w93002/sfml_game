#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

class Bullet {
public:
    sf::RectangleShape shape;
    bool isActive;

    Bullet(float x, float y) {
        shape.setSize(sf::Vector2f(5.f, 20.f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
        isActive = true;
    }

    void update(float deltaTime) {
        shape.move(0.f, -300.f * deltaTime); // Move up
        if (shape.getPosition().y < 0) {
            isActive = false; // Deactivate when off-screen
        }
    }

    void reset(float x, float y) {
        shape.setPosition(x, y);
        isActive = true;
    }
};

class Enemy {
public:
    sf::RectangleShape shape;
    bool isActive;

    Enemy(float x, float y) {
        shape.setSize(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(x, y);
        isActive = true;
    }

    void update(float deltaTime) {
        shape.move(0.f, 150.f * deltaTime); // Move down
        if (shape.getPosition().y > 480) {
            isActive = false; // Deactivate when off-screen
        }
    }
};

class Player {
public:
    sf::RectangleShape shape;
    float speed;

    Player() {
        shape.setSize(sf::Vector2f(50.f, 50.f));
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(320.f, 430.f); // Starting at the bottom center
        speed = 300.f;
    }

    void update(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            shape.move(-speed * deltaTime, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            shape.move(speed * deltaTime, 0.f);
        }
    }
};

//int main() {
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "Space Shooter Game");

    Player player;
    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    sf::Clock clock;
    sf::Clock bulletClock;

    int score = 0;
    bool gameOver = false;

    // Load the font for the score display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    // Set up the score text
    sf::Text scoreText("Score: 0", font, 20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    // Set up the game over text
    sf::Text gameOverText("GAME OVER", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(180.f, 200.f);

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float elapsedBulletTime = bulletClock.getElapsedTime().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (gameOver) {
            // If game is over, don't process further actions
            continue;
        }

        // Player update
        player.update(deltaTime.asSeconds());

        // Shooting bullets
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && elapsedBulletTime > 0.2f) {
            bullets.push_back(Bullet(player.shape.getPosition().x + 22.f, player.shape.getPosition().y));
            bulletClock.restart();
        }

        // Update bullets
        for (auto& bullet : bullets) {
            bullet.update(deltaTime.asSeconds());
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return !b.isActive; }), bullets.end());

        // Spawn enemies with reduced frequency (50% chance)
        if (rand() % 100 < 1) { // Reduced chance to 1% instead of 2% to create fewer enemies
            float x = rand() % (window.getSize().x - 50); // Random x position
            enemies.push_back(Enemy(x, -50.f)); // Spawn above the screen
        }

        // Update enemies
        for (auto& enemy : enemies) {
            enemy.update(deltaTime.asSeconds());
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return !e.isActive; }), enemies.end());

        // Check for collisions between bullets and enemies
        for (auto& bullet : bullets) {
            for (auto& enemy : enemies) {
                if (bullet.shape.getGlobalBounds().intersects(enemy.shape.getGlobalBounds())) {
                    bullet.isActive = false; // Deactivate bullet
                    enemy.isActive = false; // Deactivate enemy
                    score++; // Increase score
                    break;
                }
            }
        }

        // Check for collisions between player and enemies (game over condition)
        for (auto& enemy : enemies) {
            if (player.shape.getGlobalBounds().intersects(enemy.shape.getGlobalBounds())) {
                gameOver = true; // Trigger game over
                break;
            }
        }

        // Clear the window
        window.clear();

        // Draw everything
        window.draw(player.shape);
        for (auto& bullet : bullets) {
            window.draw(bullet.shape);
        }
        for (auto& enemy : enemies) {
            window.draw(enemy.shape);
        }

        // Update and display the score
        scoreText.setString("Score: " + std::to_string(score));
        window.draw(scoreText);

        // Display "Game Over" if the game is over
        if (gameOver) {
            window.draw(gameOverText);
        }

        // Display the window contents
        window.display();
    }

    return 0;
}
