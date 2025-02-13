#include"Game.h"
#include<math.h>

Game::Game() {
    init();
}
void Game::init() {
    
    m_window.create(sf::VideoMode(1920, 1080), "2D Geometry War",sf::Style::Fullscreen);
    m_window.setFramerateLimit(60);

    m_font.loadFromFile("Roboto\\static\\Roboto-BoldItalic.ttf");
    m_scoreText.setFont(m_font);
    m_scoreText.setPosition(0,25);
    m_scoreText.setFillColor(sf::Color::White);

    m_healthText.setFont(m_font);
    m_healthText.setPosition(0,0);
    m_healthText.setFillColor(sf::Color::White);

    m_gameOverText.setFont(m_font);
    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setPosition(1920/2, 1080/2);
    m_gameOverText.setCharacterSize(90);
    m_gameOverText.setFillColor(sf::Color::Red);
    sf::FloatRect rc = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin(rc.width / 2, rc.height / 2);

    m_playerGunshotBuffer.loadFromFile("Sound Effects\\402012__eardeer__gunshot__high_5.wav");
    m_playerGunshotSound.setBuffer(m_playerGunshotBuffer);

    m_enemyGunshotBuffer.loadFromFile("Sound Effects\\402013__eardeer__gunshot__high_4.wav");
    m_enemyGunshotSound.setBuffer(m_enemyGunshotBuffer);

    m_specialGunshotBuffer.loadFromFile("Sound Effects\\564480__lumikon__shotgun-shot-sfx.wav");
    m_specialGunshotSound.setBuffer(m_specialGunshotBuffer);

    m_playerDeathBuffer.loadFromFile("Sound Effects\\717768__1bob__death-sound.wav");
    m_playerDeathSound.setBuffer(m_playerDeathBuffer);

    m_enemyDeathBuffer.loadFromFile("Sound Effects\\641486__duskbreaker__8bit-explosion.wav");
    m_enemyDeathSound.setBuffer(m_enemyDeathBuffer);

    spawnPlayer();
}

void Game::run() {
    while (m_running) {

        sInput();
        m_scoreText.setString("Score: " + std::to_string(m_score));
        m_healthText.setString("Health: " + std::to_string(m_lives));

        if (!m_pause && m_lives > 0) {
            m_entities.Update();
            sEnemySpawner();
            sMovement();
            sCollision();
            sRespawn();
            sLifespan();
            sRetaliation();
            m_currentFrame++;
        }
        else if (m_lives == 0) {
            m_entities.Update();
            sCollision();
            sLifespan();
            sMovement();
            m_currentFrame++;
        }
        sRender();
    }
    
}

void Game::reset() {
    for (auto e : m_entities.getEntities("Enemy")) {
        e->destroy();
    }
    m_score = 0;
    m_lives = 10;
}

void Game::spawnPlayer() {
    auto entity = m_entities.addEntity("Player");
    
    entity->cTransform = std::make_shared<CTransform>(Vec2(960, 540), Vec2(1, 1), 0);
    entity->cShape = std::make_shared<CShape>(player_radius, 8, sf::Color(10, 10, 10), sf::Color(0, 255, 0), 4);
    entity->cInput = std::make_shared<CInput>();
    entity->cCollision = std::make_shared<CCollision>(32);
    m_player = entity;
}

void Game::setPause(bool pause) {
    if (pause) {
        m_pause = true;
    }
    else {
        m_pause = false;
    }
}

void Game::sEnemySpawner() {
    int current_spawn_interval = m_currentFrame - m_lastenemyspawntime;
    int scaler = 80 - std::round((m_currentFrame / 300));
    if (current_spawn_interval >= std::max(scaler,5)) {
        spawnEnemy();
    }
}

