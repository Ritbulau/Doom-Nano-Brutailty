#include "constants.h"
#include "level.h"
#include "sprites.h"
#include "input.h"
#include "entities.h"
#include "types.h"
#include "display.h"
#include "sound.h"
using namespace std;
// Useful macros
#define swap(a, b)            do { typeof(a) temp = a; a = b; b = temp; } while (0)
#define sign(a, b)            (double) (a > b ? 1 : (b > a ? -1 : 0))

void setup(void) {

  setupDisplay();
  input_setup();
  sound_init();


}

uint8_t scene = INTRO;
bool exit_scene = false;
bool invert_screen = false;
uint8_t flash_screen = 0;
uint8_t z = 6;
bool coll = 0;
uint8_t jump = 0;
uint8_t jump_height = 0;
uint8_t vel = 1;
uint8_t difficulty = 1;
uint8_t noclip = 0;
bool m = true;
uint8_t rc1 = 0;
int16_t a = 0;  
uint8_t enemyCount2 = 0;
uint8_t enemyGoal2 = 8;
// game
// player and entities
Player player;
Entity entity[MAX_ENTITIES];
StaticEntity static_entity[MAX_STATIC_ENTITIES];
uint8_t num_entities = 0;
uint8_t num_static_entities = 0;
uint8_t x = 0;
uint8_t enemyCount = 0;
uint8_t del = 0;
bool levelID = false;
uint8_t enemyGoal = 20;
bool fade_e = true;
bool debug = false;
uint8_t r = 0;
bool reload1 = false;
int16_t score;
uint8_t k;
int8_t mid = 1;
bool bss = false;
bool mc = false;


// Jump to another scene
void jumpTo(uint8_t target_scene) {
  scene = target_scene;
  exit_scene = true;
  if (target_scene == INTRO) {
    if (player.health  == 0) {
      softReset();
    }
    else if (levelID == true) {
      softReset();
    }
    else {
    enemyCount = 0;
    } 
  }
}

// Finds the player in the map
void initializeLevel(const uint8_t level[]) {
  for (uint8_t y = LEVEL_HEIGHT - 1; y >= 0; y--) {
    for (uint8_t x = 0; x < LEVEL_WIDTH; x++) {
      uint8_t block = getBlockAt(level, x, y);

      if (block == E_PLAYER) {
        player = create_player(x, y);
        return;
      }

      // todo create other static entities
    }
  }
}

uint8_t getBlockAt(const uint8_t level[], uint8_t x, uint8_t y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return E_FLOOR;
  }

  // y is read in inverse order
  return pgm_read_byte(level + (((LEVEL_HEIGHT - 1 - y) * LEVEL_WIDTH + x) / 2))
         >> (!(x % 2) * 4)       // displace part of wanted bits
         & 0b1111;               // mask wanted bits
}

bool isSpawned(UID uid) {
  for (uint8_t i = 0; i < num_entities; i++) {
    if (entity[i].uid == uid) return true;
  }

  return false;
}

bool isStatic(UID uid) {
  for (uint8_t i = 0; i < num_static_entities; i++) {
    if (static_entity[i].uid == uid) return true;
  }

  return false;
}

void spawnEntity(uint8_t type, uint8_t x, uint8_t y) {
  // Limit the number of spawned entities
  if (num_entities >= MAX_ENTITIES) {
    return;
  }

  // todo: read static entity status
  
  switch (type) {
    case E_ENEMY:
      entity[num_entities] = create_enemy(x, y);
      num_entities++;
      break;

    case E_KEY:
      entity[num_entities] = create_key(x, y);
      num_entities++;
      break;
      

    case E_MEDIKIT:
      entity[num_entities] = create_medikit(x, y);
      num_entities++;
      break;


  }
}



void spawnFireball(double x, double y) {
  // Limit the number of spawned entities
  if (num_entities >= MAX_ENTITIES) {
    return;
  }

  UID uid = create_uid(E_FIREBALL, x, y);
  // Remove if already exists, don't throw anything. Not the best, but shouldn't happen too often
  if (isSpawned(uid)) return;

  // Calculate direction. 32 angles
  int16_t dir = FIREBALL_ANGLES + atan2(y - player.pos.y, x - player.pos.x) / PI * FIREBALL_ANGLES;
  if (dir < 0) dir += FIREBALL_ANGLES * 2;
  entity[num_entities] = create_fireball(x, y, dir);
  num_entities++;
}

void removeEntity(UID uid, bool makeStatic = false) {
  uint8_t i = 0;
  bool found = false;

  while (i < num_entities) {
    if (!found && entity[i].uid == uid) {
      // todo: doze it
      found = true;
      num_entities--;
    }

    // displace entities
    if (found) {
      entity[i] = entity[i + 1];
    }

    i++;
  }
}
void clearEntities() {
  uint8_t i = num_entities;
  bool found;
  while (found == false) {
    i--;
    if (entity[i].state != S_DEAD && uid_get_type(entity[i].uid) != E_ENEMY) {
      found = true;
    }
  }
  i--;
  while (entity[i].state != S_DEAD && uid_get_type(entity[i].uid) != E_ENEMY && found == true) {
    i--;
    if (i == 0) {
      break;
    }
  }
  if (i > 0 && entity[i].state == S_DEAD) {
    removeEntity(entity[i].uid);
  }
} 

void removeStaticEntity(UID uid) {
  uint8_t i = 0;
  bool found = false;

  while (i < num_static_entities) {
    if (!found && static_entity[i].uid == uid) {
      found = true;
      num_static_entities--;
    }

    // displace entities
    if (found) {
      static_entity[i] = static_entity[i + 1];
    }

    i++;
  }
}

