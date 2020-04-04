#pragma once

#include <common/world/chunk_manager.h>
#include <common/world/entity_state.h>
#include <common/network/packet.h>

class ServerWorld {
  public:
    ServerWorld();

    void tick();

    const std::vector<EntityState>& getEntities() const;
    void serialiseEntities(ServerPacket& packet, u32 omit) const;

    u32 addEntity();
    void removeEntity(u32 id);
    EntityState& findEntity(u32 id);

  private:
    std::vector<EntityState> m_entities;
    u32 entityCount = 0;
    ChunkManager m_chunks;
};