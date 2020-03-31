#include <catch2/catch.hpp>

#include <atomic>
#include <client/network/client.h>
#include <common/network/net_constants.h>
#include <memory>
#include <server/network/server.h>
#include <thread>

struct ServerHelper {
    ServerHelper()
        : server(2)
    {
        runner = std::make_unique<std::thread>([&]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                server.tick();
            }
        });
    }

    ~ServerHelper()
    {
        running = false;
        runner->join();
    }

    Server server;
    std::atomic_bool running = true;
    std::unique_ptr<std::thread> runner;
};

TEST_CASE("Network client test")
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
        client.disconnect();

        REQUIRE(client.getConnnectionState() == ConnectionState::Disconnected);
    }
}