UID detectCollision(const uint8_t level[], Coords *pos, double relative_x, double relative_y, bool only_walls = false) {
  // Wall collision
  uint8_t round_x = int(pos->x + relative_x);
  uint8_t round_y = int(pos->y + relative_y);
  uint8_t block = getBlockAt(level, round_x, round_y);

  if (block == E_WALL & debug == false) {
    playSound(hit_wall_snd, HIT_WALL_SND_LEN);
    return create_uid(block, round_x, round_y);
  }
  else if (block == E_DOOR && player.secret == false) {
    player.secret = true;
    z = 8;
    updateHud();
    playSound(s_snd, S_SND_LEN);
  }
  else if (block == E_DOOR2 && player.secret2 == false) {
    player.secret2 = true;
    z = 8;
    updateHud();
    playSound(s_snd, S_SND_LEN);
  }
  else if (block == E_DOOR3 && player.secret3 == false) {
    player.secret3 = true;
    z = 8;
    updateHud();
    playSound(s_snd, S_SND_LEN);
  }


  if (only_walls) {
    return UID_null;
  }

  // Entity collision
  for (uint8_t i=0; i < num_entities; i++) {
    // Don't collide with itself
    if (&(entity[i].pos) == pos) {
      continue;
    }

    uint8_t type = uid_get_type(entity[i].uid);

    // Only ALIVE enemy collision
    if (type != E_ENEMY || entity[i].state == S_DEAD || entity[i].state == S_HIDDEN) {
      continue;

    }



    Coords new_coords = { entity[i].pos.x - relative_x, entity[i].pos.y - relative_y };
    uint8_t distance = coords_distance(pos, &new_coords);
    

    // Check distance and if it's getting closer
    if (distance < ENEMY_COLLIDER_DIST && distance < entity[i].distance) {
      return entity[i].uid;
      
    }
  }

  return UID_null;
}



// Shoot
void fire() {
  if (player.keys != 0) {
    playSound(shoot_snd, SHOOT_SND_LEN);
    z = 3;
    for (uint8_t i = 0; i < num_entities; i++) {
      // Shoot only ALIVE enemies
      if (uid_get_type(entity[i].uid) != E_ENEMY || entity[i].state == S_DEAD || entity[i].state == S_HIDDEN) {
        continue;
      }
      Coords transform = translateIntoView(&(entity[i].pos));
      if (abs(transform.x) < 20 && transform.y > 0) {
        uint8_t damage = (double) min(GUN_MAX_DAMAGE, GUN_MAX_DAMAGE / (abs(transform.x) * entity[i].distance) / 5);
        if (jump == 1 || jump == 2) {
          damage = damage/3;
        }
        if (difficulty == 1) {
          damage = damage * 1.5;
        }
        else if (difficulty == 2) {
          damage = damage; 
        }
        else {
          damage = damage*0.70;
        }
        if (damage > 0) {
          entity[i].health = max(0, entity[i].health - damage/difficulty);
          entity[i].state = S_HIT;
          entity[i].timer = 2;
        }
      }
    }
  }
  else {
    playSound(melee_snd, MELEE_SND_LEN);
    z = 3;
    for (uint8_t i = 0; i < num_entities; i++) {
      if (entity[i].distance <= ENEMY_MELEE_DIST) {
        // Shoot only ALIVE enemies
        if (uid_get_type(entity[i].uid) != E_ENEMY || entity[i].state == S_DEAD || entity[i].state == S_HIDDEN) {
          continue;
        }
        Coords transform = translateIntoView(&(entity[i].pos));
        if (abs(transform.x) < 20 && transform.y > 0) {
          uint8_t damage = (double) min(GUN_MAX_DAMAGE, GUN_MAX_DAMAGE / (abs(transform.x) * entity[i].distance) / 5);
          if (jump == 1 || jump == 2) {
            damage = damage/3;
          }
          if (difficulty == 1) {
            damage = damage + damage/4.0;
          }
          else if (difficulty == 2) {
            damage = damage - damage*0.1;
          }
          else {
            damage = damage*0.60;
          }
          if (damage > 0) {
            entity[i].health = max(0, entity[i].health - damage/difficulty);
            entity[i].state = S_HIT;
            entity[i].timer = 2;
          }
        }
      }
    }
  } 
  
  updateHud();
}

// Update coords if possible. Return the collided uid, if any
UID updatePosition(const uint8_t level[], Coords *pos, double relative_x, double relative_y, bool only_walls = false) {
  
  UID collide_x = detectCollision(level, pos, relative_x, 0, only_walls);
  UID collide_y = detectCollision(level, pos, 0, relative_y, only_walls);

  if (!collide_x) pos->x += relative_x;
  if (!collide_y) pos->y += relative_y;

  return collide_x || collide_y || UID_null;
}

