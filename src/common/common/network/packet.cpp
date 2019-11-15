#include "packet.h"

Packet::Packet(command_t commandToSend, Flag flag, u32 seqNumber)
    : sequenceNumber(seqNumber)
    , command(commandToSend)
    , flags(static_cast<u8>(flag))
{
    payload << commandToSend << flags << seqNumber;
}

void Packet::initFromPacket(sf::Packet& packet) {
    packet >> command >> flags >> sequenceNumber;
    payload = std::move(packet);
}

bool Packet::hasFlag(Flag flag)
{
    auto nflag = static_cast<u8>(flag);
    return (flags & nflag) == nflag;
}