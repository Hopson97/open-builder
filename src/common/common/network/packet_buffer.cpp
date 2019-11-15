#include "packet_buffer.h"

bool PacketBuffer::hasPacket(u32 sequenceNumber) const
{
    return m_reliablePacketBuffer.find(sequenceNumber) !=
           m_reliablePacketBuffer.cend();
}

void PacketBuffer::append(Packet &&packet, peer_id_t id)
{
    auto itr = m_reliablePacketBuffer.find(packet.sequenceNumber);
    if (itr == m_reliablePacketBuffer.cend()) {
        auto queuedPacket = m_reliablePacketBuffer.emplace(
            packet.sequenceNumber, std::move(packet));
        queuedPacket.first->second.clients.insert(id);
    }
    else {
        itr->second.clients.insert(id);
    }
}

void PacketBuffer::tryRemove(u32 packetNumber, peer_id_t peerId)
{
    auto itr = m_reliablePacketBuffer.find(packetNumber);
    if (itr != m_reliablePacketBuffer.end()) {
        auto &clients = itr->second.clients;
        auto client = clients.find(peerId);
        if (client != clients.end()) {
            clients.erase(client);
        }
        if (clients.empty()) {
            m_reliablePacketBuffer.erase(itr);
        }
    }
}

bool PacketBuffer::isEmpty() const
{
    return m_reliablePacketBuffer.empty();
}

ReliablePacket &PacketBuffer::begin()
{
    return m_reliablePacketBuffer.begin()->second;
}