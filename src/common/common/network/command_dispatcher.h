#pragma once

#include "net_types.h"
#include <SFML/Network/Packet.hpp>
#include <vector>

template <typename Handler, typename CommandType>
class CommandDispatcher {
  public:
    using CommandHandler = void (Handler::*)(sf::Packet &packet);

    CommandDispatcher()
        : m_commands(static_cast<size_t>(CommandType::COUNT))
    {
    }

    void addCommand(CommandType command, CommandHandler handler)
    {
        m_commands[static_cast<command_t>(command)] = handler;
    }

    void execute(Handler &handler, command_t command, sf::Packet &packet)
    {
        (handler.*(m_commands[command]))(packet);
    }

  private:
    std::vector<CommandHandler> m_commands;
};