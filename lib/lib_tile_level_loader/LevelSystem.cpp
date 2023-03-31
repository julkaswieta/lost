#include "LevelSystem.h"
#include <fstream>

using namespace sf;
using namespace std;

unique_ptr<LevelSystem::Tile[]> LevelSystem::tiles;
size_t LevelSystem::width;
size_t LevelSystem::height;

float LevelSystem::TILE_SIZE(100.f);
Vector2f LevelSystem::offset(0.0f, 30.0f);
vector<unique_ptr<RectangleShape>> LevelSystem::sprites;

map<LevelSystem::Tile, Color> LevelSystem::colours {
    {WALL, Color::White}, {END, Color::Red}
};

void LevelSystem::readInLevelFile(const string& path, string& buffer) {
    // Load in file to buffer
    ifstream f(path);
    if (f.good()) {
        f.seekg(0, std::ios::end);
        buffer.resize(f.tellg());
        f.seekg(0);
        f.read(&buffer[0], buffer.size());
        f.close();
    }
    else {
        throw string("Couldn't open level file: ") + path;
    }
}

// load the specified level file 
void LevelSystem::LoadLevelFile(const string& path, float tileSize) {
    tileSize = tileSize;
    size_t w = 0, h = 0;
    string buffer;

    readInLevelFile(path, buffer);

    vector<Tile> temp_tiles;
    int widthCheck = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        const char c = buffer[i];
        if (c == '\0') { break; }
        if (c == '\n') { // newline
            if (w == 0) {  // if we haven't written width yet
                w = i;       // set width
            }
            else if (w != (widthCheck - 1)) {
                throw string("non uniform width:" + to_string(h) + " ") + path;
            }
            widthCheck = 0;
            h++; // increment height
        }
        else {
            temp_tiles.push_back((Tile)c);
        }
        ++widthCheck;
    }

    if (temp_tiles.size() != (w * h)) {
        throw string("Can't parse level file") + path;
    }
    tiles = std::make_unique<Tile[]>(w * h);
    width = w; // set static class vars
    height = h;
    copy(temp_tiles.begin(), temp_tiles.end(), &tiles[0]);
    cout << "Level " << path << " Loaded. " << w << "x" << h << std::endl;
    buildSprites();
}

// assign sprites to correct tiles
void LevelSystem::buildSprites(bool optimise) {
    sprites.clear();

    struct tileProps {
        sf::Vector2f pos;
        sf::Vector2f size;
        sf::Color colour;
    };
    vector<tileProps> tilesProps;
    const auto tileSize = Vector2f(TILE_SIZE, TILE_SIZE);
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            Tile t = getTile({ x, y });
            if (t == EMPTY) {
                continue;
            }
            tilesProps.push_back({ getTilePosition({x, y}), tileSize, getColor(t) });
        }
    }

    const auto nonEmptyTiles = tilesProps.size();  

    // If tile of the same type are next to each other,
    // We can use one large sprite instead of two.
    if (optimise && nonEmptyTiles) {

        vector<tileProps> xAxisOptimisedTiles;
        tileProps last = tilesProps[0];
        size_t samecount = 0;

        for (size_t i = 1; i < nonEmptyTiles; ++i) {
            // Is this tile compressible with the last?
            // check if it has the same size and colour
            bool same = ((tilesProps[i].pos.y == last.pos.y) &&
                (tilesProps[i].pos.x == last.pos.x + (tileSize.x * (1 + samecount))) &&
                (tilesProps[i].colour == last.colour));
            if (same) {
                ++samecount; // Yes, keep going
            }
            // if not compressible, expand the previous tiles that were compressible together
            else {
                if (samecount) {
                    last.size.x = (1 + samecount) * tileSize.x; // Expand tile
                }
                // write tile to list
                xAxisOptimisedTiles.push_back(last);
                samecount = 0;
                last = tilesProps[i]; // assign new tile to compare against 
            }
        }
        // catch the last tile
        if (samecount) {
            last.size.x = (1 + samecount) * tileSize.x;
            xAxisOptimisedTiles.push_back(last);
        }

        // Now scan down Y, using a different algo that considers that 
        // compressible blocks may not be contiguous
        const auto xsave = xAxisOptimisedTiles.size();
        samecount = 0;
        vector<tileProps> optimisedTiles;
        for (size_t i = 0; i < xAxisOptimisedTiles.size(); ++i) {
            last = xAxisOptimisedTiles[i];
            for (size_t j = i + 1; j < xAxisOptimisedTiles.size(); ++j) {
                // is the tile below the last tile compressible with last?
                bool same = ((xAxisOptimisedTiles[j].pos.x == last.pos.x) && (xAxisOptimisedTiles[j].size == last.size) &&
                    (xAxisOptimisedTiles[j].pos.y == last.pos.y + (tileSize.y * (1 + samecount))) &&
                    (xAxisOptimisedTiles[j].colour == last.colour));
                if (same) {
                    ++samecount;
                    // if it is, remove from further consideration 
                    xAxisOptimisedTiles.erase(xAxisOptimisedTiles.begin() + j);
                    --j;
                }
            }
            // if the next tile is not compressible with last, expand the previous compressible tiles
            if (samecount) {
                last.size.y = (1 + samecount) * tileSize.y; // Expand tile
            }
            // write tile to list
            optimisedTiles.push_back(last);
            samecount = 0;
        }
        // use the optimised tiles vectos from now on
        tilesProps.swap(optimisedTiles);
    }

    // assign the sprites, using the props from the optimised vector
    for (auto& t : tilesProps) {
        auto s = make_unique<sf::RectangleShape>();
        s->setPosition(t.pos);
        s->setSize(t.size);
        s->setFillColor(Color::Red);
        s->setFillColor(t.colour);
        sprites.push_back(move(s));
    }

    cout << "Level with " << (width * height) << " Tiles, With " << nonEmptyTiles
        << " Not Empty, using: " << sprites.size() << " Sprites\n";
}

