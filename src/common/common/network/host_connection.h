#pragma once

#include <SFML/Network/UdpSocket.hpp>

#include "packet.h"
/**
 * @brief Provides interface for sending and receiving packets
 */
class HostConnection
{
    public:
        HostConnection();
        
        void send(Packet& packet, const Endpoint& endpoint);
        bool receive(Packet& packet);

    private:
        sf::UdpSocket m_socket;
};