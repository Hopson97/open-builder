#include "host_connection.h"

HostConnection::HostConnection()
{
    m_socket.setBlocking(false);
}

void HostConnection::send(Packet& packet, const Endpoint& endpoint)
{
}
bool HostConnection::receive(Packet& packet)
{
}