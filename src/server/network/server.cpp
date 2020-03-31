#include "server.h"

Server::Server(int maxConnections)
    : m_clients(maxConnections)
    , m_maxConnections(maxConnections)
    , m_salt(createHandshakeRandom())
{
    m_clientsMap.reserve(maxConnections);

    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = DEFAULT_PORT;
    mp_host = enet_host_create(&address, maxConnections, 2, 0, 0);
}

Server::~Server()
{
    if (mp_host) {
        enet_host_destroy(mp_host);
    }
}

bool Server::isSetup() const
{
    return mp_host != nullptr;
}

void Server::tick()
{
    assert(mp_host);
    ENetEvent event;
    while (enet_host_service(mp_host, &event, 0) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Got a connection " << event.peer->incomingPeerID
                          << std::endl;
                addPendingConnection(event.peer);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                std::cout << "Got a disconnection " << event.peer->incomingPeerID
                          << std::endl;
                break;

            case ENET_EVENT_TYPE_RECEIVE: {
                ServerPacket packet(event.packet);
                handlePacket(packet, event.peer);
                enet_packet_destroy(event.packet);
            } break;

            default:
                break;
        }
    }
}

void Server::handlePacket(ServerPacket& packet, ENetPeer* peer)
{
    using Cmd = ServerCommand;
    std::cout << "Command " << (int)packet.command << " Size: " << packet.payload.getDataSize() <<  std::endl;

    // clang-format off
    switch (packet.command) {
        case Cmd::HandshakePartOne: onHandshakePartOne(packet, peer); break;
        case Cmd::HandshakeResponse: onHandshakeResponse(packet, peer); break;
    }
    // clang-format on
}

void Server::addPendingConnection(ENetPeer* peer)
{
    Connection connection;
    connection.peer = peer;
    m_pendingConnections.push_back(connection);
}

void Server::onHandshakePartOne(ServerPacket& packet, ENetPeer* peer)
{
    for (auto& pending : m_pendingConnections) {
        if (pending.peer->incomingPeerID == peer->incomingPeerID) {
            pending.salt = packet.salt;
            auto outgoing = createPacket(ClientCommand::HandshakeChallenge, pending.salt);
            outgoing << m_salt;
            pending.send(outgoing);
        }
    }
}

void Server::onHandshakeResponse(ServerPacket& packet, ENetPeer* peer)
{
    for (auto itr = m_pendingConnections.begin(); itr != m_pendingConnections.end();) {
        if (itr->peer->incomingPeerID == peer->incomingPeerID) {
            // Accept or Reject connection
            u32 salt = itr->salt ^ m_salt;
            if (salt == packet.salt) {
                itr->salt = salt;
                Connection connection = *itr;
                std::cout << "Player joined!\n";
                itr = m_pendingConnections.erase(itr);
            }
            else {
                std::cout << "Connection was rejected\n";
                itr = m_pendingConnections.erase(itr);
            }
        }
    }
}