void Game::spawnEnemy() {
    auto entity = m_entities.addEntity("Enemy");
    int rand_vce = 4 + (rand() % (1 + 8 - 4));
    int rand_vx = -2 + (rand() % (1 + 2 - -2));
    int rand_vy = -2 + (rand() % (1 + 2 - -2));
    int player_x = m_player->cTransform->pos.x;
    int player_y = m_player->cTransform->pos.y;
    while (true) {
        int rand_x = player_radius + (rand() % (1 + 1920 - (5*player_radius)));
        int rand_y = player_radius + (rand() % (1 + 1080 - (5*player_radius)));

        if (abs(rand_x - player_x) > 32 && abs(rand_y - player_y) > player_radius) {
            entity->cTransform = std::make_shared<CTransform>(Vec2(rand_x, rand_y), Vec2(rand_vx, rand_vy), 0); //randomize spawn everything
            entity->cShape = std::make_shared<CShape>(32, rand_vce, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4);
            entity->cCollision = std::make_shared<CCollision>(player_radius);
            m_lastenemyspawntime = m_currentFrame;
            m_enemyspawntime = m_currentFrame;
            break;
        }
    }
}

void Game::spawnSmallEnemy(std::shared_ptr<Entity> e) {
    int a = 0;
    int r = 10;
    int vertices = e->cShape->circle.getPointCount();
    Vec2 center = e->cTransform->pos;
    float radius = e->cShape->circle.getRadius();
    const sf::Color & fill = e->cShape->circle.getFillColor();
    const sf::Color & outline = e->cShape->circle.getOutlineColor();
    int thickness = e->cShape->circle.getOutlineThickness();
    Vec2 launch_velocity(0, r);
    double theta = (4 * acos(0.0)) / vertices;

    for (int i = 1; i <= vertices; i++) {
        auto smallenemy = m_entities.addEntity("SmallEnemy");
        smallenemy->cShape = std::make_shared<CShape>(radius/2, vertices, fill, outline, thickness);
        smallenemy->cTransform = std::make_shared<CTransform>(center, launch_velocity, a);
        smallenemy->cLifespan = std::make_shared<CLifespan>(40);
        launch_velocity.x = r * sin(theta*i); 
        launch_velocity.y = r * cos(theta*i);
    }
}

void Game::spawnPlayerBullet(std::shared_ptr<Entity> e, const Vec2& firing_vec) {
    Vec2 pos = e->cTransform->pos;
    int a = 0;
    auto bullet = m_entities.addEntity("Bullet_Player");
    bullet->cTransform = std::make_shared<CTransform>(pos, firing_vec, a);
    bullet->cShape = std::make_shared<CShape>(10, 20, sf::Color(170, 215, 255), sf::Color(170, 215, 255), 2);
    bullet->cLifespan = std::make_shared<CLifespan>(150);
    bullet->cCollision = std::make_shared<CCollision>(60);
    m_playerGunshotSound.play();
    
}

void Game::spawnSpecialAttack(std::shared_ptr<Entity> e, const Vec2& firing_vec) {
    double pi = 2 * acos(0.0);
    float deg_30_in_rad = pi / 6;
    Vec2 v(1, 0);
    for (int i = 1; i <= 12; i++) {
        v.normalize();
        spawnPlayerBullet(e, v * 20);
        v.x = v.x * cos(deg_30_in_rad) - v.y * sin(deg_30_in_rad);
        v.y = v.x * sin(deg_30_in_rad) + v.y * cos(deg_30_in_rad);
    }
    m_specialGunshotSound.play();
}

void Game::spawnEnemyBullet(std::shared_ptr<Entity> e, const Vec2& firing_vec) {
    Vec2 pos = e->cTransform->pos;
    int a = 0;
    auto bullet = m_entities.addEntity("Bullet_Enemy");
    bullet->cTransform = std::make_shared<CTransform>(pos, firing_vec, a);
    bullet->cShape = std::make_shared<CShape>(10, 20, sf::Color(255, 165, 0), sf::Color(255, 165, 0), 2);
    bullet->cLifespan = std::make_shared<CLifespan>(120);
    bullet->cCollision = std::make_shared<CCollision>(60);
    m_enemyGunshotSound.play();
}

