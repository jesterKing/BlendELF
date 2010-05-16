#ifndef network_h
#define network_h

#include <enet/enet.h>

typedef struct elf_network_object elf_network_object;

struct elf_network_object {
	elf_object* data;
	elf_network_object* next;
};

ENetHost* server;
ENetHost* client;
ENetAddress serverAddress;
ENetAddress clientAddress;
ENetPeer* peer;
ENetEvent serverEvent;
ENetEvent clientEvent;

GLFWthread serverThread;
unsigned char runServer = ELF_FALSE;

GLFWthread clientThread;
unsigned char runClient = ELF_FALSE;

elf_script* serverScript = NULL;
elf_script* clientScript = NULL;

elf_network_object* root = NULL;

/* initialises networking */
unsigned char elf_init_networking()
{
	if(enet_initialize() != 0)
	{
		elf_write_to_log("net: an error occurred while initialising networking\n");
		return ELF_FALSE;
	}

	return ELF_TRUE;
}

void elf_set_server_script(elf_script* script)
{
	if(serverScript) elf_dec_ref((elf_object*)serverScript);
	serverScript = script;
	if(serverScript) elf_inc_ref((elf_object*)serverScript);
}

void elf_set_client_script(elf_script* script)
{
	if(clientScript) elf_dec_ref((elf_object*)clientScript);
	clientScript = script;
	if(clientScript) elf_inc_ref((elf_object*)clientScript);
}