// render the whole level 
void LevelSystem::Render(RenderWindow& window) {
    for (auto& t : sprites) {
        window.draw(*t);
    }
}

// get the global position of a tile in a grid
sf::Vector2f LevelSystem::getTilePosition(sf::Vector2ul p) {
    return (Vector2f(p.x, p.y) * TILE_SIZE) + offset;
}

// find all tiles of specified type
std::vector<sf::Vector2ul> LevelSystem::findTiles(LevelSystem::Tile type) {
    auto v = vector<sf::Vector2ul>();
    for (size_t i = 0; i < width * height; ++i) {
        if (tiles[i] == type) {
            v.push_back({ i % width, i / width });
        }
    }

    return v;
}

// get a tile from a specific global position
LevelSystem::Tile LevelSystem::getTileAt(Vector2f v) {
    auto a = v - offset;
    if (a.x < 0 || a.y < 0) {
        throw string("Tile out of range ");
    }
    return getTile(Vector2ul((v - offset) / (TILE_SIZE)));
}

// check if a position lies within tile grid 
bool LevelSystem::isOnGrid(sf::Vector2f v) {
    auto a = v - offset;
    if (a.x < 0 || a.y < 0) {
        return false;
    }
    auto p = Vector2ul((v - offset) / (TILE_SIZE));
    if (p.x > width || p.y > height) {
        return false;
    }
    return true;
}

// get tile at specific poistion in the tile grid
LevelSystem::Tile LevelSystem::getTile(sf::Vector2ul p) {
    if (p.x > width || p.y > height) {
        throw string("Tile out of range: ") + to_string(p.x) + "," +
            to_string(p.y) + ")";
    }
    return tiles[(p.y * width) + p.x];
}

Color LevelSystem::getColor(LevelSystem::Tile t) {
    auto it = colours.find(t);
    if (it == colours.end()) {
        colours[t] = Color::Transparent;
    }
    return colours[t];
}

void LevelSystem::setColor(LevelSystem::Tile t, sf::Color c) {
    colours[t] = c;
}

size_t LevelSystem::getWidth() { return width; }

size_t LevelSystem::getHeight() { return height; }

void LevelSystem::setOffset(const Vector2f& _offset) {
    LevelSystem::offset = _offset;
    buildSprites();
}

const Vector2f& LevelSystem::getOffset() { return offset; }

float LevelSystem::getTileSize() { return TILE_SIZE; }

void LevelSystem::Unload() {
    cout << "LevelSystem unloading\n";
    sprites.clear();
    tiles.reset();
    width = 0;
    height = 0;
    offset = { 0, 0 };
}