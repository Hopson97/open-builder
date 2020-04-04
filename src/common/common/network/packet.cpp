#include "packet.h"

sf::Packet& operator>>(sf::Packet& packet, glm::vec3& vect)
{
    packet >> vect.x >> vect.y >> vect.z;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const glm::vec3& vect)
{
    packet << vect.x << vect.y << vect.z;
    return packet;
}