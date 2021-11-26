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
	char mensajeR1[68];
    char respuestaR1[66];
     int posficha;
    int dig1ficha;
    int dig2ficha;
    int posfichaenemiga;
    int dig1posfichaenemiga;
    int dig2posfichaenemiga;
    char fichacorrespondiente;
    time_t begin, end;
    time(&begin); // inicializa el calculo del tiempo para "R4"

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
	Lee_Socket (Socket_R1, mensajeR1, 68);
    int i;
    int posrespuestatablero=2;
    for(i = 0; i<=63; i++){ 
        respuestaR1[posrespuestatablero]=mensajeR1[i];
        posrespuestatablero++;
    }
    //tablero, posicion ficha y posicion ficha enemiga la cual se aplicara el movimiento."
    dig1ficha=mensajeR1[64] -'0';
    posficha=dig1ficha*10;
    dig2ficha=mensajeR1[65]-'0';
    posficha=posficha + dig2ficha+2; // el 2 corresponde debido a que los primeros dos espacios en la respuestaR1 corresponde al tiempo

    // se aplica el movimiento en el tablero
    //el espacio de la ficha ahora sera "l"
    fichacorrespondiente=respuestaR1[posficha];
    respuestaR1[posficha]='l';

    //se actuaiza el espacio en base al movimiento indicado
    dig1posfichaenemiga=mensajeR1[66] - '0';
    posfichaenemiga=dig1posfichaenemiga*10;
    dig2posfichaenemiga=mensajeR1[67] - '0';
    posfichaenemiga=posfichaenemiga+dig2posfichaenemiga+2; // el 2 corresponde debido a que los primeros dos espacios en la respuestaR1 corresponde al tiempo

    //Se mueve la ficha correspondiente "a" o "t", al espacio enemigo correspondiente
    respuestaR1[posfichaenemiga]=fichacorrespondiente

    // finaliza el calculo del tiempo para "R4 y se agrega al mensaje a R1"
    time(&end);
    time_t tiempoR4 = end - begin;
	int tiempointR4 = tiempoR4 + 0;
	char tiempocharR4 = tiempointR4 + '0';

    respuestaR1[0]='0';
    respuestaR1[1]=tiempocharR4;

    /*
	* Se envia el mensaje a "R1, tiempo de "R4" y tablero con movimiento ya aplicados
	*/
	Escribe_Socket (Socket_R1, respuestaR1, 66); 

	close (Socket_R1);
    return;
}