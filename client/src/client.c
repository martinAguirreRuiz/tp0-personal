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
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	//Chequeamos que existan los valores pedidos

	if (!(config_has_property(config, "CLAVE")))
	{
		printf("No existe el valor 'CLAVE' asociado \n");
		exit(1);
	}
	else if (!(config_has_property(config, "IP")))
	{
		printf("No existe el valor 'IP' asociado \n");
		exit(1);
	}
	else if (!(config_has_property(config, "PUERTO")))
	{
		printf("No existe el valor 'PUERTO' asociado \n");
		exit(1);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'}

	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config
	log_info(logger, "El valor de config obtenido es: %s", valor);

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
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "Estado: ", 1, LOG_LEVEL_INFO);

	return nuevo_logger;
}


t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../tp0.config");

	if (nuevo_config == NULL)
	{
		printf("No se pudo crear el config \n");
		exit(1); //Salgo del programa con mensaje de error
	}
	

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	while (1) {
		leido = readline("> ");

        if (strcmp(leido, "") == 0) { //Si leido es vacio
            break;
        }

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
        log_info(logger, "Leido de consola: %s", leido);
	// ¡No te olvides de liberar las lineas antes de regresar!
		free(leido);
    }

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (1) {
		leido = readline("> ");

        if (strcmp(leido, "") == 0) { //Si leido es vacio
            break;
        }

        agregar_a_paquete(paquete, leido, strlen(leido)+1);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
		free(leido);
    }
	enviar_paquete(paquete, conexion);
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
