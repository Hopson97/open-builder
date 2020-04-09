#include "packet.h"

#include "../world/voxel_data.h"

sf::Packet& operator>>(sf::Packet& packet, glm::vec3& vect)
{
    packet >> vect.x >> vect.y >> vect.z;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const glm::vec3& vect)
{
    packet << vect.x << vect.y << vect.z;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, VoxelData& voxel)
{
    u8 meshStyle = 0;
    u8 type = 0;
    u8 isCollidable = 0;

    packet >> voxel.name;
    packet >> voxel.topTexture;
    packet >> voxel.sideTexture;
    packet >> voxel.bottomTexture;
    packet >> meshStyle;
    packet >> type;
    packet >> isCollidable;

    voxel.meshStyle = static_cast<VoxelMeshStyle>(meshStyle);
    voxel.type = static_cast<VoxelType>(type);
    voxel.isCollidable = isCollidable;

    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const VoxelData& voxel)
{
    u8 mesh = static_cast<u8>(voxel.meshStyle);
    u8 type = static_cast<u8>(voxel.type);
    u8 isCollidable = static_cast<u8>(voxel.isCollidable);
    packet << voxel.name;
    packet << voxel.topTexture;
    packet << voxel.sideTexture;
    packet << voxel.bottomTexture;
    packet << mesh;
    packet << type;
    packet << isCollidable;

    return packet;
}
