#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

namespace sh
{
    const float WindowWidht = 800;
    const float WindowHeight = 600;

    class Object
    {
    private:
        float width;
        float height;
        float horizontalSpeed;
        float verticalSpeed;
        bool horizontalDirection;
        bool verticalDirection;
        sf::Clock changeColorTimer;
        float interval;

    protected:
        sf::Shape *shape;

    public:
        Object()
        {
            this->horizontalSpeed = (rand() % 150 + 50) / 100.f;
            // this->horizontalSpeed = 8656.65;
            this->verticalSpeed = (rand() % 150 + 50) / 100.f;
            // this->verticalSpeed = 2000;
            this->horizontalDirection = (bool)(rand() % 2);
            this->verticalDirection = (bool)(rand() % 2);
            this->interval = rand() % 5 + 1;
        }

        ~Object()
        {
            delete this->shape;
        }

        void draw(sf::RenderWindow &window)
        {
            window.draw(*this->shape);
        }

        void changeHorizontalDirection()
        {
            this->horizontalDirection = !this->horizontalDirection;
        }

        void changeVerticalDirection()
        {
            this->verticalDirection = !this->verticalDirection;
        }

        void setSizes(float width, float height)
        {
            this->width = width;
            this->height = height;
        }

        void setPosition()
        {
            float x = rand() % (int)(WindowWidht - this->width);
            float y = rand() % (int)(WindowHeight - this->height);
            this->shape->setPosition(x, y);
        }

