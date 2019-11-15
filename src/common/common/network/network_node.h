#pragma once

#include "packet.h"
#include <SFML/Network/UdpSocket.hpp>

struct CommandPackage {
    Packet packet;
    sf::IpAddress address;
    port_t port;
};

template <typename CommandType> class NetworkNode {
  public:
    NetworkNode() = default;
    NetworkNode(port_t port)
    {
        m_socket.bind(port);
    }

    void setNodeId(client_id_t id)
    {
        m_nodeId = id;
    }

    void setBlocking(bool block)
    {
        m_socket(block);
    }

    CommandPackage createPacket(CommandType command, Packet::Flag flag)
    {
        return createCommandPacket(
            command, flag,
            flag == Packet::Flag::Reliable ? m_sequenceNumber++ : 0);
    }

    bool send(CommandPackage &package)
    {
        auto &packet = package.packet;
        bool result =
            m_socket.send(packet.payload, package.address, package.port);
        if (packet.hasFlag(Packet::Flag::Reliable)) {
            m_packetBuffer.append(packet);
        }
    }

    bool recieve(CommandPackage &package)
    {
        auto& packet = package.packet;
        sf::Packet rawPacket;
        if (m_socket.receive(rawPacket, package.address, package.port)) {
            packet.initFromPacket(rawPacket);
            if (packet.hasFlag(Packet::Flag::Reliable)) {
                auto ack = createPacket(CommandType::Acknowledgment, Packet::Flag::Reliable);
                ack.packet.payload << packet.sequenceNumber << m_nodeId;
                ack.address = package.address;
                ack.packet = package.port;
                send(ack);
            }
            return true;
        }
        return false;
    }

  private:
    sf::UdpSocket m_socket;
    PacketBuffer m_packetBuffer;
    client_id_t m_nodeId;
    u32 m_sequenceNumber = 0;
};