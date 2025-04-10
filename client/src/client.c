#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;
	

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
    

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

    
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

    
	valor=config_get_string_value (config , "CLAVE");
    ip=config_get_string_value (config , "IP");
    puerto=config_get_string_value (config , "PUERTO");
      log_info(logger,"valor");


	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */
   
	leer_consola(logger);

    
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
   log_destroy(logger);
   config_destroy(config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	 nuevo_logger = log_create("tp0.log", "MiLogger", 1, LOG_LEVEL_INFO);
   
	log_info(nuevo_logger, "Soy un Log :)");



	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
    nuevo_config=config_create	("cliente.config")	;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	while(1){
		
		if(strcmp(leido, "") == 0 ){
			break;
		}
		// El resto, las vamos leyendo y logueando hasta recibir un string vacío
		
		log_info(logger, "%s", leido);


      
		// ¡No te olvides de liberar las lineas antes de regresar!
		 free(leido);
		leido = readline("> ");
          
	}

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	
    t_paquete* paquete = malloc(sizeof(t_paquete)); // Inicializa paquete

    // Verifica si la asignación de memoria fue exitosa
    if (paquete == NULL) {
        fprintf(stderr, "Error al asignar memoria para el paquete\n");
        return; // Salir si no se pudo asignar memoria
    }
	// Leemos y esta vez agregamos las lineas al paquete
    while (1){
		leido = readline(">");
		if (strlen(leido) == 0) {
				// Liberar la memoria asignada a la línea vacía y salir del bucle
				free(leido);
				break;
			}
          agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		  free(leido);

	}
    enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
