#include <enet/enet.h>
#include <iostream>
#include <cstring>
#include "client.h"

int startClient() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet.\n";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << " Failed to create ENet client.\n";
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetPeer* peer;

    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "Could not initiate connection.\n";
        enet_host_destroy(client);
        return EXIT_FAILURE;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connected to server.\n";
    } else {
        std::cerr << "Connection failed.\n";
        enet_peer_reset(peer);
        enet_host_destroy(client);
        return EXIT_FAILURE;
    }


    const char* msg = "Hello from C++ client!";
    ENetPacket* packet = enet_packet_create(msg, strlen(msg) + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(client);

   
    while (enet_host_service(client, &event, 3000) > 0) {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            std::cout << "Server: " << reinterpret_cast<char*>(event.packet->data) << "\n";
            enet_packet_destroy(event.packet);
        } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            std::cout << "Disconnected from server.\n";
            break;
        }
    }

    enet_host_destroy(client);
    return 0;
}
