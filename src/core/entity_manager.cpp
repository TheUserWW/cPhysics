//
// Created by wcx16 on 2026/2/23.
//

#include "core/entity_manager.h"
#include "core/field.h"
#include "core/movement.h"
#include "../../include/core/collider.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstring>

#ifdef __cplusplus

// EntityManager实现
EntityManager::EntityManager() {
    entities.reserve(100); // 预分配空间
}

EntityManager::~EntityManager() {
    clearAll();
}

void EntityManager::addEntity(Entity* entity) {
    if (entity) {
        entities.emplace_back(entity);
    }
}

void EntityManager::addEntity(const std::string& name, double mass, double charge, 
                              const Vector& position, const Vector& velocity,
                              double radius, bool is_static) {
    Entity* entity = new Entity();
    std::strncpy(entity->name, name.c_str(), 255);
    entity->name[255] = '\0';
    entity->mass = mass;
    entity->charge = charge;
    entity->position = position;
    entity->velocity = velocity;
    entity->acceleration = {0, 0, 0};
    entity->is_static = is_static;
    entity->coefficient_of_restitution = radius;
    
    entities.emplace_back(entity);
}


bool EntityManager::removeEntity(const std::string& name) {
    auto it = std::find_if(entities.begin(), entities.end(), 
        [&](Entity* entity) { return std::strcmp(entity->name, name.c_str()) == 0; });
    
    if (it != entities.end()) {
        delete *it;
        entities.erase(it);
        return true;
    }
    return false;
}

bool EntityManager::removeEntity(Entity* entity) {
    auto it = std::find(entities.begin(), entities.end(), entity);
    if (it != entities.end()) {
        delete *it;
        entities.erase(it);
        return true;
    }
    return false;
}

void EntityManager::clearAll() {
    for (Entity* entity : entities) {
        delete entity;
    }
    entities.clear();
}

Entity* EntityManager::findEntity(const std::string& name) {
    auto it = std::find_if(entities.begin(), entities.end(), 
        [&](Entity* entity) { return std::strcmp(entity->name, name.c_str()) == 0; });
    return (it != entities.end()) ? *it : nullptr;
}

Entity* EntityManager::getEntity(size_t index) {
    return (index < entities.size()) ? entities[index] : nullptr;
}

size_t EntityManager::getEntityCount() const {
    return entities.size();
}

void EntityManager::updateAllPhysics(double delta_time) {
    for (size_t i = 0; i < entities.size(); i++) {
        for (size_t j = i + 1; j < entities.size(); j++) {
            apply_universal_gravitation(entities[i], entities[j]);
        }
    }

    for (Entity* entity : entities) {
        if (!entity->is_static) {
            // 更新速度：v = v0 + a * dt
            entity->velocity.x += entity->acceleration.x * delta_time;
            entity->velocity.y += entity->acceleration.y * delta_time;
            entity->velocity.z += entity->acceleration.z * delta_time;
            
            // 更新位置：s = s0 + v * dt
            entity->position.x += entity->velocity.x * delta_time;
            entity->position.y += entity->velocity.y * delta_time;
            entity->position.z += entity->velocity.z * delta_time;
            
            // 更新旋转
            update_rotation(entity, delta_time);
        }
    }
    
    // 检查碰撞
    checkCollisions();
    
    // 重置所有实体的加速度（为下一帧准备）
    for (Entity* entity : entities) {
        if (!entity->is_static) {
            entity->acceleration = {0, 0, 0};
        }
    }
}

void EntityManager::applyGravityField(double magnitude, const Vector& direction) {
    gravitational_field gravity = {magnitude, direction};
    for (Entity* entity : entities) {
        if (!entity->is_static) {
            apply_gravitational_field(entity, &gravity);
        }
    }
}

void EntityManager::applyElectricField(double magnitude, const Vector& direction) {
    electric_field e_field = {magnitude, direction};
    for (Entity* entity : entities) {
        if (!entity->is_static && fabs(entity->charge) > 1e-10) {
            apply_electric_field(entity, &e_field);
        }
    }
}

