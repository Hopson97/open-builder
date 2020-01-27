#include <catch2/catch.hpp>

#include <common/network/command_dispatcher.h>
#include <common/network/net_command.h>

enum class Command 
{
    Add = 0,
    Subtract,

    COUNT
};

class Handler 
{
  public:
      Handler()
      {  
          m_dispatcher.addCommand(Command::Add, &Handler::onAdd);
          m_dispatcher.addCommand(Command::Subtract, &Handler::onSubtract);
      }  

      void handle(command_t command, sf::Packet& packet)
      {
          m_dispatcher.execute(*this, command, packet);
      }

      u8 counter = 0;

  private:
      void onAdd(sf::Packet& packet)
      {  
          u8 adder = 0;
          packet >> adder;
          counter += adder;
      }  

      void onSubtract(sf::Packet& packet)
      {            
          u8 subber = 0;
          packet >> subber;
          counter -= subber;
      }  

      CommandDispatcher<Handler, Command> m_dispatcher;
};

TEST_CASE("Command dispatcher tests")
{
    Handler handler;
    

    SECTION("The command dispatcher can handle one type of command")
    {
        u8 adder = 2;
        sf::Packet packet;
        packet << static_cast<command_t>(Command::Add) << adder;

        //This would be done by net handler
        command_t command;
        packet >> command;
        handler.handle(command, packet);

        REQUIRE(handler.counter == adder);
    }

    SECTION("The command dispatcher can handle multiple types of command")
    {
        u8 adder = 12;
        u8 subber = 8;
        {
            sf::Packet packet;
            packet << static_cast<command_t>(Command::Add) << adder;

            //This would be done by net handler
            command_t command;
            packet >> command;
            handler.handle(command, packet);
        }
        {
            sf::Packet packet;
            packet << static_cast<command_t>(Command::Subtract) << subber;

            //This would be done by net handler
            command_t command;
            packet >> command;
            handler.handle(command, packet);
        }
        REQUIRE(handler.counter == (adder - subber));
    }

}