        void changeColor()
        {
            sf::Color newColor = sf::Color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55);
            while (newColor == this->shape->getFillColor())
            {
                newColor = sf::Color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55);
            }
            this->shape->setFillColor(sf::Color(rand() % 200 + 55, rand() % 200 + 55, rand() % 200 + 55));
        }

        void move()
        {
            int multiplierX = horizontalDirection ? 1 : -1;
            int multiplierY = verticalDirection ? 1 : -1;
            float vectorX = this->horizontalSpeed * multiplierX;
            float vectorY = this->verticalSpeed * multiplierY;

            float nextX = this->shape->getPosition().x + vectorX;

            if (nextX < 0)
            {
                float newVectorX = -this->shape->getPosition().x;
                float multiplier = newVectorX / vectorX;
                vectorX = newVectorX;
                vectorY = vectorY * multiplier;
                changeHorizontalDirection();
            }
            else if (nextX > WindowWidht - this->width)
            {
                float newVectorX = WindowWidht - this->width - this->shape->getPosition().x;
                float multiplier = newVectorX / vectorX;
                vectorX = newVectorX;
                vectorY = vectorY * multiplier;
                changeHorizontalDirection();
            }

            float nextY = this->shape->getPosition().y + vectorY;

            if (nextY < 0)
            {
                float newVectorY = -this->shape->getPosition().y;
                float multiplier = newVectorY / vectorY;
                vectorY = newVectorY;
                vectorX = vectorX * multiplier;
                changeVerticalDirection();
            }
            else if (nextY > WindowHeight - this->height)
            {
                float newVectorY = WindowHeight - this->height - this->shape->getPosition().y;
                float multiplier = newVectorY / vectorY;
                vectorY = newVectorY;
                vectorX = vectorX * multiplier;
                changeVerticalDirection();
            }

            this->shape->move(sf::Vector2f(vectorX, vectorY));
        }

        void UpdateObject(sf::RenderWindow &window)
        {
            float elapsedTime = this->changeColorTimer.getElapsedTime().asSeconds();
            if (elapsedTime > this->interval)
            {
                this->changeColor();
                this->changeColorTimer.restart();
            }
            this->move();
            this->draw(window);
        }
    };

    class Triangle : public Object
    {
    public:
        Triangle()
        {
            float radius = rand() % 50 + 30;
            sf::CircleShape *shape = new sf::CircleShape(radius, 3);
            this->shape = shape;
            this->setSizes(this->shape->getGlobalBounds().width, this->shape->getGlobalBounds().height);
            this->shape->setOrigin(radius - (this->shape->getGlobalBounds().width / 2.f), 0);
            this->changeColor();
            this->setPosition();
        }
    };

    class Circle : public Object
    {
    public:
        Circle()
        {
            float radius = rand() % 50 + 30;
            sf::CircleShape *shape = new sf::CircleShape(radius);
            this->shape = shape;
            this->setSizes(this->shape->getGlobalBounds().width, this->shape->getGlobalBounds().height);
            this->changeColor();
            this->setPosition();
        }
    };

    class Rectangle : public Object
    {
    public:
        Rectangle()
        {
            float width = rand() % 50 + 30;
            float height = rand() % 50 + 30;
            sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(width, height));
            this->shape = shape;
            this->setSizes(this->shape->getGlobalBounds().width, this->shape->getGlobalBounds().height);
            this->changeColor();
            this->setPosition();
        }
    };

    class Square : public Object
    {
    public:
        Square()
        {
            float size = rand() % 50 + 30;
            sf::RectangleShape *shape = new sf::RectangleShape(sf::Vector2f(size, size));
            this->shape = shape;
            this->setSizes(this->shape->getGlobalBounds().width, this->shape->getGlobalBounds().height);
            this->changeColor();
            this->setPosition();
        }
    };

    class Octagon : public Object
    {
    public:
        Octagon()
        {
            float radius = rand() % 50 + 30;
            sf::CircleShape *shape = new sf::CircleShape(radius, 8);
            this->shape = shape;
            this->setSizes(this->shape->getGlobalBounds().width, this->shape->getGlobalBounds().height);
            this->shape->setOrigin(radius - (this->shape->getGlobalBounds().width / 2.f), radius - (this->shape->getGlobalBounds().height / 2.f));
            this->changeColor();
            this->setPosition();
        }
    };

    class Application
    {
    private:
        sf::RenderWindow *window;
        bool isPaused;
        std::string title;
        Object *objects;
        unsigned int objectsCount;

    public:
        Application(std::string title, unsigned int countObjects)
        {
            srand(time(0));
            this->title = title;
            this->window = new sf::RenderWindow(sf::VideoMode(WindowWidht, WindowHeight), title);
            this->window->setFramerateLimit(60);
            this->window->setVerticalSyncEnabled(true);
            this->window->setKeyRepeatEnabled(false);
            this->isPaused = false;
            this->objectsCount = countObjects;
            this->objects = new Object[countObjects];
            this->isPaused = true;
        }

        ~Application()
        {
            delete this->window;
            delete[] this->objects;
        }

        void setup()
        {
            for (int i = 0; i < this->objectsCount; i++)
            {
                Object *object = nullptr;
                int choice = rand() % 5;
                switch (choice)
                {
                case 0:
                {
                    Circle *circle = new Circle();
                    object = circle;
                    break;
                }
                case 1:
                {
                    Triangle *triangle = new Triangle();
                    object = triangle;
                    break;
                }
                case 2:
                {
                    Rectangle *rectangle = new Rectangle();
                    object = rectangle;
                    break;
                }
                case 3:
                {
                    Square *square = new Square();
                    object = square;
                    break;
                }
                case 4:
                {
                    Octagon *octagon = new Octagon();
                    object = octagon;
                    break;
                }
                default:
                {
                    Circle *circle = new Circle();
                    object = circle;
                    break;
                }
                }

                this->objects[i] = *object;
            }
        }

        void Update()
        {
            this->window->clear(sf::Color::Black);
            for (int i = 0; i < this->objectsCount; i++)
            {
                this->objects[i].UpdateObject(*this->window);
            }
            this->window->display();
        }

        void run()
        {
            this->setup();
            if (this->window->isOpen())
            {
                this->Update();
            }
            while (this->window->isOpen())
            {

                sf::Event event;
                while (this->window->pollEvent(event))
                {
                    switch (event.type)
                    {
                    case sf::Event::Closed:
                    {
                        this->window->close();
                        break;
                    }
                    case sf::Event::KeyPressed:
                    {
                        if (event.key.code == sf::Keyboard::Escape)
                        {
                            this->window->close();
                        }
                        else if (event.key.code == sf::Keyboard::Space)
                        {
                            this->isPaused = !this->isPaused;
                        }
                        break;
                    }
                    }
                }
                if (!this->isPaused)
                {
                    this->Update();
                }
            }
        }
    };
}

int main()
{
    std::string title = "Lab3";
    unsigned int countObjects = 500;
    sh::Application app(title, countObjects);
    app.run();
    return 0;
}