void Game::sLifespan() {
    for (auto e : m_entities.getEntities()) {
        if (e->cLifespan) {

            auto color = e->cShape->circle.getFillColor();
            int alpha = std::max(0, std::min(255, static_cast<int>(std::round(((float)e->cLifespan->remaining / (float)e->cLifespan->total) * 255))));
            e->cShape->circle.setFillColor(sf::Color(color.r, color.g, color.b, alpha));

            if (e->cLifespan->remaining > 0) {
                e->cLifespan->remaining--;
            } 
            if (e->cLifespan->remaining == 0){
                e->destroy();
            }
                
        }
    }
}

void Game::sRender() {

    m_window.clear();
    m_window.draw(m_healthText);
    m_window.draw(m_scoreText);

    for (auto e : m_entities.getEntities()) {
        e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        if (e->tag() != "Bullet_Player" || e->tag() != "Bullet_Enemy") {
            e->cTransform->angle += 1;
            e->cShape->circle.setRotation(e->cTransform->angle);
        }
        m_window.draw(e->cShape->circle);
        
    }
    if (m_lives == 0)
        m_window.draw(m_gameOverText);

    m_window.display();
}

void Game::sMovement() {
    for (auto e : m_entities.getEntities()) {
        e->cTransform->pos += e->cTransform->velocity;
    }

    Vec2 playerVelocity(0, 0);
    if (m_player->cInput->left) {
        playerVelocity.x -= 8; //playerconfig speed
    }
    if (m_player->cInput->right) {
        playerVelocity.x += 8; //playerconfig speed
    }
    if (m_player->cInput->up) {
        playerVelocity.y -= 8; //playerconfig speed
    }
    if (m_player->cInput->down) {
        playerVelocity.y += 8; //playerconfig speed
    }
    m_player->cTransform->velocity = playerVelocity;

}

void Game::Bullet_reset() {
    for (auto b : m_entities.getEntities("Bullet_Enemy")) {
        b->destroy();
    }
}

void Game::sCollision() {

    for (auto p : m_entities.getEntities("Player")) {
        float radius = p->cCollision->radius; //player cannot move past borders of the window
        float current_px = (p->cTransform->pos.x);
        float current_py = (p->cTransform->pos.y); 

        if (current_px <= (0 + radius)) {
            p->cTransform->pos.x = radius; 
        }
        else if (current_px >= (1920 - radius)) {
            p->cTransform->pos.x = 1920 - radius;
        }

        if (current_py <= (0 + radius)) {
            p->cTransform->pos.y = radius; 
        }
        else if (current_py >= (1080 - radius)) {
            p->cTransform->pos.y = 1080 - radius;
        }
        
        for (auto e : m_entities.getEntities("Enemy")) { //player collides with enemy and dies
            float collision_dist = (p->cCollision->radius) + (e->cCollision->radius);
            float current_dist_sqr = p->cTransform->pos.dist_sqr(e->cTransform->pos);
            if (current_dist_sqr <= collision_dist*collision_dist) {
                p->destroy(); 
                m_playerDeathSound.play();
                m_lives--;
                player_isAlive = false;
                spawnSmallEnemy(p);
                Bullet_reset();
                m_deathtime = m_currentFrame;
                e->destroy();
                m_enemyDeathSound.play();
                spawnSmallEnemy(e);
            }
        }

        for (auto b : m_entities.getEntities("Bullet_Enemy")) { //player is shot by enemy and dies
            float collision_dist = (p->cCollision->radius) + (b->cCollision->radius);
            float current_dist_sqr = p->cTransform->pos.dist_sqr(b->cTransform->pos);
            if (current_dist_sqr <= collision_dist * collision_dist) {
                m_deathtime = m_currentFrame;
                p->destroy();
                m_playerDeathSound.play();
                m_lives--;
                player_isAlive = false;
                spawnSmallEnemy(p);
                Bullet_reset();
                b->destroy();
            }
        }
    }

    for (auto b : m_entities.getEntities("Bullet_Player")) { //bullet destroys enemy
        for (auto e : m_entities.getEntities("Enemy")) {
            float collision_dist = (b->cCollision->radius) + (e->cCollision->radius);
            float current_dist_sqr = (b->cTransform->pos).dist_sqr(e->cTransform->pos);
            if (current_dist_sqr <= collision_dist*collision_dist) {
                b->destroy();
                e->destroy();
                m_enemyDeathSound.play();
                spawnSmallEnemy(e);
                m_score++;
            }
        }
    }

    for (auto e : m_entities.getEntities("Enemy")) { //enemy bounces off wall
        float radius = e->cCollision->radius;
        float current_x = (e->cTransform->pos.x);
        float current_y = (e->cTransform->pos.y);
        if (current_x < (0 + radius) || current_x >(1920 - radius)) {
            e->cTransform->velocity.x *= -1;
        } 
        if (current_y < (0 + radius) || current_y >(1080 - radius)) {
            e->cTransform->velocity.y *= -1;
        }

    }
}
void Game::sRespawn() {
    if (!player_isAlive && m_lives > 0) {
        if (m_currentFrame - m_deathtime >= 45) {
            spawnPlayer();
            player_isAlive = true;
        }
    }
}

