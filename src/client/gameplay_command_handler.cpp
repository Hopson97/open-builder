#include "gameplay.h"
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>

void Gameplay::handlePackets()
{
    ENetEvent event;
    while (enet_host_service(m_host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                handlePacket(event.packet);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_CONNECT:
                LOG("Client", "Connection request has been received.");
                break;

            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
            case ENET_EVENT_TYPE_DISCONNECT:
                LOG("Client", "Disconnect received\n");
                break;

            default:
                break;
        }
    }
}

void Gameplay::handlePacket(ENetPacket *packet)
{
    sf::Packet pkt;
    pkt.append(packet->data, packet->dataLength);
    ClientCommand command;
    pkt >> command;
    switch (command) {
        case ClientCommand::PlayerJoin:
            handlePlayerJoin(pkt);
            break;

        case ClientCommand::PlayerLeave:
            handlePlayerLeave(pkt);
            break;

        case ClientCommand::Snapshot:
            handleSnapshot(pkt);
            break;

        case ClientCommand::AcceptConnection:
        case ClientCommand::RejectConnection:
            break;
    }
}

void Gameplay::handleSnapshot(sf::Packet &packet)
{
    /*
    u16 n = 0;
    packet.data >> n;

    for (u16 i = 0; i < n; i++) {
        client_id_t id = 0;
        float x, y, z;
        packet.data >> id >> x >> y >> z;
        if (id != m_client.getClientId()) {
            auto *p = &m_entities[id];
            p->position = {x, y, z};
            p->active = true;
        }
    }
    */
}

void Gameplay::handlePlayerJoin(sf::Packet &packet)
{
    client_id_t id = 0;
    packet >> id;
    m_entities[id].active = true;
    LOGVAR("Client", "Player has joined the game, Client ID:", (int)id);
}

void Gameplay::handlePlayerLeave(sf::Packet &packet)
{
    client_id_t id = 0;
    packet >> id;
    m_entities[id].active = false;
    LOGVAR("Client", "Player has left the game, Client ID:", (int)id);
}