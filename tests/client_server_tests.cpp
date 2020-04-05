#include <catch2/catch.hpp>

#include <atomic>
#include <client/network/client.h>
#include <common/network/net_constants.h>
#include <memory>
#include <server/network/server.h>
#include <server/world/server_world.h>
#include <thread>

struct ServerHelper {
    ServerHelper(int max = 2)
        :   world (1)
        ,   server(max, world)
    {
        runner = std::make_unique<std::thread>([&]() {
            while (running) {
                server.tick();
            }
        });
    }

    ~ServerHelper()
    {
        running = false;
        runner->join();
    }
    ServerWorld world;
    Server server;
    std::atomic_bool running = true;
    std::unique_ptr<std::thread> runner;

};

void tickClient(Client& client, int times)
{
    for (int i = 0; i < times; i++) {
        client.tick();
    }
}

TEST_CASE("Client/Server connection tests")
{
    SECTION("The client can connect to the server")
    {
        ServerHelper server;
        Client client;
        client.connectTo(LOCAL_HOST);

        REQUIRE(client.getConnnectionState() == ConnectionState::Pending);
    }

    SECTION("The client can disconnect from the server")
    {
        ServerHelper server;
        Client client;
        client.connectTo(LOCAL_HOST);
        client.tick();
        REQUIRE(client.getConnnectionState() == ConnectionState::Pending);
        client.disconnect();

        REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);
    }

    SECTION("The client can disconnect from the server, and then reconnect")
    {
        ServerHelper server;
        Client client;
        client.connectTo(LOCAL_HOST);
        client.tick();
        client.disconnect();

        REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);

        client.connectTo(LOCAL_HOST);
        REQUIRE(client.getConnnectionState() == ConnectionState::Pending);

        client.tick();
        client.disconnect();

        REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);
    }

    SECTION("The client will be fully connected to a server if it is not full")
    {
        ServerHelper server;
        Client client;
        client.connectTo(LOCAL_HOST);
        tickClient(client, 100);
        REQUIRE(client.getConnnectionState() == ConnectionState::Connected);
    }    
    SECTION("The client will be disconnected from the server if it is full")
    {
        ServerHelper server(1);
        Client client;
        client.connectTo(LOCAL_HOST);
        tickClient(client, 100);        

        Client client2;
        client2.connectTo(LOCAL_HOST);
        tickClient(client2, 100);
        REQUIRE(client2.getConnnectionState() == ConnectionState::Disconnected);
    }
}