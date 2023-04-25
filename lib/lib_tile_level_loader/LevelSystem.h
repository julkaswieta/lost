#pragma once

#include <SFML/Graphics.hpp>
#include <maths.h>
#include <memory>
#include <string>
#include <vector>
#include <map>

#define ls LevelSystem

class LevelSystem {
public:
    static void LoadLevelFile(const std::string&, float tSize = 100.0f);
    static void Unload();
    static void Render(sf::RenderWindow& window);

    typedef unsigned char Tile;

    enum TILES {
        EMPTY = ' ',
        WALL = 'a',
        START = 'b',
        END = 'c',
        STAR = 'd',
        ENEMY = 'e',
        SPIKE_UP = 'f',
        SPIKE_DOWN = 'g',
        SPIKE_RIGHT = 'h',
        SPIKE_LEFT = 'i',
        SPIKE_BALL = 'j',
        SAWBLADE = 'k',
        // Continue here

/*****  Don't change these  *****/
        EXIT = 'v',
        SETTINGS = 'w',
        LEVEL1 = 'x',
        LEVEL2 = 'y',
        LEVEL3 = 'z'
    };

    static Tile getTile(sf::Vector2ul);

    static Tile getTileAt(sf::Vector2f);

    static bool isOnGrid(sf::Vector2f);

    static size_t getWidth();

    static size_t getHeight();

    static sf::Vector2f getTilePosition(sf::Vector2ul);

    static std::vector<sf::Vector2ul> findTiles(Tile);

    static sf::Color getColor(Tile t);

    static void setColor(Tile t, sf::Color c);

    static void setOffset(const sf::Vector2f& _offset);

    static const sf::Vector2f& getOffset();

    static float getTileSize();

protected:
    static std::unique_ptr<Tile[]> tiles;
    static size_t width;
    static size_t height;
    static sf::Vector2f offset;

    static std::vector<std::unique_ptr<sf::RectangleShape>> sprites;

    static void buildSprites(bool optimise = true);

    static float tileSize;
    static std::map<Tile, sf::Color> colours;

private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;

    static void readInLevelFile(const std::string& path, std::string& buffer);
};
