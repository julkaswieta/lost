#include "cmp_game_sounds.h"

#include "../save_system.h"
#include "system_renderer.h"
#include "system_resources.h"

void GameSoundsComponent::playPlayerHitSound() { playerHitSound.play(); }
void GameSoundsComponent::playEnemyJumpSound() { enemyJumpSound.play(); }
void GameSoundsComponent::playCollectSound() { collectSound.play(); }

GameSoundsComponent::GameSoundsComponent(Entity* p) : Component(p) {
	// Load sounds
	playerHitBuffer = Resources::get<sf::SoundBuffer>("PlayerHit.wav");
	playerHitSound = sf::Sound(*playerHitBuffer);
	playerHitSound.setVolume(SaveSystem::getVolume());
	enemyJumpBuffer = Resources::get<sf::SoundBuffer>("EnemyJump.wav");
	enemyJumpSound = sf::Sound(*enemyJumpBuffer);
	enemyJumpSound.setVolume(SaveSystem::getVolume());
	collectBuffer = Resources::get<sf::SoundBuffer>("StarCollect.wav");
	collectSound = sf::Sound(*collectBuffer);
	collectSound.setVolume(SaveSystem::getVolume());
}