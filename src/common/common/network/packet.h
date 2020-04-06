#pragma once

#include "enet.h"
#include "net_command.h"

#include <glm/vec3.hpp>

struct VoxelData;

sf::Packet& operator>>(sf::Packet& packet, glm::vec3& vect);
sf::Packet& operator<<(sf::Packet& packet, const glm::vec3& vect);

sf::Packet& operator>>(sf::Packet& packet, VoxelData& voxelData);
sf::Packet& operator<<(sf::Packet& packet, const VoxelData& voxelData);

template <typename Incoming, typename Outgoing>
class Packet {
  public:
    Packet(ENetPacket* packet);

    Packet(Outgoing command, u32 salt);

    template <typename T>
    Packet& write(const T& t)
    {
        m_payload << t;
        return *this;
    }

    template <typename T>
    Packet& read(T& t)
    {
        m_payload >> t;
        return *this;
    }

    template <typename T>
    T read()
    {
        T t;
        m_payload >> t;
        return t;
    }

    const sf::Packet& get() const;
    Incoming command() const;
    u32 getSalt() const;

  private:
    Incoming m_command;
    u32 m_salt = 0;
    sf::Packet m_payload;
};

using ServerPacket = Packet<ServerCommand, ClientCommand>;
using ClientPacket = Packet<ClientCommand, ServerCommand>;

template <typename Incoming, typename Outgoing>
inline Packet<Incoming, Outgoing>::Packet(ENetPacket* packet)
{
    m_payload.append(packet->data, packet->dataLength);
    m_payload >> m_command >> m_salt;
}

template <typename Incoming, typename Outgoing>
inline Packet<Incoming, Outgoing>::Packet(Outgoing command, u32 salt)
    : m_salt(salt)
{
    m_payload << command << salt;
}

template <typename Incoming, typename Outgoing>
inline const sf::Packet& Packet<Incoming, Outgoing>::get() const
{
    return m_payload;
}

template <typename Incoming, typename Outgoing>
inline Incoming Packet<Incoming, Outgoing>::command() const
{
    return m_command;
}

template <typename Incoming, typename Outgoing>
inline u32 Packet<Incoming, Outgoing>::getSalt() const
{
    return m_salt;
}
