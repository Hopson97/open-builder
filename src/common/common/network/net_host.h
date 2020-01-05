#pragma once

#include "../macros.h"
#include "enet.h"
#include "net_command.h"
#include "net_constants.h"
#include <SFML/Network/Packet.hpp>
#include <array>
#include <deque>
#include <iostream>
#include <optional>

#include <SFML/System/Clock.hpp>

template <typename T> void bench(const char *n, T f)
{
    sf::Clock c;
    f();
    float t = c.restart().asSeconds() * 1000;
    std::cout << n << ": " << t << '\n';
}

struct QueuedPacket {
    enum class Style {
        One,
        Broadcast,
    };
    ENetPeer *peer = nullptr;
    ENetPacket *packet = nullptr;
    Style style = Style::Broadcast;
    u8 channel = 0;
};

/**
 * @brief Base class for network hosts (clients/ servers)
 */
class NetworkHost {
    NON_COPYABLE(NetworkHost)
    NON_MOVEABLE(NetworkHost)

  public:
    NetworkHost(std::string &&name);

    virtual ~NetworkHost();

    /**
     * @brief Does 1 tick of the host, must be called once per frame.
     * The function will do all the receving and handling of connections and
     * commands
     */
    void tick();

    /**
     * @brief Creates as a client, and connects the host to a server
     *
     * @param ip The IP address of the server
     * @return std::optional<ENetPeer *> The server peer, might not be
     * successful connection hence optional
     */
    std::optional<ENetPeer *> createAsClient(const std::string &ip);

    /**
     * @brief Sets up the host to be a server
     *
     * @param maxConnections The maximum number connections to allow
     * @return true
     * @return false
     */
    bool createAsServer(int maxConnections);

    /**
     * @brief Sends a disconnect request to the peer
     *
     * @param peer The peer to disconnect from
     */
    void disconnectFromPeer(ENetPeer *peer);

    /**
     * @brief Disconnects all peers from this host
     */
    void disconnectAllPeers();

    int getConnectedPeerCount() const;
    peer_id_t getPeerId() const;
    int getMaxConnections() const;

  protected:
    /**
     * @brief Send a packet to a peer
     *
     * @param peer The peer to send the packet to
     * @param packet The packet to send
     * @param channel The channel ID to send the packet on
     * @param flags Flags for the packet (ENetPacketFlag)
     * @return true The packet was sent successfully
     * @return false The packet was not sent
     */
    void sendToPeer(ENetPeer *peer, sf::Packet &packet, u8 channel, u32 flags);

    /**
     * @brief Broadcasts a packet to all connected peers
     *
     * @param packet The packet to broadcast
     * @param channel The channel ID to send the packet on
     * @param flags Flags for the packet (ENetPacketFlag)
     */
    void broadcastToPeers(sf::Packet &packet, u8 channel, u32 flags);

  private:
    void removePeerFromPacketQueue(ENetPeer *peer);
    virtual void onPeerConnect(ENetPeer *peer) = 0;
    virtual void onPeerDisconnect(ENetPeer *peer) = 0;
    virtual void onPeerTimeout(ENetPeer *peer) = 0;
    virtual void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                                  command_t command) = 0;

    void onCommandRecieve(ENetPeer *peer, const ENetPacket &packet);

    void flush();

    std::deque<QueuedPacket> m_queue;

    ENetHost *mp_host = nullptr;
    const std::string m_name;

    peer_id_t m_peerId = 0;
    int m_maxConnections = 0;
};