void GLFWCALL elf_run_networking(void* arg)
{
	if(server == NULL)
		return;

	while(runServer)
	{
		if(enet_host_service(server, &serverEvent, 100) > 0)
		{
			elf_write_to_log("net: cake event %d\n", serverEvent.type);
			switch(serverEvent.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				elf_write_to_log("net: client connected from %x:%u\n",
					serverEvent.peer->address.host,
					serverEvent.peer->address.port);

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				elf_write_to_log("net: packet of length %d received: %s\n",
					serverEvent.packet->dataLength,
					serverEvent.packet->data);

				elf_run_script(serverScript);

				enet_packet_destroy(serverEvent.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				elf_write_to_log("net: server - client disconnected.\n");
				break;
			default:
				break;
			}
		}
	}
}

void GLFWCALL elf_run_client_networking(void* arg)
{
	if(client == NULL)
		return;

	while(runClient)
	{
		if(enet_host_service(client, &clientEvent, 100) > 0)
		{
			elf_write_to_log("net: cookie event %d\n", clientEvent.type);
			switch(clientEvent.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("you like cake");
				//elf_write_to_log("net: server - client connected.\n");
				//elf_write_to_log("net: client connected from %x:%u\n",
				//	event.peer->address.host,
				//	event.peer->address.port);

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				elf_write_to_log("net: packet of length %d received\n",
					clientEvent.packet->dataLength);

				elf_run_script(clientScript);

				enet_packet_destroy(clientEvent.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				elf_write_to_log("net: client - client disconnected.\n");
				break;
			default:
				break;
			}
		}
	}
}

unsigned char elf_create_session(const char* address, unsigned short port)
{
	if(NULL != server)
	{
		elf_write_to_log("net: aborting attempt to initialise server: server is already initialised\n");
		return ELF_FALSE;
	}

	enet_address_set_host(&serverAddress, address);
	serverAddress.port = port;

	printf("hostname: %x\n", serverAddress.host);

	server = enet_host_create(&serverAddress, 32, 0, 0);

	if(NULL == server)
	{
		elf_write_to_log("net: unable to initialise server\n");
		return ELF_FALSE;
	}

	elf_write_to_log("net: server initialised at %s:%d\n", address, port);

	runServer = ELF_TRUE;
	serverThread = glfwCreateThread(elf_run_networking, server);

	return ELF_TRUE;
}

unsigned char elf_connect_session(const char* address, unsigned short port)
{
	elf_write_to_log("net: attempting to connect to %s:%d\n", address, port);

	if(NULL != peer)
	{
		elf_write_to_log("net: aborting attempt to connect to server: client is already connected\n");
		return ELF_FALSE;
	}

	client = enet_host_create(0, 1, 0, 0);

	if(NULL == client)
	{
		elf_write_to_log("net: failed to create client\n");
		return ELF_FALSE;
	}

	//clientAddress = (ENetAddress*)malloc(sizeof(ENetAddress));
	enet_address_set_host(&clientAddress, address);
	clientAddress.port = port;

	//printf("hostname: %x\n", clientAddress.host);

	peer = enet_host_connect(client, &clientAddress, 2);

	if(NULL == peer)
	{
		elf_write_to_log("net: failed to connect to server\n");
		return ELF_FALSE;
	}

	if(enet_host_service(client, &clientEvent, 5000) > 0 && clientEvent.type == ENET_EVENT_TYPE_CONNECT)
	{
		elf_write_to_log("net: successfully connected to %s:%d\n", address, port);

		runClient = ELF_TRUE;

		clientThread = glfwCreateThread(elf_run_client_networking, client);

		return ELF_TRUE;
	}

	enet_peer_reset(peer);

	elf_write_to_log("net: unable to connect to host\n");

	return ELF_FALSE;
}

unsigned char elf_disconnect_session()
{
	if(NULL == peer)
	{
		return ELF_FALSE;
	}

	runClient = ELF_FALSE;

	if(glfwWaitThread(clientThread, GLFW_WAIT))
	{
		enet_peer_disconnect(peer, 0);

		if(NULL != client)
			enet_host_destroy(client);

		peer = NULL;
		client = NULL;

		return ELF_TRUE;
	}
	else
	{
		elf_write_to_log("net: unable to terminate client thread");

		return ELF_FALSE;
	}
}

unsigned char elf_stop_session()
{
	if(NULL == server)
	{
		return ELF_FALSE;
	}

	runServer = ELF_FALSE;

	if(glfwWaitThread(serverThread, GLFW_WAIT))
	{
		enet_host_destroy(server);

		server = NULL;

		return ELF_TRUE;
	}
	else
	{
		elf_write_to_log("net: unable to terminate server thread");
		return ELF_FALSE;
	}
}

void elf_send_string_to_clients(const char* message)
{
	ENetPacket* packet;

	if(NULL == server) return;

	packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(server, 0, packet);
}

void elf_send_string_to_server(const char* message)
{
	ENetPacket* packet;

	if(NULL == peer) return;

	packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(client, 0, packet);
}

const char* elf_get_server_data_as_string()
{
	return clientEvent.packet->data;
}

const char* elf_get_client_data_as_string()
{
	return serverEvent.packet->data;
}

int elf_get_server_event()
{
	switch(serverEvent.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		return (int)ELF_NET_CONNECT;
	case ENET_EVENT_TYPE_RECEIVE:
		return (int)ELF_NET_RECEIVE;
	case ENET_EVENT_TYPE_DISCONNECT:
		return (int)ELF_NET_DISCONNECT;
	}

	return (int)ELF_NET_NONE;
}

int elf_get_client_event()
{
	switch(clientEvent.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		return (int)ELF_NET_CONNECT;
	case ENET_EVENT_TYPE_RECEIVE:
		return (int)ELF_NET_RECEIVE;
	case ENET_EVENT_TYPE_DISCONNECT:
		return (int)ELF_NET_DISCONNECT;
	}

	return (int)ELF_NET_NONE;
}

int elf_get_current_client()
{
	return (int)serverEvent.peer->incomingPeerID;
}

unsigned char elf_is_server()
{
	return NULL == peer;
}

unsigned char elf_is_client()
{
	return NULL == server;
}

/* deinitialises networking */
void elf_deinit_networking()
{
	if(NULL != peer)
	{
		elf_disconnect_session();
	}

	if(serverScript) elf_dec_ref((elf_object*)serverScript);
	if(clientScript) elf_dec_ref((elf_object*)clientScript);

	

	if(NULL != server)
	{
		enet_host_destroy(server);
	}
	if(NULL != client)
	{
		enet_host_destroy(client);
	}

	enet_deinitialize();
}

#endif