void updateEntities(const uint8_t level[]) {
  x = rand() % 4 +1;
  uint8_t i = 0;
  while (i < num_entities) {
    // update distance
    entity[i].distance = coords_distance(&(player.pos), &(entity[i].pos));

    // Run the timer. Works with actual frames.
    // Todo: use delta here. But needs double type and more memory
    if (entity[i].timer > 0) entity[i].timer--;

    // too far away. put it in doze mode
    if (entity[i].distance > MAX_ENTITY_DISTANCE) {
      removeEntity(entity[i].uid);
      // don't increase 'i', since current one has been removed
      continue;
    }
    // bypass render if hidden
    if (entity[i].state == S_HIDDEN) {
      i++;
      continue;
    }

    uint8_t type = uid_get_type(entity[i].uid);

    switch (type) {
      case E_ENEMY: {

          // Enemy "IA"
          if (entity[i].health == 0) {
            if (entity[i].a == false) {
              if (x == 1 ) {
                spawnEntity(E_KEY, entity[i].pos.x, entity[i].pos.y);
                entity[i].a = true;
                enemyCount ++;
                z = 7;
              }
              else if (x == 2 ) {
                spawnEntity(E_KEY, entity[i].pos.x, entity[i].pos.y);
                entity[i].a = true;
                enemyCount ++;
                z = 7;
                
              }
              else if (x == 3 ) {
                spawnEntity(E_MEDIKIT, entity[i].pos.x, entity[i].pos.y);
                entity[i].a = true;
                enemyCount ++;
                z = 7;
              }
              else {
                entity[i].a = true;
                enemyCount ++;
                z = 7;
              }
              if (bss == true) enemyCount2++;
            }
            if (bss == true && enemyCount > 2) {
            }
            if (entity[i].state != S_DEAD) {
              entity[i].state = S_DEAD;
              entity[i].timer = 6;
            }
          } else  if (entity[i].state == S_HIT) {
            if (entity[i].timer == 0) {
              // Back to alert state
              entity[i].state = S_ALERT;
              entity[i].timer = 40;     // delay next fireball thrown
            }
          } else if (entity[i].state == S_FIRING) {
            if (entity[i].timer == 0) {
              // Back to alert state
              entity[i].state = S_ALERT;
              entity[i].timer = 40;
                  // delay next fireball throwm
            }
          } else {
            // ALERT STATE
            if (entity[i].distance > ENEMY_MELEE_DIST && entity[i].distance < MAX_ENEMY_VIEW) {
              if (entity[i].state != S_ALERT) {
                entity[i].state = S_ALERT;
                entity[i].timer = 20;   // used to throw fireballs
              } else {
                if (entity[i].timer == 0) {
                  // Throw a fireball
                  spawnFireball(entity[i].pos.x, entity[i].pos.y);
                  entity[i].state = S_FIRING;
                  entity[i].timer = 6;
                } else {
                  // move towards to the player.
                  updatePosition(
                    level,
                    &(entity[i].pos),
                    sign(player.pos.x, entity[i].pos.x) * ENEMY_SPEED * delta,
                    sign(player.pos.y, entity[i].pos.y) * ENEMY_SPEED * delta,
                    true
                  );
                }
              }

            } else if (entity[i].distance <= ENEMY_MELEE_DIST) {
              if (entity[i].state != S_MELEE) {
                // Preparing the melee attack
                entity[i].state = S_MELEE;
                entity[i].timer = 10;

              } else if (entity[i].timer == 0) {
                // Melee attack;
                if (debug == false) {
                  player.health = max(0, player.health - ENEMY_MELEE_DAMAGE*difficulty);
                }
                entity[i].timer = 14;
                flash_screen = 1;
                updateHud();
              }
            } else {
              // stand
              entity[i].state = S_STAND;

            }
          }
          break;
        }

      case E_FIREBALL: {
          if (entity[i].distance < FIREBALL_COLLIDER_DIST) {
            // Hit the player and disappear
            if (debug == false) {
              player.health = max(0, player.health - ENEMY_FIREBALL_DAMAGE*difficulty);
            }
            flash_screen = 1;
            updateHud();
            removeEntity(entity[i].uid);
            continue; // continue in the loop
          } else {
            // Move. Only collide with walls.
            // Note: using health to store the angle of the movement
            UID collided = updatePosition(
              level,
              &(entity[i].pos),
              cos((double) entity[i].health / FIREBALL_ANGLES * PI) * FIREBALL_SPEED,
              sin((double) entity[i].health / FIREBALL_ANGLES * PI) * FIREBALL_SPEED,
              true
            );

            if (collided) {
              removeEntity(entity[i].uid);
              continue; // continue in the entity check loop
            }
          }
          break;
        }

      case E_MEDIKIT: {
          if (entity[i].distance < ITEM_COLLIDER_DIST && player.health != 100 && jump_height < 14) {
            // pickup
            playSound(medkit_snd, MEDKIT_SND_LEN);
            entity[i].state = S_HIDDEN;
            if (difficulty == 1) {
              player.health = min(100, player.health + 65);
            }
            else if (difficulty == 2) {
              player.health = min(100, player.health + 50);
            }
            else {
              player.health = min(100, player.health + 25);
            }
            updateHud();
            flash_screen = 1;
            z = 3;
            updateHud();
            z = 2;
            updateHud();
          }
          break;
        }

      case E_KEY: {
          if (entity[i].distance < ITEM_COLLIDER_DIST && player.keys < 240 && jump_height < 14) {
            // pickup
            playSound(get_key_snd, GET_KEY_SND_LEN);
            entity[i].state = S_HIDDEN;
            if (difficulty == 1) {
              player.keys = player.keys + 13;
            }
            else if (difficulty == 2) {
              player.keys = player.keys + 10;
            }
            else {
              player.keys = player.keys + 9;
            }
            if (player.keys >240) {
              player.keys = 240;
            }
            updateHud();
            z = 3;
            updateHud();
            z = 1;
            updateHud();
          }
          break;
        }
    }

    i++;
  }
}

// The map raycaster. Based on https://lodev.org/cgtutor/raycasting.html
void renderMap(const uint8_t level[], double view_height) {
  UID last_uid;

  for (uint8_t x = 0; x < SCREEN_WIDTH; x += RES_DIVIDER) {
    double camera_x = 2 * (double) x / SCREEN_WIDTH - 1;
    double ray_x = player.dir.x + player.plane.x * camera_x;
    double ray_y = player.dir.y + player.plane.y * camera_x;
    uint8_t map_x = uint8_t(player.pos.x);
    uint8_t map_y = uint8_t(player.pos.y);
    Coords map_coords = { player.pos.x, player.pos.y };
    double delta_x = abs(1 / ray_x);
    double delta_y = abs(1 / ray_y);

    int8_t step_x; 
    int8_t step_y;
    double side_x;
    double side_y;

    if (ray_x < 0) {
      step_x = -1;
      side_x = (player.pos.x - map_x) * delta_x;
    } else {
      step_x = 1;
      side_x = (map_x + 1.0 - player.pos.x) * delta_x;
    }

    if (ray_y < 0) {
      step_y = -1;
      side_y = (player.pos.y - map_y) * delta_y;
    } else {
      step_y = 1;
      side_y = (map_y + 1.0 - player.pos.y) * delta_y;
    }

    // Wall detection
    uint8_t depth = 0;
    bool hit = 0;
    bool side; 
    bool coll = 0;
    while (!hit && depth < MAX_RENDER_DEPTH) {
      if (side_x < side_y) {
        side_x += delta_x;
        map_x += step_x;
        side = 0;
      } else {
        side_y += delta_y;
        map_y += step_y;
        side = 1;
      }

      uint8_t block = getBlockAt(level, map_x, map_y);

      if (block == E_WALL || block == E_DOOR || block == E_DOOR2 || block == E_DOOR3 || block == E_COLL) {
        hit = 1;
        if (block == E_COLL) coll = 1;
      } else {
        // Spawning entities here, as soon they are visible for the
        // player. Not the best place, but would be a very performance
        // cost scan for them in another loop
        if (block == E_ENEMY || (block & 0b00001000) /* all collectable items */) {
          // Check that it's close to the player
          if (coords_distance(&(player.pos), &map_coords) < MAX_ENTITY_DISTANCE) {
            UID uid = create_uid(block, map_x, map_y);
            if (last_uid != uid && !isSpawned(uid)) {
              spawnEntity(block, map_x, map_y);
              last_uid = uid;
            }
          }
        }
      }

      depth++;
    }

    if (hit) {
      double distance;
      
      if (side == 0) {
        distance = max(1, (map_x - player.pos.x + (1 - step_x) / 2) / ray_x);
      } else {
        distance = max(1, (map_y - player.pos.y + (1 - step_y) / 2) / ray_y);
      }

      // store zbuffer value for the column
      zbuffer[x / Z_RES_DIVIDER] = min(distance * DISTANCE_MULTIPLIER, 255);

      // rendered line height
      uint8_t line_height = RENDER_HEIGHT / distance - 1;

      if (coll == true){
        drawVLine(
          x,
          view_height / distance - line_height / 2 + RENDER_HEIGHT / 2 - 17,
          view_height / distance + line_height / 2 + RENDER_HEIGHT / 2 ,
          GRADIENT_COUNT - int(distance / MAX_RENDER_DEPTH * GRADIENT_COUNT) - side * 2
        );
      }
      else {
        drawVLine(
          x,
          view_height / distance - line_height / 2 + RENDER_HEIGHT / 2,
          view_height / distance + line_height / 2 + RENDER_HEIGHT / 2 ,
          GRADIENT_COUNT - int(distance / MAX_RENDER_DEPTH * GRADIENT_COUNT) - side * 2
        );
      }
    }
  }
}

