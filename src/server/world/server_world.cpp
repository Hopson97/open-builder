#include "server_world.h"

#include <cassert>

ServerWorld::ServerWorld()
{
    m_entities.resize(1024);
}

void ServerWorld::tick()
{
    /*
    for (int i = 1; i < m_entities.size(); i++) {
        EntityState& state = m_entities[i];
        if (state.active) {
        }
    }
    */
}

const std::vector<EntityState>& ServerWorld::getEntities() const
{
    return m_entities;
}

void ServerWorld::serialiseEntities(ServerPacket& packet) const
{
    packet.write(entityCount);
    for (int i = 1; i < m_entities.size(); i++) {
        const auto& state = m_entities[i];
        if (state.active) {
            packet.write(static_cast<u32>(i));
            packet.write(state.position);
            packet.write(state.rotation);
        }
    }
}

u32 ServerWorld::addEntity()
{
    for (int i = 1; i < m_entities.size(); i++) {
        auto& state = m_entities[i];
        if (!state.active) {
            entityCount++;
            state.active = true;
            return i;
        }
    }
    return 0;
}

void ServerWorld::removeEntity(u32 id)
{
    assert(id < m_entities.size());
    m_entities[id].active = false;
    entityCount--;
}

EntityState& ServerWorld::findEntity(u32 id)
{
    assert(id < m_entities.size());
    return m_entities[id];
}
