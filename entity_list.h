#ifndef entity_list_h
#define entity_list_h

#include "entity.h"

#include <memory>
#include <vector>

using namespace std;

class entity_list : public entity {
    public:
        entity_list() {}
        entity_list(shared_ptr<entity> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<entity> object) { objects.push_back(object); }

        bool hit(ray r, double t_min, double t_max, hit_record &rec) override;

    public:
        std::vector<shared_ptr<entity>> objects;
};



#endif