// Sort entities from far to close
uint8_t sortEntities() {
  uint8_t gap = num_entities;
  bool swapped = false;
  while (gap > 1 || swapped) {
    //shrink factor 1.3
    gap = (gap * 10) / 13;
    if (gap == 9 || gap == 10) gap = 11;
    if (gap < 1) gap = 1;
    swapped = false;
    for (uint8_t i = 0; i < num_entities - gap; i++)
    {
      uint8_t j = i + gap;
      if (entity[i].distance < entity[j].distance)
      {
        swap(entity[i], entity[j]);
        swapped = true;
      }
    }
  }
}

Coords translateIntoView(Coords *pos) {
  //translate sprite position to relative to camera
  double sprite_x = pos->x - player.pos.x;
  double sprite_y = pos->y - player.pos.y;

  //required for correct matrix multiplication
  double inv_det = 1.0 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);
  double transform_x = inv_det * (player.dir.y * sprite_x - player.dir.x * sprite_y);
  double transform_y = inv_det * (- player.plane.y * sprite_x + player.plane.x * sprite_y); // Z in screen

  return { transform_x, transform_y };
}

void renderEntities(double view_height) {
  sortEntities();

  for (uint8_t i = 0; i < num_entities; i++) {
    if (entity[i].state == S_HIDDEN) continue;

    Coords transform = translateIntoView(&(entity[i].pos));

    // don´t render if behind the player or too far away
    if (transform.y <= 0.1 || transform.y > MAX_SPRITE_DEPTH) {
      continue;
    }

    int16_t sprite_screen_x = HALF_WIDTH * (1.0 + transform.x / transform.y);
    int8_t sprite_screen_y = RENDER_HEIGHT / 2 + view_height / transform.y;
    uint8_t type = uid_get_type(entity[i].uid);

    // don´t try to render if outside of screen
    // doing this pre-shortcut due int16 -> int8 conversion makes out-of-screen
    // values fit into the screen space
    if (sprite_screen_x < - HALF_WIDTH || sprite_screen_x > SCREEN_WIDTH + HALF_WIDTH) {
      continue;
    }

    switch (type) {
      case E_ENEMY: {
          uint8_t sprite;
          if (entity[i].state == S_ALERT) {
            // walking
            sprite = int(millis() / 500) % 2;
          } else if (entity[i].state == S_FIRING) {
            // fireball
            sprite = 2;
          } else if (entity[i].state == S_HIT) {
            // hit
            sprite = 3;
          } else if (entity[i].state == S_MELEE) {
            // melee atack
            sprite = entity[i].timer > 10 ? 2 : 1;
          } else if (entity[i].state == S_DEAD) {
            // dying
            sprite = entity[i].timer > 0 ? 3 : 4;

          } else {
            // stand
            sprite = 0;
          }

          drawSprite(
            sprite_screen_x - BMP_IMP_WIDTH * .5 / transform.y,
            sprite_screen_y - 8 / transform.y,
            bmp_imp_bits,
            bmp_imp_mask,
            BMP_IMP_WIDTH,
            BMP_IMP_HEIGHT,
            sprite,
            transform.y

          );
          break;
        }

      case E_FIREBALL: {
          drawSprite(
            sprite_screen_x - BMP_FIREBALL_WIDTH / 2 / transform.y,
            sprite_screen_y - BMP_FIREBALL_HEIGHT / 2 / transform.y,
            bmp_fireball_bits,
            bmp_fireball_mask,
            BMP_FIREBALL_WIDTH,
            BMP_FIREBALL_HEIGHT,
            0,
            transform.y
          );
          break;
        }

      case E_MEDIKIT: {
          drawSprite(
            sprite_screen_x - BMP_ITEMS_WIDTH / 2 / transform.y,
            sprite_screen_y + 5 / transform.y,
            bmp_items_bits,
            bmp_items_mask,
            BMP_ITEMS_WIDTH,
            BMP_ITEMS_HEIGHT,
            0,
            transform.y
          );
          break;
        }

      case E_KEY: {
          drawSprite(
            sprite_screen_x - BMP_ITEMS_WIDTH / 2 / transform.y,
            sprite_screen_y + 5 / transform.y,
            bmp_items_bits,
            bmp_items_mask,
            BMP_ITEMS_WIDTH,
            BMP_ITEMS_HEIGHT,
            1,
            transform.y
          );
          break;
        }
    }
  }
}