void EntityManager::applyMagneticField(double magnitude, const Vector& direction) {
    magnetic_field b_field = {magnitude, direction, {0, 0, 0}};
    for (Entity* entity : entities) {
        if (!entity->is_static && fabs(entity->charge) > 1e-10) {
            apply_magnetic_field(entity, &b_field);
        }
    }
}

void EntityManager::checkCollisions() {
    for (size_t i = 0; i < entities.size(); i++) {
        for (size_t j = i + 1; j < entities.size(); j++) {
            double dist = get_euclidean_distance(entities[i], entities[j]);
            double r1 = get_entity_radius(entities[i]);
            double r2 = get_entity_radius(entities[j]);
            double threshold = r1 + r2;
            
            static int frame_count = 0;
            frame_count++;
            if (frame_count % 1000 == 0) {
                printf("[DEBUG] Distance: %.6f, r1: %.6f, r2: %.6f, threshold: %.6f, colliding: %d\n",
                       dist, r1, r2, threshold, dist < threshold);
            }
            
            if (check_sphere_collision(entities[i], entities[j])) {
                printf("[COLLISION] Detected between %s and %s!\n", 
                       entities[i]->name, entities[j]->name);
                resolve_sphere_collision(entities[i], entities[j]);
            }
        }
    }
}

bool EntityManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << entities.size() << std::endl;
    for (Entity* entity : entities) {
        file << entity->name << " " 
             << entity->mass << " " 
             << entity->charge << " "
             << entity->position.x << " " << entity->position.y << " " << entity->position.z << " "
             << entity->velocity.x << " " << entity->velocity.y << " " << entity->velocity.z << " "
             << entity->is_static << " "
             << entity->coefficient_of_restitution << std::endl;
    }
    
    file.close();
    return true;
}

bool EntityManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    clearAll();
    
    size_t count;
    file >> count;
    
    for (size_t i = 0; i < count; i++) {
        Entity* entity = new Entity();
        file >> entity->name
             >> entity->mass
             >> entity->charge
             >> entity->position.x >> entity->position.y >> entity->position.z
             >> entity->velocity.x >> entity->velocity.y >> entity->velocity.z
             >> entity->is_static
             >> entity->coefficient_of_restitution;
        
        entity->acceleration = {0, 0, 0};
        entities.push_back(entity);
    }
    
    file.close();
    return true;
}

// 迭代器实现
std::vector<Entity*>::iterator EntityManager::begin() {
    return entities.begin();
}

std::vector<Entity*>::iterator EntityManager::end() {
    return entities.end();
}

std::vector<Entity*>::const_iterator EntityManager::begin() const {
    return entities.begin();
}

std::vector<Entity*>::const_iterator EntityManager::end() const {
    return entities.end();
}

// C接口实现
struct EntityManager_C {
    EntityManager* manager;
};

extern "C" {

EntityManager_C* entity_manager_create() {
    EntityManager_C* wrapper = new EntityManager_C();
    wrapper->manager = new EntityManager();
    return wrapper;
}

void entity_manager_destroy(EntityManager_C* manager) {
    if (manager) {
        delete manager->manager;
        delete manager;
    }
}

void entity_manager_add_entity(EntityManager_C* manager, Entity* entity) {
    if (manager && manager->manager) {
        manager->manager->addEntity(entity);
    }
}

size_t entity_manager_get_count(EntityManager_C* manager) {
    return (manager && manager->manager) ? manager->manager->getEntityCount() : 0;
}

Entity* entity_manager_get_entity(EntityManager_C* manager, size_t index) {
    return (manager && manager->manager) ? manager->manager->getEntity(index) : nullptr;
}

void entity_manager_update_physics(EntityManager_C* manager, double delta_time) {
    if (manager && manager->manager) {
        manager->manager->updateAllPhysics(delta_time);
    }
}

}

#endif
