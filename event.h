#ifndef EVENT_H
# define EVENT_H

# include <stdint.h>

# include "dungeon.h"

typedef enum eventype {
  event_character_turn,
} eventype_t;

struct event {
  eventype_t type;
  uint32_t time;
  uint32_t sequence;
  union {
    character *c;
  };
};

int32_t compare_events(const void *event1, const void *event2);
event *new_event(dungeon *d, eventype_t t, void *v, uint32_t delay);
event *update_event(dungeon *d, event *e, uint32_t delay);
void event_delete(void *e);

#endif
