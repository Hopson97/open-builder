#pragma once

#include "packet.h"
#include <SFML/Network/UdpSocket.hpp>
#include "packet_buffer.h"
#include "endpoint.h"

template <typename CommandType> class NetworkNode {
  public:
    NetworkNode(int connectionCount)
    :   connections (connectionCount)
    {

    }
    
    NetworkNode(int connectionCount, port_t port)
    :   connections (connectionCount)
    {
        m_socket.bind(port);
    }

    void setNodeId(peer_id_t id)
    {
        m_nodeId = id;
    }

    void setBlocking(bool block)
    {
        m_socket.setBlocking(block);
    }

    Packet createPacket(CommandType command,
                                Packet::Flag flag = Packet::Flag::None)
    {
        return createCommandPacket(
            command, flag,
            flag == Packet::Flag::Reliable ? m_sequenceNumber++ : 0);
    }

    void handleAckPacket(sf::Packet &packet)
    {
        u32 sequence;
        peer_id_t id;
        packet >> sequence >> id;
        m_packetBuffer.tryRemove(sequence, id);
    }

    bool send(Packet &packet, const Endpoint& endpoint)
    {
        bool result = m_socket.send(packet.payload, endpoint.address,
                                    endpoint.port) == sf::Socket::Done;
        if (packet.hasFlag(Packet::Flag::Reliable)) {
            m_packetBuffer.append(std::move(packet), endpoint.id);
        }
        return result;
    }

    bool recieve(Packet &packet, Endpoint& endpoint)
    {
        sf::Packet rawPacket;
        if (m_socket.receive(rawPacket, endpoint.address, endpoint.port) ==
            sf::Socket::Done) {
            packet.initFromPacket(rawPacket);
            if (packet.hasFlag(Packet::Flag::Reliable)) {
                auto ack = createPacket(CommandType::Acknowledgment,
                                        Packet::Flag::Reliable);
                ack.payload << packet.sequenceNumber << m_nodeId;
                send(ack, endpoint);
            }
            return true;
        }
        return false;
    }

    void resendPackets()
    {
        if (!m_packetBuffer.isEmpty()) {
            auto &packet = m_packetBuffer.begin();
            if (!packet.clients.empty()) {
                auto itr = packet.clients.begin();
                send(packet.packet, connections[*itr]);
                packet.clients.erase(itr);
            }
        }
    }

    std::vector<Endpoint> connections;

  private:
    sf::UdpSocket m_socket;
    PacketBuffer m_packetBuffer;
    peer_id_t m_nodeId;
    u32 m_sequenceNumber = 0;
};




/*
        if (!m_packetBuffer.isEmpty()) {
            auto &packet = m_packetBuffer.begin();
            if (!packet.clients.empty()) {
                auto itr = packet.clients.begin();
                sendToClient(*packet.clients.begin(), packet.packet);
                packet.clients.erase(itr);
            }
        }
        */