void renderGun(uint8_t gun_pos, double amount_jogging, bool gun_fired, uint8_t r1) {
  // jogging
  char x = 48 + sin((double) millis() * JOGGING_SPEED) * 10 * amount_jogging - 9;
  char y = RENDER_HEIGHT - gun_pos + abs(cos((double) millis() * JOGGING_SPEED)) * 8 * amount_jogging - 3;
  uint8_t clip_height = max(0, min(y + BMP_GUN_HEIGHT, RENDER_HEIGHT) - y);
  if (gun_pos > GUN_SHOT_POS - 2) {
    // Gun fire
    if (player.keys > 0 && gun_fired == true) {
      display.drawBitmap(x + 14, y - 11, bmp_fire_bits, BMP_FIRE_WIDTH, BMP_FIRE_HEIGHT, 1);
    }
  }
  if (r1 == 1) {
    clip_height = max(0, min(y + BMP_RE1_HEIGHT, RENDER_HEIGHT) - y + 22);
    display.drawBitmap(x-10, y-22, bmp_re1_mask, BMP_RE1_WIDTH, clip_height, 0);
    display.drawBitmap(x-10, y-22, bmp_re1_bits, BMP_RE1_WIDTH, clip_height, 1);
  }
  else if (r1 == 2) {
    clip_height = max(0, min(y + BMP_RE2_HEIGHT, RENDER_HEIGHT) - y + 22);
    display.drawBitmap(x-10, y-22, bmp_re2_mask, BMP_RE2_WIDTH, clip_height, 0);
    display.drawBitmap(x-10, y-22, bmp_re2_bits, BMP_RE2_WIDTH, clip_height, 1);
  }
  else if (r1 == 0) {
    display.drawBitmap(x, y, bmp_gun_mask, BMP_GUN_WIDTH, clip_height, 0);
    display.drawBitmap(x, y, bmp_gun_bits, BMP_GUN_WIDTH, clip_height, 1);
  }
  else {}


  // Don't draw over the hud!
  

  // Draw the gun (black mask + actual sprite).
  

}

// Only needed first time
void renderHud() {
  if (debug == false) {
    drawText(2, 58, F("{}"), 0);        // Health symbol
    drawText(105, 58, F("[]"), 0);       // Keys symbol
    updateHud();
  }
  else {
    drawText(2, 58, F("X"));        // Health symbol
    drawText(105, 58, F("Y"));       // Keys symbol
    updateHud();
  }
}

// Render values for the HUD
void updateHud() {
  display.clearRect(12, 58, 100, 6);
  display.clearRect(50, 58, 15, 6);
  display.clearRect(58, 58, 70, 6);

  if (z == 1) {
    drawText(31, 58, F("FOUND "));
    drawText(65, 58, F(" SHELLS"));
    if (difficulty == 1) {
      drawText(57, 58, F("13"));
    }
    else if (difficulty == 2) {
      drawText(57, 58, F("10"));
    }
    else {
      drawText(60, 58, F("9"));
    }

  }

  else if (z == 2) {
    drawText(31, 58, F("FOUND A MEDKIT"));
  }
  else if (z == 3) {
    display.clearRect(12, 58, 100, 6);
    display.clearRect(1, 58, 100, 6);
    display.clearRect(50, 58, 15, 6);
    display.clearRect(58, 58, 70, 6);
  }
  else if (z == 4) {
    drawText(38, 58, F("GAME OVER"));
  }
  else if (z == 5) {
    drawText(44, 58, F("YOU WIN"));
  }
  else if (z == 6) {
    drawText(33, 58, F("GOAL-20 KILLS"));
  }
  else if (z == 7) {
    if (levelID == true && bss == true) {
      drawText(37, 58, enemyCount2);
      drawText(52, 58, F("OUT OF "));
      drawText(87, 58, enemyGoal2);
    }
    else if (levelID == false) {
      drawText(37, 58, enemyCount);
      drawText(52, 58, F("OUT OF "));
      drawText(87, 58, enemyGoal);
    }
  }
  else if (z == 8) {
    drawText(35, 58, F("SECRET FOUND"));
  }
  else if (z == 9) {
    drawText(31, 58, F("GOAL-FIND EXIT"));
  }
  else if (z == 10) {
    drawText(31, 58, F("DEBUG MODE OFF"));
  }
  else {
    drawText(32, 58, F("DEBUG MODE ON"));
  }
  
  if (debug == false) {
    display.clearRect(1, 58, 8, 6);
    drawText(2, 58, F("{}"), 0);   
    drawText(103, 58, F("[]"), 0);   
    drawText(12, 58, player.health);
    drawText(113, 58, player.keys);
  }
  else {
    display.clearRect(1, 58, 8, 6);
    drawText(2, 58, F("X"));   
    drawText(105, 58, F("Y"));  
    drawText(12, 58, player.pos.x);
    drawText(113, 58, player.pos.y);
  }
}

// Debug stats
void renderStats() {
  
}
void softReset() {
  asm volatile ("jmp 0");
}


