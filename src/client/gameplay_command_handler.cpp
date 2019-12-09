#include "gameplay.h"
#include <common/network/packet.h>
#include <common/debug.h>

void Gameplay::handlePackets()
{
    Packet incoming;
    while (receivePacket(m_client.socket, incoming)) {
        switch (static_cast<ClientCommand>(incoming.command)) {
            case ClientCommand::PlayerJoin:
                handlePlayerJoin(incoming);
                break;

            case ClientCommand::PlayerLeave:
                handlePlayerLeave(incoming);
                break;

            case ClientCommand::Snapshot:
                handleSnapshot(incoming);
                break;

            case ClientCommand::ConnectionChallenge:
            case ClientCommand::AcceptConnection:
            case ClientCommand::RejectConnection:
                break;
        }
    }
}

void Gameplay::handleSnapshot(Packet &packet)
{
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
}

void Gameplay::handlePlayerJoin(Packet &packet)
{
    client_id_t id = 0;
    packet.data >> id;
    m_entities[id].active = true;
    LOGVAR("Client", "Player has joined the game, Client ID:", (int)id);
}

void Gameplay::handlePlayerLeave(Packet &packet)
{
    client_id_t id = 0;
    packet.data >> id;
    m_entities[id].active = false;
    LOGVAR("Client", "Player has left the game, Client ID:", (int)id);
}