#include "utils.h"
#include <errno.h>

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints; 
	struct addrinfo *server_info;// server_info puntero donde se guardara direccciones que coinciden

	memset(&hints, 0, sizeof(hints));// pone o en todos los lugares del struct hints
	hints.ai_family = AF_INET;      // configuracion de hints
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int err= getaddrinfo(ip, puerto, &hints, &server_info);// getaddrinfo toma informacion del servidor
    if (err != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    return 1;}

	// Ahora vamos a crear el socket.
	int socket_cliente = 0;
    socket_cliente= socket(server_info->ai_family,server_info->ai_socktype,server_info->ai_protocol);// se crea el socket con info guardada en addrinfo usando el puntero server_info y llenado por getaddrinfo
   if (socket_cliente == -1) {// veo si se creo bien el socket
        perror("Error al crear el socket");
        freeaddrinfo(server_info);
        return 1;
    }

	// Ahora que tenemos el socket, vamos a conectarlo
   if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        perror("Error al conectar");
        freeaddrinfo(server_info);
        close(socket_cliente); // Cerramos el socket si falla la conexión
        return 1;
    }

    printf("Entro a crear_conexión\n");
	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	   if (paquete->buffer == NULL) {
        perror("Error al asignar memoria para el buffer");
        return;
    }
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
	 printf("se crea buffer\n");
}

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
if (paquete == NULL) {
        perror("Error al asignar memoria para el paquete");
        return NULL;
    }

	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	printf("se crea paquete\n");
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{

	
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
	printf("se agrego al paquete\n"); 
  

}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
