#include <stdio.h>
#include <Socket_Cliente.h>
#include <Socket.h>
#include <string.h>
#include <time.h>

#define SERVERR1 "192.168.0.10" // asociado a la IP de la tarjeta Rasberry Pi 2 llamada "R1"
                                // la cual se recibiran y enviaran mensajes correspondientes.
void main ()
{
	/*
	* Descriptor del socket necesario
	*/
	int Socket_R1;

    /*
	* Variables de utilizadas para lectura del "tablero.txt". Para envio del mensaje
	* a "R1"
	*/
	char mensajeR1[64];
    char respuestaR1[3]=['x','x','x'];
    time_t begin, end;
    time(&begin); // inicializa el calculo del tiempo para "R2"

	/*
	* Se abre la conexion con el servidor, con la ip correspondiente
	* "cpp_java" es un servicio dado de alta en /etc/services
	*/
	Socket_R1 = Abre_Conexion_Inet (SERVERR1, "cpp_java");
	if (Socket_R1 == 1)
	{
		printf ("No puedo establecer conexion con el servidor\n");
		exit (-1);
	}

    /*
	* Se lee la informacion enviada por el servidor "R1", que es
	* una cadena de 64 caracteres que corresponde al tablero en analisis.
	*/
	Lee_Socket (Socket_R1, mensajeR1, 64);
    int i;
    for(i = 0; i<=63; i++){ 
        if(mensajeR1[i] == 'a'){
            respuestaR1[0]='a';
            break;
        }
        if(mensajeR1[i] == 't'){
            respuestaR1[0]='t';
            break;
        }
    }

    // finaliza el calculo del tiempo para "R2"
    time(&end);
    time_t tiempoR2 = end - begin;
	int tiempointR2 = tiempoR2 + 0;
	char tiempocharR2 = tiempointR2 + '0';

    respuestaR1[1]='0';
    respuestaR1[2]=tiempocharR2;

    /*
	* Se envia el mensaje a "R1"
	*/
	Escribe_Socket (Socket_R1, respuestaR1, 3); 

	close (Socket_R1);
    return;
}