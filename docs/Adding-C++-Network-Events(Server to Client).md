# Adding new network events

This is a step-by-step guide on adding new network events that are sent from the server side, to be handled by the client side.

For example, updating the positions of entities on the client, by sending a packet from the server

## Step 1 - Add the event to the enum class

1. Go to (TODO: Add link) `src/common/network/net_command.h`

2. Add to either `ClientCommand` or `ServerCommand`

    a. `ClientCommand` is for sending a message FROM the server TO the client

    b. `ServerCommand` is for sending a message FROM the client TO the server

3. In this case, it is a `ClientCommand`. Add the enum, and explain what data will be sent in a comment

Example:
```cpp
enum class ClientCommand : command_t {
    ...
    // Update entities states (position and rotation)
    // u32: The number of entities to update
    // [Loop this]
    // u32: The ID of the entity
    // float[3]: The position of the entity
    // float[3]: The rotation of the entity
    UpdateEntityStates,
```

## Step 2 - Add a function to the client class for handling the event

1. Go to (TODO Add link) `src/client/network/client_packet_handler.cpp` and (TODO Add link) `src/client/network/client.h`

2. In the header file, find the `void on<ClientCommand>(ClientPacket& packet);` function declarations and add your one here, keeping it in alphabetical order

3. In this case, it would be `void onUpdateEntityStates(ClientPacket& packet);`

4. Define your function in the source file, in this case it would be 

```cpp
void Client::onUpdateEntityStates(ClientPacket& packet)
{
    //...
}
```

## Step 3 - Read the packet

1. Read the data from the packet, you can see the other functions to understand how the interface of `ClientPacket` works (`auto data = packet.read<type>()`)

2. For example, for this you would read a u32 for the entity count, loop through it "count" times, updating the entities as you go:

```cpp
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        auto entId = packet.read<u32>();
        auto entPosition = packet.read<glm::vec3>();
        auto entRotation = packet.read<glm::vec3>();
        mp_world->updateEntity(entId, entPosition, entRotation);
    }
```

3. If needed, you would probably have to add the functionality to do whatever your event is handling (eg I had to add the update entity function to the ClientWorld class for this one)

## Step 4 - Listen for the packet

1. In the `client.cpp` file, find the `handlePacket` function

2. You'll see a switch case, simply add a `case` for the new packet type, calling the new function; keeping things in alphabetical order and alligned eg

```cpp
void Client::handlePacket(ClientPacket& packet)
{
using Cmd = ClientCommand;
// ....
case Cmd::UpdateEntityStates:   onUpdateEntityStates    (packet);   break;
// ....

```

## Step 5 - Write functions to create a packet on the server

1. Go to (TODO Add link) `src/server/network/` folder.

2. Depending on the packet type depends where the function to create the packet is created. 

3. If it is client-specific, then it would go into the `ClientSession` class (`client_session.h` and `client_session.cpp`)

3. In this case, it is a broadcast to all clients, so it goes in the `Server` class (`server.h` and `server.cpp`)

4. For a broadcast, name the function `broadcast<ClientCommand name>`, eg this case would be `void broadcastEntityStates();`

5. Again, define it in the source file:

```cpp
void Server::broadcastEntityStates()
{

}
```

## Step 6 - Send the packet

1. In the function body, create a ServerPacket, where the first parameter is the command name and the second parameter is `m_salt`

2. Eg, in this case it would be like `    ServerPacket packet(ClientCommand::UpdateEntityStates, m_salt);
`

3. Write some data to the packet. Usually, this would be passed in via the function parameters

4. In this case, I already have a function to serialise packets, so I will just call that

```cpp
mp_world->serialiseEntities(packet, 0);
```

5. Send the packet. For a broadcast, this is like:

    ```
    broadcastToPeers(m_host.handle, packet.get());
    ```

## Step 7 - Call your new function from somewhere on the server

1. The server is owned by the ServerLauncher class, and maybe you could call it from the server loop, for example:

```cpp

void ServerLauncher::launch()
{
    // ...
    while (m_isServerRunning) {
        // ...
        m_server.broadcastEntityStates();
        // ...
    }
}
```

2. Done

# END
