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
        START = 'p',
        END = 'g',
        WALL = 'w',
        ENEMY = 'e',
        SPIKE = 's',
        WAYPOINT = '+',
        COLLECTIBLE = 'c'
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

    static float tileSize; // for rendering
    static std::map<Tile, sf::Color> colours;

private:
    LevelSystem() = delete;
    ~LevelSystem() = delete;

    static void readInLevelFile(const std::string& path, std::string& buffer);
};
