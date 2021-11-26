#include <stdio.h>
#include <Socket_Cliente.h>
#include <Socket.h>
#include <string.h>
#include <time.h>

#define SERVERR1 "192.168.0.10" // asociado a la IP de la tarjeta Rasberry Pi 2 llamada "R1"
                                // la cual se recibiran y enviaran mensajes correspondientes.

//funciones asociadas al arfil "a"
void movdiagizqsuperior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                       int posicionfichanalizar, int *movimientofinal);
void movdiagdersuperior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                       int posicionfichanalizar, int *movimientofinal);
void movdiagderinferior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                       int posicionfichanalizar, int *movimientofinal);
void movdiagizqinferior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                       int posicionfichanalizar, int *movimientofinal);

//funciones asociadas a la torre "t"
void movsuperior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                 int posicionfichanalizar, int *movimientofinal);
void movderecha(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                 int posicionfichanalizar, int *movimientofinal);
void movinferior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, 
                 int posicionfichanalizar, int *movimientofinal);
void movizquierda(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios,
                 int posicionfichanalizar, int *movimientofinal);


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
	char mensajeR1[65];
    char respuestaR1[6];
    int movimientosalestorios[14]; // mayor cantidad de posibles movimientos
    int tempposmovaleatorio; // almacena la posicion del indice de posibles movovimientos
    int movposicionfichaenemiga=99; // almacena el movimiento final
    char fichaaevaluar; // almacena ficha a analizar
    int posfichaanalizar; // almacena posicion de la ficha ("a" o "t") a analizar
    int primerdigmovmiento; // almacena el primer digito de izquirda a derecha del movimiento a realizar
    int segundodigmovmiento; // almacena el segundo digito de izquirda a derecha del movimiento a realizar
    time_t begin, end;
    time(&begin); // inicializa el calculo del tiempo para "R3"

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
	* una cadena de 65 caracteres que corresponde al tablero en analisis.
	*/
	Lee_Socket (Socket_R1, mensajeR1, 65);
    fichaaevaluar=mensajeR1[64];

    int i;
    for(i = 0; i<=63; i++){ 
        if(mensajeR1[i] == fichaaevaluar){
            posfichaanalizar=i;
            break;
        }
    }

    // verificaciones movimiento arfil
    if(fichaaevaluar == 'a'){
        movdiagizqsuperior(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        if(movposicionfichaenemiga==99){
            movdiagderqsuperior(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        }
        if(movposicionfichaenemiga==99){
            movdiagderinferior(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        }
        if(movposicionfichaenemiga==99){
            movdiagizqinferior(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        }
        if(movposicionfichaenemiga==99){
            int movaletorio=  rand() % (tempposmovaleatorio+1);
            primerdigmovmiento= movaletorio/10;
            segundodigmovmiento=  movaletorio%10;
        }
        else{
            int movposf=  movposicionfichaenemiga;
            primerdigmovmiento= movposf/10;
            segundodigmovmiento=  movposf%10;
        }
    }
    else{
        // verificaciones movimiento torre
        movsuperior(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        if(movposicionfichaenemiga==99){
            movderecha(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        }
        if(movposicionfichaenemiga==99){
            movinferior(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        }
        if(movposicionfichaenemiga==99){
            movizquierda(mensajeR1, movimientosalestorios, &tempposmovaleatorio, posfichaanalizar, &movposicionfichaenemiga);
        }
        if(movposicionfichaenemiga==99){
            int movaletorio=  rand() % (tempposmovaleatorio+1);
            primerdigmovmiento= movaletorio/10;
            segundodigmovmiento=  movaletorio%10;
        }
        else{
            int movposf=  movposicionfichaenemiga;
            primerdigmovmiento= movposf/10;
            segundodigmovmiento=  movposf%10;
        }
    }

    
    // finaliza el calculo del tiempo para "R2"
    time(&end);
    time_t tiempoR3 = end - begin;
	int tiempointR3 = tiempoR3 + 0;
	char tiempocharR3 = tiempointR3 + '0';

    //se crea el mensaje indica tiempo de "R3", posicion de la ficha analizada y posicion a mover la ficha.
    // se indica tiempo en el mensaje a R1
    respuestaR1[0]='0';
    respuestaR1[1]=tiempocharR3;

    //se indica posicion de la ficha analizada en el mensaje a R1
    int primerdigfichaanalizda= posfichaanalizar/10;
    int segundodigfichaanalizda=  posfichaanalizar%10;
    char charprimerdigfichaanalizda = primerdigfichaanalizda + '0';
    char charsegundodigfichaanalizda = segundodigfichaanalizda + '0';
    respuestaR1[2]=charprimerdigfichaanalizda;
    respuestaR1[3]=charsegundodigfichaanalizda;
    
    //se indica posicion a mover la ficha en el mensaje a R1
    char charprimerdigmovimiento = primerdigmovmiento + '0';
    char charsegundodigmovimiento = segundodigmovmiento + '0';
    respuestaR1[4]=charprimerdigfichaanalizda;
    respuestaR1[5]=charsegundodigfichaanalizda;

    /*
	* Se envia el mensaje a "R1, indica tiempo, posicion de la ficha en analisis y movimiento de la ficha"
	*/
	Escribe_Socket (Socket_R1, respuestaR1, 6); 

	close (Socket_R1);
    return;
}

//verificacion movimientos del arfil("a")
// verificacion del movimiento del arfil en forma diagonal izquierda superior
void movdiagizqsuperior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites arriba e izquierda del tablero
    if (posicionfichanalizar == 0 || posicionfichanalizar == 1 || posicionfichanalizar == 2 || posicionfichanalizar == 3 || posicionfichanalizar == 4 ||
        posicionfichanalizar == 5 || posicionfichanalizar == 6 || posicionfichanalizar == 7 || posicionfichanalizar == 8 || posicionfichanalizar == 16||
        posicionfichanalizar == 24 || posicionfichanalizar == 32 || posicionfichanalizar == 40 || posicionfichanalizar == 48 || posicionfichanalizar == 56){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 0 && posicionfichanalizar != 1 && posicionfichanalizar != 2 && posicionfichanalizar != 3 && posicionfichanalizar != 4 &&
               posicionfichanalizar != 5 && posicionfichanalizar != 6 && posicionfichanalizar != 8 && posicionfichanalizar != 16 &&
               posicionfichanalizar != 24 && posicionfichanalizar != 32 && posicionfichanalizar != 40 && posicionfichanalizar != 48 ){
            posicionfichanalizar=posicionfichanalizar-9;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
        if(tablero[posicionfichanalizar] == 'f'){
            *movimientofinal=posicionfichanalizar;
        }
    } 
}

// verificacion del movimiento del arfil en forma diagonal derecha superior
void movdiagdersuperior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites arriba y derecha del tablero
    if (posicionfichanalizar == 0 || posicionfichanalizar == 1 || posicionfichanalizar == 2 || posicionfichanalizar == 3 || posicionfichanalizar == 4 ||
        posicionfichanalizar == 5 || posicionfichanalizar == 6 || posicionfichanalizar == 7 || posicionfichanalizar == 15 || posicionfichanalizar == 23||
        posicionfichanalizar == 31 || posicionfichanalizar == 39 || posicionfichanalizar == 47 || posicionfichanalizar == 55 || posicionfichanalizar == 63){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 1 && posicionfichanalizar != 2 && posicionfichanalizar != 3 && posicionfichanalizar != 4 &&
        posicionfichanalizar != 5 && posicionfichanalizar != 6 && posicionfichanalizar != 7 && posicionfichanalizar != 15 && posicionfichanalizar != 23&&
        posicionfichanalizar != 31 && posicionfichanalizar != 39 && posicionfichanalizar != 47 && posicionfichanalizar != 55 ){
            posicionfichanalizar=posicionfichanalizar-7;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
        if(tablero[posicionfichanalizar] == 'f'){
            *movimientofinal=posicionfichanalizar;
        }
    } 
}

// verificacion del movimiento del arfil en forma diagonal derecha inferior
void movdiagderinferior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites inferior y derecha del tablero
    if (posicionfichanalizar == 7 || posicionfichanalizar == 15 || posicionfichanalizar == 23 || posicionfichanalizar == 31 || posicionfichanalizar == 39 ||
        posicionfichanalizar == 47 || posicionfichanalizar == 55 || posicionfichanalizar == 63 || posicionfichanalizar == 56 || posicionfichanalizar == 57||
        posicionfichanalizar == 58 || posicionfichanalizar == 59 || posicionfichanalizar == 60 || posicionfichanalizar == 61 || posicionfichanalizar == 62){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 15 && posicionfichanalizar != 23 && posicionfichanalizar != 31 && posicionfichanalizar != 39 &&
        posicionfichanalizar != 47 && posicionfichanalizar != 55 && posicionfichanalizar != 63 && posicionfichanalizar != 57 &&
        posicionfichanalizar != 58 && posicionfichanalizar != 59 && posicionfichanalizar != 60 && posicionfichanalizar != 61 && posicionfichanalizar != 62){
            posicionfichanalizar=posicionfichanalizar+9;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
        if(tablero[posicionfichanalizar] == 'f'){
            *movimientofinal=posicionfichanalizar;
        }
    } 
}

// verificacion del movimiento del arfil en forma diagonal izquierda inferior
void movdiagizqinferior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites inferior e izquierda del tablero
    if (posicionfichanalizar == 0 || posicionfichanalizar == 8 || posicionfichanalizar == 16 || posicionfichanalizar == 24 || posicionfichanalizar == 32 ||
        posicionfichanalizar == 40 || posicionfichanalizar == 48 || posicionfichanalizar == 56 || posicionfichanalizar == 57 || posicionfichanalizar == 58 || 
        posicionfichanalizar == 59 || posicionfichanalizar == 60 || posicionfichanalizar == 61 || posicionfichanalizar == 62 || posicionfichanalizar == 63){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 8 && posicionfichanalizar != 16 && posicionfichanalizar != 24 && posicionfichanalizar != 32 &&
        posicionfichanalizar != 40 && posicionfichanalizar != 48 && posicionfichanalizar != 56 && posicionfichanalizar != 57 && posicionfichanalizar != 58 && 
        posicionfichanalizar != 59 && posicionfichanalizar != 60 && posicionfichanalizar != 61 && posicionfichanalizar != 62 ){
            posicionfichanalizar=posicionfichanalizar+7;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
        if(tablero[posicionfichanalizar] == 'f'){
            *movimientofinal=posicionfichanalizar;
        }
    } 
}



//verificacion movimientos de la toorre("t")
// verificacion del movimiento de la torre en forma superior
void movsuperior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites arriba tablero
    if (posicionfichanalizar == 0 || posicionfichanalizar == 1 || posicionfichanalizar == 2 || posicionfichanalizar == 3 || posicionfichanalizar == 4 ||
        posicionfichanalizar == 5 || posicionfichanalizar == 6 || posicionfichanalizar == 7 ){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 0 && posicionfichanalizar != 1 && posicionfichanalizar != 2 && posicionfichanalizar != 3 && posicionfichanalizar != 4 &&
               posicionfichanalizar != 5 && posicionfichanalizar != 6 && posicionfichanalizar != 7){
            posicionfichanalizar=posicionfichanalizar-8;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
        if(tablero[posicionfichanalizar] == 'f'){
            *movimientofinal=posicionfichanalizar;
        }
    } 
}

// verificacion del movimiento de la torre en forma derecha
void movderecha(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites derecha del tablero
    if (posicionfichanalizar == 7 || posicionfichanalizar == 15 || posicionfichanalizar == 23|| posicionfichanalizar == 31 || posicionfichanalizar == 39 || 
        posicionfichanalizar == 47 || posicionfichanalizar == 55 || posicionfichanalizar == 63){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 7 && posicionfichanalizar != 15 && posicionfichanalizar != 23 && posicionfichanalizar != 31 && 
               posicionfichanalizar != 39 && posicionfichanalizar != 47 && posicionfichanalizar != 55 && posicionfichanalizar != 63 ){
            posicionfichanalizar=posicionfichanalizar + 1;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
       *movimientofinal=posicionfichanalizar;
    } 
}

// verificacion del movimiento de la torre en forma inferior
void movinferior(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites inferior tablero
    if (posicionfichanalizar == 56 || posicionfichanalizar == 57 || posicionfichanalizar == 58 || posicionfichanalizar == 59 || 
        posicionfichanalizar == 60 || posicionfichanalizar == 61 || posicionfichanalizar == 62 || posicionfichanalizar == 63){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar != 56 && posicionfichanalizar != 57 && posicionfichanalizar != 58 && posicionfichanalizar != 59 && 
        posicionfichanalizar != 60 && posicionfichanalizar != 61 && posicionfichanalizar != 62 && posicionfichanalizar != 63){
            posicionfichanalizar=posicionfichanalizar+8;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
        *movimientofinal=posicionfichanalizar;
    } 
}

// verificacion del movimiento de la torre en forma izquierda
void movizquierda(char tablero[], int almacenarposiblesmov[], int *posalmecenamientoaleatorios, int posicionfichanalizar, int *movimientofinal){
    //se verifica que no este en los limites izquierda del tablero
    if (posicionfichanalizar == 0 || posicionfichanalizar == 8 || posicionfichanalizar == 16 || posicionfichanalizar == 24 || posicionfichanalizar == 32 ||
        posicionfichanalizar == 40 || posicionfichanalizar == 48 || posicionfichanalizar == 56){

    }
    else{
        //Identificacion de una posible ficha enemiga "f"
        while (posicionfichanalizar == 0 && posicionfichanalizar == 8 && posicionfichanalizar == 16 && posicionfichanalizar == 24 && posicionfichanalizar == 32 &&
        posicionfichanalizar == 40 && posicionfichanalizar == 48 && posicionfichanalizar == 56){
            posicionfichanalizar=posicionfichanalizar+7;
            if(tablero[posicionfichanalizar] == 'f'){
                *movimientofinal=posicionfichanalizar;
                break;
            }
            else{
                almacenarposiblesmov[*posalmecenamientoaleatorios]=posicionfichanalizar;
                *posalmecenamientoaleatorios++;
            }
        }
       * movimientofinal=posicionfichanalizar;
    } 
}