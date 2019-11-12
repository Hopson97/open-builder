#include "packet.h"

Packet::Packet(command_t commandToSend, Flag flag, u32 seqNumber)
    : sequenceNumber(seqNumber)
    , command(commandToSend)
    , flags(static_cast<u8>(flag))
{
    payload << commandToSend << flags;
    if (seqNumber > 0) {
        payload << seqNumber;
    }
}

bool Packet::hasFlag(Flag flag)
{
    auto nflag = static_cast<u8>(flag);
    return (flags & nflag) == nflag;
}