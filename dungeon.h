#ifndef DUNGEON_H
# define DUNGEON_H

# include <vector>

# include "heap.h"
# include "dims.h"
# include "character.h"
# include "descriptions.h"

#define DUNGEON_X              80
#define DUNGEON_Y              21
#define MIN_ROOMS              6
#define MAX_ROOMS              10
#define ROOM_MIN_X             4
#define ROOM_MIN_Y             3
#define ROOM_MAX_X             20
#define ROOM_MAX_Y             15
#define PC_VISUAL_RANGE        3
#define NPC_VISUAL_RANGE       15
#define PC_SPEED               10
#define NPC_MIN_SPEED          5
#define NPC_MAX_SPEED          20
#define MAX_MONSTERS           15
#define MAX_OBJECTS           15
#define SAVE_DIR               ".rlg327"
#define DUNGEON_SAVE_FILE      "dungeon"
#define DUNGEON_SAVE_SEMANTIC  "RLG327-" TERM
#define DUNGEON_SAVE_VERSION   0U
#define MONSTER_DESC_FILE      "monster_desc.txt"
#define OBJECT_DESC_FILE       "object_desc.txt"
#define MAX_INVENTORY          10

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])
#define charpair(pair) (d->character_map[pair[dim_y]][pair[dim_x]])
#define charxy(x, y) (d->character_map[y][x])
#define objpair(pair) (d->objmap[pair[dim_y]][pair[dim_x]])
#define objxy(x, y) (d->objmap[y][x])

enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_unknown,
  ter_wall,
  ter_wall_immutable,
  ter_floor,
  ter_floor_room,
  ter_floor_hall,
  ter_stairs,
  ter_stairs_up,
  ter_stairs_down
};

typedef struct room {
  pair_t position;
  pair_t size;
} room_t;

class pc;
class object;

class dungeon {
 public:
  dungeon() : num_rooms(0), rooms(0), map{ter_wall}, hardness{0},
              pc_distance{0}, pc_tunnel{0}, character_map{0}, PC(0),
              num_monsters(0), max_monsters(0), character_sequence_number(0),
              time(0), is_new(0), quit(0), monster_descriptions(),
              object_descriptions() {}
  uint32_t num_rooms;
  room_t *rooms;
  terrain_type map[DUNGEON_Y][DUNGEON_X];
  /* Since hardness is usually not used, it would be expensive to pull it *
   * into cache every time we need a map cell, so we store it in a        *
   * parallel array, rather than using a structure to represent the       *
   * cells.  We may want a cell structure later, but from a performanace  *
   * perspective, it would be a bad idea to ever have the map be part of  *
   * that structure.  Pathfinding will require efficient use of the map,  *
   * and pulling in unnecessary data with each map cell would add a lot   *
   * of overhead to the memory system.                                    */
  uint8_t hardness[DUNGEON_Y][DUNGEON_X];
  uint8_t pc_distance[DUNGEON_Y][DUNGEON_X];
  uint8_t pc_tunnel[DUNGEON_Y][DUNGEON_X];
  character *character_map[DUNGEON_Y][DUNGEON_X];
  object *objmap[DUNGEON_Y][DUNGEON_X];
  pc *PC;
  heap_t events;
  uint16_t num_monsters;
  uint16_t max_monsters;
  uint16_t num_objects;
  uint16_t max_objects;
   uint32_t character_sequence_number;
  /* Game time isn't strictly necessary.  It's implicit in the turn number *
   * of the most recent thing removed from the event queue; however,       *
   * including it here--and keeping it up to date--provides a measure of   *
   * convenience, e.g., the ability to create a new event without explicit *
   * information from the current event.                                   */
  uint32_t time;
  uint32_t is_new;
  uint32_t quit;
  std::vector<monster_description> monster_descriptions;
  std::vector<object_description> object_descriptions;
};

void init_dungeon(dungeon *d);
void new_dungeon(dungeon *d);
void delete_dungeon(dungeon *d);
int gen_dungeon(dungeon *d);
void render_dungeon(dungeon *d);
int write_dungeon(dungeon *d, char *file);
int read_dungeon(dungeon *d, char *file);
int read_pgm(dungeon *d, char *pgm);
void render_distance_map(dungeon *d);
void render_tunnel_distance_map(dungeon *d);
void init_dungeon(dungeon *d);
void pc_see_object(character *the_pc, object *o);

#endif
