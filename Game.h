#pragma once
#include"Common.h"
#include"Entity.h"
#include"EntityManager.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OB, OT, V; float s; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, Vmin, Vmax, L, SI; float Smin, Smax; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float s; };

class Game {

    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;

    sf::Text m_scoreText;
    sf::Text m_healthText;
    sf::Text m_gameOverText;

    sf::SoundBuffer m_playerGunshotBuffer;
    sf::Sound m_playerGunshotSound;

    sf::SoundBuffer m_enemyGunshotBuffer;
    sf::Sound m_enemyGunshotSound;

    sf::SoundBuffer m_specialGunshotBuffer;
    sf::Sound m_specialGunshotSound;

    sf::SoundBuffer m_playerDeathBuffer;
    sf::Sound m_playerDeathSound;

    sf::SoundBuffer m_enemyDeathBuffer;
    sf::Sound m_enemyDeathSound;

    //PlayerConfig m_playerconfig;
    //EnemyConfig m_enemyconfig;
    //BulletConfig m_bulletconfig;

    int m_score = 0;
    int m_lives = 10;
    int m_currentFrame = 0;
    int m_lastenemyspawntime = 0;
    int m_enemyspawntime = 0;
    int player_radius = 32;
    bool player_isAlive = true;
    bool m_pause = false;
    bool m_running = true;
    
    int m_deathtime;
    int m_lastframefired;
   

    std::shared_ptr<Entity> m_player;

    void init();
    void setPause(bool pause);
    void reset();

    void sMovement();
    void sInput();
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();
    void sRetaliation();
    void sRespawn();

    void Bullet_reset();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemy(std::shared_ptr<Entity> entity);
    void spawnPlayerBullet(std::shared_ptr<Entity> entity, const Vec2& firing_vec);
    void spawnSpecialAttack(std::shared_ptr<Entity> entity, const Vec2& firing_vec);
    void spawnEnemyBullet(std::shared_ptr<Entity> entity, const Vec2& firing_vec);
    

public:
    Game();
    bool m_newGame = true;
    void run();
};