void Game::sRetaliation() {

    for (auto e : m_entities.getEntities("Enemy")) {
        int last_shot_fired;
        if (player_isAlive) {
            if (m_currentFrame - m_enemyspawntime == 0) {
                Vec2 firing_vec = m_player->cTransform->pos - e->cTransform->pos;
                firing_vec.normalize();
                spawnEnemyBullet(e, firing_vec * 5);
            }
            else {
                int shoot = 1 + (rand() % (1 + 30 - 1));
                if (shoot == 5) {
                    Vec2 firing_vec = m_player->cTransform->pos - e->cTransform->pos;
                    firing_vec.normalize();
                    spawnEnemyBullet(e, firing_vec * 5);
                }
            }

        }
    }
}

void Game::sInput(){
    sf::Event event;
    while (m_window.pollEvent(event)) {

        if (event.type == sf::Event::Closed) { //exit the game
            m_running = false; 
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            m_running = false;
        }

        if (event.type == sf::Event::KeyPressed) { //key press
            switch (event.key.code) {
            case sf::Keyboard::W: //W for up
                m_player->cInput->up = true;
                break;

            case sf::Keyboard::A: //A for left
                m_player->cInput->left = true;
                break;

            case sf::Keyboard::S: //S for down
                m_player->cInput->down = true;
                break;

            case sf::Keyboard::D: //D for right
                m_player->cInput->right = true;
                break;

            case sf::Keyboard::Space: //Space to pause
                if (m_pause == false && m_lives > 0) {
                    setPause(true);
                }
                else if(m_pause == true && m_lives > 0){
                    setPause(false);
                }
                else {
                    reset();
                }
                break;
            }
        }

        if (event.type == sf::Event::KeyReleased) { //key release
            switch (event.key.code) {
            case sf::Keyboard::W: //release W
                m_player->cInput->up = false;
                break;

            case sf::Keyboard::A: //release A
                m_player->cInput->left = false;
                break;

            case sf::Keyboard::S: //release S
                m_player->cInput->down = false;
                break;

            case sf::Keyboard::D: //release D
                m_player->cInput->right = false;
                break;
            }

        }

        if (event.type == sf::Event::MouseButtonPressed) {
            Vec2 mouse_vec(event.mouseButton.x, event.mouseButton.y);

            if (event.mouseButton.button == sf::Mouse::Left && player_isAlive == true) {
                Vec2 firing_vec = mouse_vec - m_player->cTransform->pos;
                firing_vec.normalize();
                spawnPlayerBullet(m_player, firing_vec*20); //bullet speed
            }
            if (event.mouseButton.button == sf::Mouse::Right && player_isAlive == true && m_currentFrame - m_lastframefired >= 75) {
                m_lastframefired = m_currentFrame;
                Vec2 firing_vec = mouse_vec - m_player->cTransform->pos;
                firing_vec.normalize();
                spawnSpecialAttack(m_player, firing_vec * 20);
            }
        }


    }
        
}


