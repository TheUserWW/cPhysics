//
// Created by wcx16 on 2026/2/23.
//

#ifndef CPHYSICS_ENTITY_MANAGER_H
#define CPHYSICS_ENTITY_MANAGER_H

#ifdef __cplusplus
#include "entity.h"
#include <vector>
#include <string>

class EntityManager {
private:
    std::vector<Entity*> entities;
    
public:
    EntityManager();
    ~EntityManager();
    
    // 添加实体
    void addEntity(Entity* entity);
    void addEntity(const std::string& name, double mass, double charge, 
                   const Vector& position, const Vector& velocity,
                   double radius = 0.5, bool is_static = false);
    
    // 移除实体
    bool removeEntity(const std::string& name);
    bool removeEntity(Entity* entity);
    void clearAll();
    
    // 查找实体
    Entity* findEntity(const std::string& name);
    Entity* getEntity(size_t index);
    
    // 获取实体数量
    size_t getEntityCount() const;
    
    // 批量操作
    void updateAllPhysics(double delta_time);
    void applyGravityField(double magnitude, const Vector& direction);
    void applyElectricField(double magnitude, const Vector& direction);
    void applyMagneticField(double magnitude, const Vector& direction);
    
    // 碰撞检测
    void checkCollisions();
    
    // 序列化/反序列化
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    
    // 迭代器支持
    std::vector<Entity*>::iterator begin();
    std::vector<Entity*>::iterator end();
    std::vector<Entity*>::const_iterator begin() const;
    std::vector<Entity*>::const_iterator end() const;
};

// C接口（用于C代码调用）
extern "C" {
#endif

    // C接口函数声明
    typedef struct EntityManager_C EntityManager_C;
    
    EntityManager_C* entity_manager_create();
    void entity_manager_destroy(EntityManager_C* manager);
    void entity_manager_add_entity(EntityManager_C* manager, Entity* entity);
    size_t entity_manager_get_count(EntityManager_C* manager);
    Entity* entity_manager_get_entity(EntityManager_C* manager, size_t index);
    void entity_manager_update_physics(EntityManager_C* manager, double delta_time);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_ENTITY_MANAGER_H