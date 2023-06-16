#include <SFML/Graphics.hpp>
#include <list>
#include <time.h>

using namespace sf;

sf::Font font;
sf::Text scoreText;
sf::Text timeText;
sf::Text menuText;
sf::Text gameOverText;

int score = 0;
sf::Clock gameClock; // игровые часы для работы тиков
sf::Clock stopwatch; // секундомер

enum GameState { MENU, GAME, GAME_OVER }; // текущий стейт режима или меню
GameState state = MENU;

int N = 30, M = 20;
int size = 16; 
int w = size * N;
int h = size * M + 50; // сдвиг по y для оторбажения счёта и секундомера сверху

int dir;
int num = 4;

struct Snake
{ 
    int x, y;
} s[100];

struct Fruit
{
    int x, y;  
} f;

std::list<Snake> snake;

bool gameOver = false;

void Tick()
{
    for (int i = num; i > 0; --i)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    if (dir == 0) 
        s[0].y += 1;
    if (dir == 1) 
        s[0].x -= 1;
    if (dir == 2) 
        s[0].x += 1;
    if (dir == 3) 
        s[0].y -= 1;

    if ((s[0].x == f.x) && (s[0].y == f.y))
    {
        score++;
        num++;
        f.x = rand() % N;
        f.y = rand() % M;
    }

    if (s[0].x > N) 
        s[0].x = 0; 
    if (s[0].x < 0) 
        s[0].x = N;
    if (s[0].y > M) 
        s[0].y = 0; 
    if (s[0].y < 0) 
        s[0].y = M;

    // проверка удара об саму змею
    for (int i = 1; i < num; i++)
    {
        if (s[0].x == s[i].x && s[0].y == s[i].y) 
            state = GAME_OVER;

    }
}


int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(w, h), "Zmeika!");

    font.loadFromFile("arial.ttf");

    scoreText.setFont(font);
    scoreText.setCharacterSize(24); 
    scoreText.setFillColor(sf::Color::Red); 
    scoreText.setPosition(10, 10); 

    timeText.setFont(font);
    timeText.setCharacterSize(24); 
    timeText.setFillColor(sf::Color::Red); 
    timeText.setPosition(w - 150, 10); 
    

    menuText.setFont(font);
    menuText.setCharacterSize(24); 
    menuText.setFillColor(sf::Color::Red); 
    menuText.setPosition(20, h/2 - 50); 
    // sfml не очень дружит с русским текстом
    menuText.setString("press SPACE to start");

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24); 
    gameOverText.setFillColor(sf::Color::Red); 
    gameOverText.setPosition(20, h/2 - 50); 
    gameOverText.setString("gameover, press SPACE to restart");


    Texture t1, t2;
    t1.loadFromFile("white.png");
    t2.loadFromFile("red.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);

    Clock clock;
    float timer=0;
    float delay=0.1;

    s[0].x = s[0].y = s[1].x = s[1].y = s[2].x = s[2].y = s[3].x = s[3].y = 10;
    f.x = 15; f.y = 15; 

    while (window.isOpen())
    {
        float time = gameClock.getElapsedTime().asSeconds();
        gameClock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) 
            dir = 1;
        if (Keyboard::isKeyPressed(Keyboard::Right))  
            dir = 2;
        if (Keyboard::isKeyPressed(Keyboard::Up)) 
            dir = 3;
        if (Keyboard::isKeyPressed(Keyboard::Down)) 
            dir = 0;

        
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            if (state == MENU) {
                state = GAME;
                // перезпускаем секундомер
                stopwatch.restart();  
            } else if (state == GAME_OVER) {
                // обнуляем игровые перменные
                score = 0;
                num = 4;
                dir = 0;
                gameOver = false;
                // растартим положение змейки
                for(int i = 0; i < num; i++){
                    s[i].x = 10;
                    s[i].y = 10;
                }
                // и фруктов
                f.x = 15; f.y = 15;
                // возвращаем стейт на ИГРА
                state = GAME;
                gameClock.restart();  // обновляем внутриигровые часы для тиков
                stopwatch.restart();
            }
        }

        if (timer > delay && state == GAME && !gameOver)
        {
            timer = 0;
            Tick();
            scoreText.setString("Score: " + std::to_string(score));

            sf::Time elapsed = stopwatch.getElapsedTime();
            timeText.setString("Time: " + std::to_string(int(elapsed.asSeconds())));
        }


        window.clear();

        if (state == MENU) {
            window.draw(menuText);
        }
        else if (state == GAME) {
            window.draw(scoreText);
            window.draw(timeText);

            for (int i=0; i<N; i++) {
                for (int j=0; j<M; j++) 
                { 
                    sprite1.setPosition(i*size, j*size + 50);
                    window.draw(sprite1);
                }
            }

            for (int i=0; i<num; i++)
            {
                sprite2.setPosition(s[i].x*size, s[i].y*size + 50);
                window.draw(sprite2); 
            }

            sprite2.setPosition(f.x*size,  f.y*size + 50);  
            window.draw(sprite2);  

        } 
        else if (state == GAME_OVER) {
            window.draw(gameOverText);
        }
        window.display();
    }

    return 0;
}
