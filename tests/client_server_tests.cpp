#include <catch2/catch.hpp>

#include <atomic>
#include <client/network/client.h>
#include <common/network/net_constants.h>
#include <memory>
#include <server/server_engine.h>
#include <thread>



struct ServerHelper {
    ServerHelper(int max = 2)
    {
        server.runAsThread();
    }

    ~ServerHelper()
    {
        server.stop();
    }

    ServerEngine server;
};

void tickClient(Client& client)
{
    for (int i = 0; i < 100; i++) {
        client.tick();
    }
}

TEST_CASE("Client/Server connection tests")
{
    // SECTION("The client can connect to the server")
    // {
    //     ServerHelper server;
    //     Client client;
    //     client.connectTo(LOCAL_HOST);

    //     REQUIRE(client.getConnnectionState() == ConnectionState::Pending);
    // }

    // SECTION("The client can disconnect from the server")
    // {
    //     ServerHelper server;
    //     Client client;
    //     client.connectTo(LOCAL_HOST);
    //     client.tick();
    //     REQUIRE(client.getConnnectionState() == ConnectionState::Pending);
    //     client.disconnect();

    //     REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);
    // }

    // SECTION("The client can disconnect from the server, and then reconnect")
    // {
    //     ServerHelper server;
    //     Client client;
    //     client.connectTo(LOCAL_HOST);
    //     client.tick();
    //     client.disconnect();

    //     REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);

    //     client.connectTo(LOCAL_HOST);
    //     REQUIRE(client.getConnnectionState() == ConnectionState::Pending);

    //     client.tick();
    //     client.disconnect();

    //     REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);
    // }
/*
    SECTION("The client will be fully connected to a server if it is not full")
    {
        ServerHelper server;
        Client client;
        client.connectTo(LOCAL_HOST);
        tickClient(client);
        REQUIRE(client.getConnnectionState() == ConnectionState::Connected);
    }
    SECTION("The client will be disconnected from the server if it is full")
    {
        ServerHelper server(1);
        Client client;
        client.connectTo(LOCAL_HOST);
        tickClient(client);

        Client client2;
        client2.connectTo(LOCAL_HOST);
        tickClient(client2);
        REQUIRE(client2.getConnnectionState() == ConnectionState::Disconnected);
    }
    */
}
