#include <stdio.h>
#include <Socket_Servidor.h>
#include <Socket.h>
#include <string.h>
#include <time.h>
#define SERVERR1 "192.168.0.10" // asociado a la IP de la Rasberry Pi 2 llamada "R1"
#define CLIENTR2 "192.168.0.11" // asociado a la IP de la Rasberry Pi 2 llamada "R2"
#define CLIENTR3 "192.168.0.12" // asociado a la IP de la Rasberry Pi 2 llamada "R3"
#define CLIENTR4 "192.168.0.13" // asociado a la IP de la Rasberry Pi 2 llamada "R4"
void main ()
{	
	/*
	* Descriptores de socket servidor ("R1") y de socket de los cliente ("R2","R3" y "R4")
	*/
	int Socket_R1;
	int Socket_R2;
    int Socket_R3;
    int Socket_R4;
	
	/*
	* Variables de utilizadas para lectura del "tablero.txt". Para envio de los mensajes
	* a las distintas "R2","R3" y "R4" . y para medicioon de tiempos
	*/
	char caracterestablero[72];
	char mensajeR2[64];
	char respuestaR2[3];
	char mensajeR3[65];
	char respuestaR3[6];
	char mensajeR4[68];
	char respuestaR4[66];
	char respuestafinal[73];
	FILE *lecturatablero;
    char caracter;
	FILE* escriturarespuesta;
	time_t begin, end;
    time(&begin); // inicializa el calculo del tiempo para "R1"
	
	/*
	* Se abre el socket servidor. Ademas se realiza la asociacion con la IP
    * correspondiente de la Rasberry Pi 2 llamada "R1"
	*/
	Socket_R1 = Abre_Socket_Inet ("cpp_java");
	Socket_R1.inet_addr(SERVERR1);

    /*
	* Se abre los sockect clientes. Ademas se realiza la asociacion de cada IP
    * correspondiente de la Rasberry Pi 2 llamadas R2","R3" y "R4"
	*/
    Socket_R2 = Abre_Socket_Inet ("cpp_java");
	Socket_R2.inet_addr(CLIENTR2);
    Socket_R3 = Abre_Socket_Inet ("cpp_java");
	Socket_R3.inet_addr(CLIENTR3);
    Socket_R4 = Abre_Socket_Inet ("cpp_java");
	Socket_R4.inet_addr(CLIENTR4);

	if (Socket_R1 == -1)
	{
		printf ("No se puede abrir socket servidor\n");
		exit (-1);
	}

    /*
	* Se prepara el tablero en forma texto para enviarla a "R2"
	*/

    lecturatablero =fopen("tablero.txt","r");
    int temp=0;
    while((caracter = fgetc(ptr_file)) != EOF)
    {   
        caracterestablero[temp]=caracter;
        temp++;
	};
	fclose(lecturatablero);
	// Para extraer los saltos de lineas
    int posarraytablero=0;
	int i;
	int y;
	for(i = 0; i<=7; i++){ 
		for(y = 0; y<=7; y++){
            mensajeR2[posarraytablero]=caracterestablero[posarraytablero];
			mensajeR3[posarraytablero]=caracterestablero[posarraytablero];
			mensajeR4[posarraytablero]=caracterestablero[posarraytablero];
        	posarraytablero++;
        }
		posarraytablero++;
    } 

	Escribe_Socket (Socket_R2, mensajeR2, 64);

    /*
	* Se  recibe la respuesta de "R2, incluye ficha detectada y tiempo de ejecucíon"
	*/

	Lee_Socket (Socket_R2, respuestaR2, 3);
	if(respuestaR2[0] == 'x'){    // corresponde a error debido a que en "R2" no se encontre ficha de movimiento ("a" o "t")
		escriturarespuesta = fopen("respuesta.txt", "wt");
    	fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fputs("error\n", escriturarespuesta);
		fclose(escriturarespuesta);
		return;
	}
	else{
	/*
	* Se envia el mensaje a "R3"
	*/
		mensajeR3[64]=respuestaR2[2];
		Escribe_Socket (Socket_R3, mensajeR3, 65);

	/*
	* Se  recibe la respuesta de "R3", indica tiempo de "R3", posicion de la ficha y posicion a mover la ficha.
	*/
		Lee_Socket (Socket_R3, respuestaR3, 6);

	/*
	* Se envia el mensaje a "R4; tablero, posicion ficha y posicion ficha enemiga la cual se aplicara el movimiento."
	*/
	//posicion ficha
		mensajeR4[64]=respuestaR3[2];
		mensajeR4[65]=respuestaR3[3];
	//posicion del movimiento ficha
		mensajeR4[66]=respuestaR3[4];
		mensajeR4[67]=respuestaR3[5];
		Escribe_Socket (Socket_R4, mensajeR4, 68);

	/*
	* Se  recibe la respuesta de "R4", indica tiempo de "R4", y tablero actualizado.
	*/
		Lee_Socket (Socket_R4, respuestaR4, 66);
	// se cierran todos los sockect de las tarjetas ("R1","R2","R3" y "R4")
		close (Socket_R1);
		close (Socket_R2);
		close (Socket_R3);
		close (Socket_R4);

	// finaliza el calculo del tiempo para "R1"
		time(&end);
    	time_t tiempoR1 = end - begin;
		int tiempointR1 = tiempoR1 + 0;
		char tiempocharR1 = tiempointR1 + '0';

		// inicializa la escritura de la "respuesta.txt"
		escriturarespuesta = fopen("respuesta.txt", "wt");
		int final=2;
		int j;
		int k;
		for(j = 0; j<=7; j++){ 
			for(k= 0; k<=7; k++){
				fputc(respuestaR4[final], escriturarespuesta);            
        		final++;
        	}
			fputc('\n', escriturarespuesta);   
    	} 
		//se agrega el tiempo de "R1"
		fputc('0', escriturarespuesta);
		fputc(tiempocharR1, escriturarespuesta);
		//se agrega el tiempo de "R2"
		fputc(respuestaR2[1], escriturarespuesta);
		fputc(respuestaR2[2], escriturarespuesta);
		//se agrega el tiempo de "R3"
		fputc(respuestaR2[0], escriturarespuesta);
		fputc(respuestaR2[1], escriturarespuesta);
		//se agrega el tiempo de "R2"
		fputc(respuestaR2[0], escriturarespuesta);
		fputc(respuestaR2[1], escriturarespuesta);
		fputc('\0', escriturarespuesta);
		fclose(escriturarespuesta);	
		return;
	}	
}