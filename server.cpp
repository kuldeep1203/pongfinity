#include <enet/enet.h>
#include<iostream>
#include "server.h"
int startServer() {
    if (enet_initialize() != 0) {
        fprintf(stderr, " Failed to initialize ENet.\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost* server;

    address.host = ENET_HOST_ANY; // Listen on all interfaces
    address.port = 1234;

    server = enet_host_create(&address, 32, 2, 0, 0);
    if (server == NULL) {
        fprintf(stderr, "Failed to create ENet server.\n");
        return EXIT_FAILURE;
    }

    printf("ENet server started on port 1234. Waiting for connections...\n");

    ENetEvent event;
    while (1) {
        if (enet_host_service(server, &event, 5000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A client connected from %u.%u.%u.%u:%u\n",
                        event.peer->address.host & 0xFF,
                        (event.peer->address.host >> 8) & 0xFF,
                        (event.peer->address.host >> 16) & 0xFF,
                        (event.peer->address.host >> 24) & 0xFF,
                        event.peer->address.port);
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    printf("Received: %s\n", (char*)event.packet->data);
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    printf(" Client disconnected.\n");
                    break;

                default:
                    break;
            }
        } else {
            printf(" No activity. Still listening...\n");
        }
    }

    enet_host_destroy(server);
    return 0;
}
