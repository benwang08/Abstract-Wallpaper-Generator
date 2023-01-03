#include "entity_list.h"

bool entity_list::hit(ray r, double t_min, double t_max, hit_record &rec){
    bool found_hit = false;
    hit_record h_record;

    for (auto& ent: objects){
        if (ent->hit(r, t_min, t_max, h_record)){
            found_hit = true;
            t_max = h_record.t;
            rec = h_record;
        }
    }

    return found_hit;
}
