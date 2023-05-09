#pragma once
#include "engine.h"

class GameSoundsComponent : public Component {
protected:
    std::shared_ptr<sf::SoundBuffer> playerHitBuffer, enemyJumpBuffer, collectBuffer;
	sf::Sound playerHitSound, enemyJumpSound, collectSound;
public:
    void playPlayerHitSound();
    void playEnemyJumpSound();
    void playCollectSound();

    void Update(double dt) override {}
    void Render() override {}
    GameSoundsComponent() = delete;
    explicit GameSoundsComponent(Entity* p);
};