void loopMid() {
  display.clearRect(1, 1, 127, 63);
  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif
  if (mid == 1) {
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .14, F("YEAR 2027. HUMANS REACHED"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .23, F("OTHER PLANETS, BUT WE ARE"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .32, F("NOT ALONE, THERE IS ALSO"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .42, F("HOSTILE ALIENS HERE. YOU"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .51, F("ARE AN UNKNOWN MARINE,"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .60, F("WHO FIGHT IN OLD LAB FOR"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .70, F("REMNANTS OF EARTH. RESIST"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .80, F("ALIENS TO ESCAPE."));
  }
  else if (mid == 2){
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .14, F("AFTER KILLING BUNCH OF "));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .23, F("ALIENS, LIGHTS TURNED OFF"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .32, F("AND THE FLOOR COLLAPSED"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .42, F("UNDER YOUR FEET AND YOU "));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .51, F("FELL INTO THE UTILITY"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .60, F("ROOMS. YOU HAVE NO CHOICE"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .70, F("BUT TO START LOOKING FOR "));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .80, F("EXIT, WHILE FIGHT ALIENS."));
    
    levelID = true;
  }

  else {
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .14, F("AFTER HARD FIGHT YOU WENT"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .23, F("TO EXIT. AND AS SOON AS"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .32, F("YOU STEP OUT, AN ALIEN"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .42, F("ATTACKS YOU FROM BEHIND"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .51, F("AND KILLS YOU. YOU DIDNT"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .60, F("EXPECT THIS. YOUR FIGHT"));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .70, F("CAN NOT END LIKE THIS..."));
    drawText(SCREEN_WIDTH / 4.6 - 26, SCREEN_HEIGHT * .80, F("THE END (MAYBE...)"));
  }
  drawText(SCREEN_WIDTH / 2.1 - 24, SCREEN_HEIGHT * .01, F("THE STORY"));
  drawText(SCREEN_WIDTH / 2 - 27, SCREEN_HEIGHT * .91, F("PRESS FIRE"));

  display.display();
  while (!exit_scene) {
    if (input_fire()) {
      fade_e = true;
      if (mid < 3) {
        jumpTo(GAME_PLAY);
      }
      else {
        jumpTo(SCORE);
      }
    }

  };
}


void loopScore() {
  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif
  score = player.keys/2;
  score += player.health;
  score *= 43;
  score *= difficulty;
  if (player.secret != 0){
    score += 69;
  }
  if (player.secret2 += 0){
    score += 69;
  }
  if (player.secret3 += 0){
    score += 69;
  }
  score += k;
  
  display.clearRect(1, 1, 127, 63);

  display.drawBitmap(
    (SCREEN_WIDTH - BMP_LOGO_WIDTH) / 2 - 27,
    (SCREEN_HEIGHT - BMP_LOGO_HEIGHT) / 6,
    bmp_logo_bits,
    BMP_LOGO_WIDTH,
    BMP_LOGO_HEIGHT,
    1
  );
  drawText(SCREEN_WIDTH / 2.36 - 52, SCREEN_HEIGHT * .79, F("NANO BRUTALITY"));
  drawText(SCREEN_WIDTH / 0.99 - 45, SCREEN_HEIGHT * .2, F("YOU WIN"));
  if (player.cheats == false) {
    drawText(SCREEN_WIDTH / 0.99 - 40, SCREEN_HEIGHT * .4, F("SCORE"));
    if (a < score) {
      a+=155;
      drawText(SCREEN_WIDTH / 0.99 - 40, SCREEN_HEIGHT * .5, a);
      playSound(walk1_snd, WALK1_SND_LEN);
    }
    else if (a > score) {
      a = score;
      drawText(SCREEN_WIDTH / 0.99 - 40, SCREEN_HEIGHT * .5, a);
      m = false;
      music = 1;
      playSound(shot_snd, SHOT_SND_LEN);

    }
    else {
      drawText(SCREEN_WIDTH / 0.99 - 40, SCREEN_HEIGHT * .5, a);
      drawText(SCREEN_WIDTH / 0.99 - 52, SCREEN_HEIGHT * .91, F("PRESS FIRE"));
      if (input_fire()) {
        display.clearRect(1, 1, 127, 63);
        display.display();
        music = 99;
        delay(1000);
        fade_e = true;
        softReset();
      }
    }
  }
  else if (player.cheats == true) {
    drawText(SCREEN_WIDTH / 0.99 - 49, SCREEN_HEIGHT * .4, F("NO SCORE"));
    drawText(SCREEN_WIDTH / 0.99 - 37, SCREEN_HEIGHT * .5, F("FOR"));
    drawText(SCREEN_WIDTH / 0.99 - 49, SCREEN_HEIGHT * .6, F("CHEESERS"));
    drawText(SCREEN_WIDTH / 0.99 - 52, SCREEN_HEIGHT * .91, F("PRESS FIRE"));
    if (input_fire()) {
      display.clearRect(1, 1, 127, 63);
      display.display();
      music = 99;
      delay(1000);
      fade_e = true;
      softReset();
    }
    if (mc == false) {
      m = false;
      music = 1;
      playSound(mus_s1_snd, MUS_S1_SND_LEN);
      delay(100);
      mc = true;
    }
  }
  display.display();

  fade_e = false;
  jumpTo(SCORE);
}




// Intro screen
void loopIntro() {
  music = 1;
  
  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif

  display.drawBitmap(
    (SCREEN_WIDTH - BMP_LOGO_WIDTH) / 2,
    (SCREEN_HEIGHT - BMP_LOGO_HEIGHT) / 6,
    bmp_logo_bits,
    BMP_LOGO_WIDTH,
    BMP_LOGO_HEIGHT,
    1
  );

  delay(100);
  drawText(SCREEN_WIDTH / 2.36 - 25, SCREEN_HEIGHT * .79, F("NANO BRUTALITY"));
  drawText(SCREEN_WIDTH / 4.6 - 25, SCREEN_HEIGHT * .3, F("PRESS"));
  drawText(SCREEN_WIDTH / 0.99 - 25, SCREEN_HEIGHT * .3, F("FIRE"));
  drawText(SCREEN_WIDTH / 4.6 - 25, SCREEN_HEIGHT * .91, F("V 1.4"));
  display.display();
  playSound(mus_s1_snd, MUS_S1_SND_LEN);
  while (!exit_scene) {
    if (input_fire()) jumpTo(DIFF);
  };

  // wait for fire
  
}

void loopDiff() {
  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif
  delay(200);


  display.clearRect(1, 1, 127, 63);

  drawText(SCREEN_WIDTH / 2.66 - 25, SCREEN_HEIGHT * .05, F("CHOOSE SKILL LEVEL"));

  drawText(SCREEN_WIDTH / 2.75 - 25, SCREEN_HEIGHT * .3, F("I"));
  drawText(SCREEN_WIDTH / 2.4 - 25, SCREEN_HEIGHT * .3, F("M TOO YOUNG TO DIE."));
  drawText(SCREEN_WIDTH / 2.6 - 25, SCREEN_HEIGHT * .26, F(","));


  drawText(SCREEN_WIDTH / 2.66 - 25, SCREEN_HEIGHT * .43, F("HURT ME PLENTY."));

  drawText(SCREEN_WIDTH / 2.66 - 25, SCREEN_HEIGHT * .56, F("NIGHTMARE."));

  drawText(SCREEN_WIDTH / 3.32 - 25, SCREEN_HEIGHT * .77, F("NOTE - BUTTONS OUTSIDE"));
  drawText(SCREEN_WIDTH / 4.1 - 25, SCREEN_HEIGHT * .9, F("GAMEPLAY WORK AS U THINK"));

  if (difficulty == 2) {
    drawText(SCREEN_WIDTH / 3 - 25, SCREEN_HEIGHT * .43, F("#"));
  }
  else if (difficulty == 1) {
    drawText(SCREEN_WIDTH / 3 - 25, SCREEN_HEIGHT * .3, F("#"));
  }
  else {
    drawText(SCREEN_WIDTH / 3 - 25, SCREEN_HEIGHT * .56, F("#"));
  }

  display.display();
  while (!exit_scene) {
    if (input_down()) {
      difficulty++;
      if (difficulty == 4) {
        difficulty = 1;
      }
      fade_e = false;
      jumpTo(DIFF);
    }
    else if (input_up()) {
      difficulty--;
      if (difficulty == 0) {
        difficulty = 3;
      }
      fade_e = false;
      jumpTo(DIFF);
    }
    else if (input_fire()) {
      fade_e = true;
      jumpTo(MUS);
    }
  };
    
  display.display();

}

void loopMus() {
  #ifdef SNES_CONTROLLER
  getControllerData();
  #endif
  fade_e = false;
  delay(200);


  display.clearRect(1, 1, 127, 63);

  drawText(SCREEN_WIDTH / 2.75 - 25, SCREEN_HEIGHT * .25, F("MUSIC"));

  drawText(SCREEN_WIDTH / 2.66 - 25, SCREEN_HEIGHT * .39, F("OFF"));

  drawText(SCREEN_WIDTH / 2.66 - 25, SCREEN_HEIGHT * .50, F("ON (NOT RECOMENDED)"));


  if (m == false) {
    drawText(SCREEN_WIDTH / 3 - 25, SCREEN_HEIGHT * .50, F("#"));
  }
  else if (m == true) {
    drawText(SCREEN_WIDTH / 3 - 25, SCREEN_HEIGHT * .39, F("#"));
  }

  display.display();
  while (!exit_scene) {
    if (input_down() || input_up()) {
      m = !m;
      jumpTo(MUS);
    }
    else if (input_fire()) {
      fade_e = true;
      jumpTo(MID);
    }
  };
    
  display.display();

}



unsigned long last_time;
void loopGamePlay() {

  bool gun_fired = false;
  bool walkSoundToggle = false;
  uint8_t gun_pos = 0;
  double rot_speed;
  double old_dir_x;
  double old_plane_x;
  double view_height;
  double jogging;
  uint8_t fade = GRADIENT_COUNT - 1;


  mc = false;
  if (levelID == false) {
    initializeLevel(E1M1);
  }
  if (levelID == true) {
    initializeLevel(E1M2);
  }

  
  do {
    fps();


    if (player.keys == 0) {
      coll = false;
    }
    else if (player.keys != 0) {
      coll = true;
    }

    if (levelID == false) {
      k = player.keys;
    }

    display.clearRect(1, 58, 100, 2);
    
    updateHud();

    // Clear only the 3d view
    memset(display_buf, 0, SCREEN_WIDTH * (RENDER_HEIGHT / 8));

    #ifdef SNES_CONTROLLER
    getControllerData();
    #endif
    
    if (player.pos.x >= 2 && player.pos.x <= 3 && player.pos.y >= 54 && player.pos.y <= 55 && z == 1 && player.secret <2) {
      spawnEntity(E_ENEMY, 1, 51);
      spawnEntity(E_ENEMY, 3, 51);
      player.secret++;
    }
    
    if (player.pos.x >= 46 && player.pos.x <= 47 && player.pos.y >= 35 && player.pos.y <= 36 && levelID == true) {
      player.pos.x = 12.5;
      player.pos.y = 33.5;
      enemyCount = 0;
      spawnEntity(E_ENEMY, 10, 38);
      spawnEntity(E_ENEMY, 13, 38);
      bss = true;
    }
    if (player.pos.y >= 55 && player.pos.y <=56 && player.pos.x >= 12 && player.pos.x <= 23 && levelID == true) {
      mid = 3;
      m = false;
      playSound(mus_s1_snd, MUS_S1_SND_LEN);
      jumpTo(MID);
    }
    if (levelID == true && bss == true) {
      if (enemyCount == 1 || enemyCount == 5 || enemyCount == 9 ) {
        clearEntities();
        enemyCount++;
        spawnEntity(E_ENEMY, 13, 38);
        
      }
      else if (enemyCount == 3 || enemyCount == 7 || enemyCount == 11) {
        clearEntities();
        enemyCount++;
        spawnEntity(E_ENEMY, 10, 38);
      }
      else if (enemyCount == 13) {
        player.pos.y = player.pos.y + 12;
        enemyCount = 0;
        enemyCount2 = 8;
        updateHud();
      }
    }
    
    if (m == true) {
      music = 99;
    }

    // If the player is alive
    if (player.health > 0) {
      
      if (jump == 1 || jump == 2) {
        if (jump_height > 0 && jump == 2){
          view_height -= 4; 
          jump_height -= 4;
        }
        else if (jump_height < 20 && jump == 1) {
          view_height += 4;
          jump_height += 4; 
        }
        else if (jump_height == 20) jump = 2;
        else if (jump_height == 0) jump = 0;

        vel = 2;
      }
      if (jump == 0) {
        view_height = abs(sin((double) millis() * JOGGING_SPEED)) * 6 * jogging;
        vel = 1;
      }
      
      
      // Player speed
      if (input_up()) {

        player.velocity += (MOV_SPEED - player.velocity) * .4;
        if (jump == 1 || jump == 2) {
          jogging = 0;
          gun_pos = 22;
        }
        else {
          jogging = abs(player.velocity) * MOV_SPEED_INV * 2;
        }

      } else if (input_down()) {

        jogging = abs(player.velocity) * MOV_SPEED_INV * 2;
        player.velocity += (- MOV_SPEED - player.velocity) * .4;
        if (jump == 1 || jump == 2) {
          jogging = 0;
          gun_pos = 22;
        }
        else {
          jogging = abs(player.velocity) * MOV_SPEED_INV * 2;
        }

      } else {
        if (jump == 1 || jump == 2) {
          jogging = 0;
          gun_pos = 22;
        }
        else {
          jogging = abs(player.velocity) * MOV_SPEED_INV * 2;
        }
        player.velocity *= .5;
      }

      // Player rotation
      if (input_right()) {
        

        rot_speed = ROT_SPEED * delta;
        old_dir_x = player.dir.x;
        player.dir.x = player.dir.x * cos(-rot_speed) - player.dir.y * sin(-rot_speed);
        player.dir.y = old_dir_x * sin(-rot_speed) + player.dir.y * cos(-rot_speed);
        old_plane_x = player.plane.x;
        player.plane.x = player.plane.x * cos(-rot_speed) - player.plane.y * sin(-rot_speed);
        player.plane.y = old_plane_x * sin(-rot_speed) + player.plane.y * cos(-rot_speed);
      } else if (input_left()) {

        rot_speed = ROT_SPEED * delta;
        old_dir_x = player.dir.x;
        player.dir.x = player.dir.x * cos(rot_speed) - player.dir.y * sin(rot_speed);
        player.dir.y = old_dir_x * sin(rot_speed) + player.dir.y * cos(rot_speed);
        old_plane_x = player.plane.x;
        player.plane.x = player.plane.x * cos(rot_speed) - player.plane.y * sin(rot_speed);
        player.plane.y = old_plane_x * sin(rot_speed) + player.plane.y * cos(rot_speed);
      }


      


      if (input_left() && input_right() && jump==0) {
        jump = 1;
        playSound(jump_snd, JUMP_SND_LEN);
      }

      if(view_height > 2.95 && jump == 0) {
        if(sound == false) {
          if(walkSoundToggle) {
            playSound(walk1_snd, WALK1_SND_LEN);
            walkSoundToggle = false;
          } else {
            playSound(walk2_snd, WALK2_SND_LEN);
            walkSoundToggle = true;
          }
        }
      }
      // Update gun
      if (gun_pos > GUN_TARGET_POS) {
        // Right after fire
        gun_pos -= 2;
      } else if (gun_pos < GUN_TARGET_POS) {
        // Showing up
        gun_pos += 2;
      } else if (!gun_fired && input_fire() && player.keys>0 && reload1 == false) {
        // ready to fire and fire pressed
        gun_pos = GUN_SHOT_POS;
        gun_fired = true;
        fire();
        if (debug == false) {
          player.keys--;
        }
      } else if (gun_fired && !input_fire()) {
        // just fired and restored position
        gun_fired = false;
        reload1 = true;
      } else if (!gun_fired && input_fire() && player.keys==0 && reload1 == false) {
        gun_pos = GUN_SHOT_POS;
        gun_fired = true;
        fire();
      }

    } else {
      // The player is dead
      z = 3;
      updateHud();      
      z = 4;
      updateHud();

      if (view_height > -5) view_height--; 
      
      
      else if (input_fire()) {
        jumpTo(INTRO);
      }
      if (gun_pos > 0) gun_pos -= 2;
      else {
        rc1 = 3;
      }
       
    }

    if (abs(player.velocity) > 0.003) {

      if (levelID == false) {
        updatePosition(
          E1M1,
          &(player.pos),
          player.dir.x * player.velocity * delta * vel,
          player.dir.y * player.velocity * delta * vel
          );
      }
      if (levelID == true) {
        updatePosition(
          E1M2,
          &(player.pos),
          player.dir.x * player.velocity * delta * vel,
          player.dir.y * player.velocity * delta * vel
          );
      }
    } else {
      player.velocity = 0;
    }





    if (enemyCount == enemyGoal && levelID == false) {
      z = 3;
      updateHud();
      z = 5;
      updateHud();
      if (del == 0) {
        delay(200);
        del++;
      }
      if (input_fire()){
        player.pos.x = 230;
        player.pos.y = 50;
        mid = 2;
        enemyCount = 0;
        z = 3;
        updateHud();
        jumpTo(MID);
      }
    }

    // Update things
    if (levelID == false) {
      updateEntities(E1M1);
    }
    if (levelID == true) {
      updateEntities(E1M2);
    }
    
    updateHud();


    // Render stuff
    

    if (levelID == false) {
      renderMap(E1M1, view_height);
    }
    else if (levelID == true) {
      renderMap(E1M2, view_height);
    }

    renderEntities(view_height);

    if (reload1 == true ) {
      r++;
    }

    if (coll == false) {
      r=7;
    }

    if (r == 1) {
      rc1 = 1;
    }
    else if (r == 3) {
      rc1 = 2;
      playSound(r1_snd, R1_SND_LEN);
    }
    
    else if (r == 5) {
      rc1 = 1;
      playSound(r2_snd, R2_SND_LEN);
    }
    else if (r == 7) {
      r = 0;
      reload1 = false;
      rc1 = 0;
    }


    renderGun(gun_pos, jogging, gun_fired, rc1);

    // Fade in effect
    if (fade > 0) {
      fadeScreen(fade);
      fade--;

      if (fade == 0) {
        // Only draw the hud after fade in effect
        renderHud();
      }
    } else {
      renderStats();
    }

    

    // flash screen
    if (flash_screen > 0) {
      invert_screen = !invert_screen;
      flash_screen--;
    } else if (invert_screen) {
      invert_screen = 0;
    }
    updateHud();

    // Draw the frame
    display.invertDisplay(invert_screen);
    display.display();

    // Exit routine
    #ifdef SNES_CONTROLLER
    if (input_start()) {
    #else
    if (input_left() && input_right() && input_up() && input_down() && input_fire()) {
    #endif
      z = 3;
      updateHud();
      if (debug == true) {
        z = 3;
        updateHud();
        z = 10;
        debug = false;
        updateHud();
      }
      else {
        z = 3;
        updateHud();
        z = 11;
        debug = true;
        updateHud();
        player.cheats = true;
      }
      updateHud();
      delay(500);
    }

  } while (!exit_scene);

  



}

void loop(void) {
  
  switch (scene) {
    case INTRO: {
        
        loopIntro();
        break;
      }
    case SCORE: {
        loopScore();
        break;
      }
    case MUS: {
        
        loopMus();
        break;
      }
    case DIFF: {
        loopDiff();
        break;
      }
    case GAME_PLAY: {

        loopGamePlay();
    
        break;
        
      }
    case MID: {
        loopMid();

        break;
      }
  }

  if (fade_e == true) {// fade out effect
    for (uint8_t i=0; i<GRADIENT_COUNT; i++) {
      fadeScreen(i, 0);
      display.display();
      delay(40);
    }
  }
  exit_scene = false;






 // Stop (so it doesn't repeat forever driving you crazy--you're welcome).

}
