#include <cstdlib>

#include "event.h"
#include "character.h"

static uint32_t next_event_number(void)
{
  static uint32_t sequence_number;

  /* We need to special case the first PC insert, because monsters go *
   * into the queue before the PC.  Pre-increment ensures that this   *
   * starts at 1, so we can use a zero there.                         */
  return ++sequence_number;
}

int32_t compare_events(const void *event1, const void *event2)
{
  int32_t difference;

  difference = (((event *) event1)->time -
                ((event *) event2)->time);
  return difference ? difference : (((event *) event1)->sequence -
                                    ((event *) event2)->sequence);

}

event *new_event(dungeon *d, eventype_t t, void *v, uint32_t delay)
{
  event *e;

  e = (event *) malloc(sizeof (*e));

  e->type = t;
  e->time = d->time + delay;
  e->sequence = next_event_number();
  switch (t) {
  case event_character_turn:
    e->c = (character *) v;
  }

  return e;
}

event *update_event(dungeon *d, event *e, uint32_t delay)
{
  e->time = d->time + delay;
  e->sequence = next_event_number();

  return e;
}

void event_delete(void *v)
{
  event *e = (event *) v;

  switch (e->type) {
  case event_character_turn:
    character_delete(e->c);
    break;
  }

  free(e);
}
