#include "enet.h"

#include <cassert>

ENetPacket* createPacket(const sf::Packet& packet, u32 flags)
{
    return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
}

void Connection::send(const sf::Packet& packet, int channel, u32 flags)
{
    assert(peer);
    auto enetPacket = createPacket(packet, flags);
    enet_peer_send(peer, channel, enetPacket);
}

