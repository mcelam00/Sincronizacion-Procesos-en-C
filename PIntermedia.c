#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>




//PROTOTIPOS DE LAS FUNCIONES
int calculaAleatorios(int min, int max);


//MANEJADORAS:

void manejadoraMozo(int NumSenial){


int encontrado;

		//HABIENDO RECIBIDO EL SIGPIPE EL MOZO SE PONE EN FUNCIONAMIENTO
	printf("(MOZO) Soy el mozo y mi PID es %d\n", getpid());
	printf("(MOZO) Sí Somelier, ¡¡voy inmediatamente!!\n");
				
	printf("(MOZO) <<Sale disparado a por lo que falta>>\n");
	srand(getpid());
	encontrado = calculaAleatorios(0,1); //se genera un número aleatorio que es el que indica si sen encuentra el vino o no, o si se encientran los ingredientes o no (en general).

	printf("(MOZO) <<Rebusca por los armarios>>\n");

		if(encontrado == 1){
			//encontrado
			printf("(MOZO) ¡Encontrado Somelier!\n");	
			exit(1); //al encontrarlos el mozo se muere y retorna un 1 al somelier

		}else{
			//No encontrado	
			printf("(MOZO) ¡No encontrado Somelier!\n");
			exit(0); //al no encontrarlos el mozo se muere y retorna un 0 al somelier
		}

}





void manejadoraSomelier(int NumSenial){


//aquí venimos cuando el chef da la orden de que empieza a preparar los platos
pid_t mozo;


 //en el codigo del padre (el padre del mozo, que es el somelier) es donde hay que ver si la señal que le ha llegado a él (somelier) es de vino o de ingredientes
//voy a crear una manejadora para la señal SIGPIPE

int estado;
pid_t PIDMozo;
int encontrado;



//creo el mozo de los recados
mozo = fork();



	if(mozo == -1){

		 perror("(SOMELIER) Error en la llamada al fork\n");

	}else if(mozo == 0){
		/* CODIGO QUE EJECUTA EL HIJO CREADO POR EL SOMELIER, ES DECIR EL MOZO */

			if(signal(SIGPIPE, manejadoraMozo) == SIG_ERR){		//enmascara la señal sigpipe
				perror("(MOZO) Error en la llamada a signal\n");
			}
			
		pause(); //el mozo empieza nada mas nacer pausado para que no se descompase la ejecucion y no va a arrancar hasta que su padre el somelier le diga lo que debe hacer con SIGPIPE Y LO HARÁ EN LA MANEJADORA DE ESA SEÑAL	
			


	}else{

		/* CODIGO QUE EJECUTA EL PADRE, ES DECIR, EL PROPIO SOMELIER */

		sleep(2);//NOTA: es necesario porque si no, no le da tiempo al mozo a enmascarar la señal y la ejecucion no entra en la manejadora del mozo		
		
		//Distinguimos en funcion del valor de señal recibida que le llega como parámetro a la manejadora entre la SIGUSR1 Y A LA SIGUSR2

			if(NumSenial == SIGUSR1){		//si la señal que le llega al somelier del mozo es la SIGUSR1  se demandan los ingredientes.

				printf("(SOMELIER) ¡Alto y Claro Chef!\n");

				printf("(SOMELIER) ¡Mozo! ¡Vaya a por los ingredientes por favor!\n");

				int error;
				
				error = kill(mozo,SIGPIPE);

					if(error == -1){
						perror("(SOMELIER) Error en el envío de la señal SIGPIPE al Mozo\n");						
					}

					
				PIDMozo = wait(&estado);		//se queda en espera a que termine el mozo la tarea

					if(PIDMozo == -1 ){
						perror("(SOMELIER) Se ha producido un error durante la llamada a wait: Mozo\n");
					}

				encontrado = WEXITSTATUS(estado);

					if(encontrado == 0){
						printf("(SOMELIER) ¡Chef, no hemos encontrado los ingredientes!\n");
						exit(2);
					}else{
						printf("(SOMELIER) ¡Chef, ya tenemos los ingredientes!\n");
						exit(3);
					}


			}else if(NumSenial == SIGUSR2){
				
				printf("(SOMELIER) ¡Alto y Claro Chef!\n");
				printf("(SOMELIER) ¡Mozo! ¡Vaya a buscar vino por favor!\n");

				int error;

				error = kill(mozo,SIGPIPE);

					if(error == -1){
						perror("(SOMELIER) Error en el envío de la señal SIGPIPE al Mozo\n");	
					}

				PIDMozo = wait(&estado);//se queda en espera a que termine el mozo la tarea

					if(PIDMozo == -1 ){
						perror("(SOMELIER) Se ha producido un error durante la llamada a wait: Mozo\n");
					}

				encontrado = WEXITSTATUS(estado);

					if(encontrado == 0){
						printf("(SOMELIER) ¡Chef, no hemos encontrado el vino!\n");
						exit(1);
					}else{
						printf("(SOMELIER) ¡Chef, ya tenemos el vino!\n");
						exit(3);
					}



			}
	}
}




void manejadoraPinches(int NumSenial){ 

	int numeroAleatorioDormirPinches;
	int cocinadoBien;
	srand(getpid());
	numeroAleatorioDormirPinches = calculaAleatorios(2,3); //Uso 2 y 3 y no 2,5 porque lo completo luego con otro aleatorio entre 0 y 2 de manera que en el maximo de los casos es 3+2= 5

	sleep(numeroAleatorioDormirPinches+(calculaAleatorios(0,2))); //le sumo un aleatorio entre 0 y 2 para que haya menos posibilidades de que a dos pinches les salga el mismo tiempo por ejemplo

	cocinadoBien = calculaAleatorios(0,1);
						
		if(cocinadoBien == 1){
			printf("(PINCHE) Plato bien cocinado\n");
		}else{
			printf("(PINCHE) Plato mal cocinado\n");
		}


	exit(cocinadoBien); 	//Cada proceso pinche se muere, retornando el valor de si el plato está bien cocinado o no 


}


void manejadoraJefeSala(int NumSenial){


	printf("(JEFE DE SALA) <<Se afana en montar las mesas>>\n");
	sleep(3);
	printf("(JEFE DE SALA) ¡He acabado de montar las mesas Chef!\n");
	exit(0); 

}




//Entrada a la ejecucución de mi programa

int main(int argc, char * argv[]){

	pid_t miPid;
	pid_t PIDSomelier;
	pid_t PIDPincheMalPlato;
	pid_t jefeDeSala;
	int pinchesCocina;
	int numeroHijosChef;
	int *plantilla;
	int tamanio;
	int numeroAleatorioIngredientes;	
	int estado;
	int valorRetorno;
	int status;
	int platoTerminado;
	int contador;
	int mesasMontadas;
	struct sigaction pinches;
	struct sigaction jefeSala;



	//le asigno la manejadora al sa_handler de la estructura
	pinches.sa_handler = manejadoraPinches;
	jefeSala.sa_handler = manejadoraJefeSala;
	
	printf("RESTAURANTE EL GRAN BOCADO\n");
	printf("\n");

	pinchesCocina = atoi(argv[1]);		//trasformo el numero de pinches de cocina que me llega como argumento a entero
	srand(getpid());		//inicio una semilla para la funcion de los números aleatorios

	//hay que crear tantos pinches como se indique, hijos del chef y ademas un sommelier y un jefe de sala
	//el mozo de los recados es hijo del somelier
	
	tamanio = pinchesCocina+2;		//+2 Por el jefe de sala y el somelier
	plantilla = (int *)malloc(tamanio*sizeof(int));		//reservo memoria para el puntero en el que almaceno los PID de los hijos



	//el mozo lo creo en la manejadora del somelier, pero el jefe de sala va en el for 
	//para matar a los pinches haria un for desde 2 hasta el final teniendo por sentado que el 0 y 1 son el jefe de sala y el Somelier
 

	
	numeroHijosChef = (pinchesCocina+2); //los que recibo como parámetro mas el Jefe de sala y el Sommelier

		//HACEMOS EL FOR QUE CREA A TODOS LOS HIJOS DEL CHEF

	printf("(CHEF) Soy el chef y mi PID es %d\n", getpid());
		
		for(int i = 0; i < numeroHijosChef; i++){

			miPid = fork();
			
				if(miPid == -1){

					 perror("(CHEF) Error en la llamada al fork\n");

				}else if(miPid == 0){
						/* Código que ejecuta el proceso hijo */

					//Hago un if separando y especificando cada iteración del for para poner la traza del hijo que es y poder ponerle su código específico, puesto que se debe mostrar un mensaje (p.ej: el 0 es el Somelier)

					if(i == 0){

						printf("(SOMELIER) Soy el Somelier y mi PID es %d\n", getpid());
						
						//NOTA: Si se utiliza sigaction aquí para enmascarar la SIGUSR1 y SIGUS2 ocurre que la ejecucion se queda trabada en la manejadora del sommelier y se bloquea el envío de la señal SIGPIPE al mozo,(incluso si se manda desde otra terminal) es decir, he tenido que utilizar signal para que permita enviar la señal SIGPIPE al mozo 
							
							//Gestiono si se produce algún error
							if(signal(SIGUSR1, manejadoraSomelier) == SIG_ERR){
								perror("(SOMELIER) Error en la llamada a signal\n");
							}

							if(signal(SIGUSR2, manejadoraSomelier) == SIG_ERR){
								perror("(SOMELIER) Error en la llamada a signal\n");								
							}
						
						//Con pause() la ejecucion del proceso queda suspendida a la espera de una señal
						
						pause();						
																		
										

					}else if( i == 1){

						printf("(JEFE DE SALA) Soy el Jefe de Sala y mi PID es %d\n", getpid());

						int error;

						error = sigaction(SIGUSR1, &jefeSala, NULL);
						
						//Tratamos la posibilidad de que se produzca un error al enmascarar la señal

							if(error == -1){
								perror("(JEFE DE SALA) Error en la llamada a sigaction\n");	
							}
							
						pause();						
																								

												

					}else{ 		//i a partir de 2, por eso le resto 1 para que el numeo de pinche quede bien
						
						printf("(PINCHE %d) Soy el Pinche %d y mi PID es %d\n",(i-1), (i-1), getpid());
						
						int error1;

						error1 = sigaction(SIGUSR2, &pinches, NULL);
			
							if(error1 == -1){
								perror("(PINCHE) Error en la llamada a sigaction\n");
							}

						pause();						
		
							
						
					}
					
				}else{
						/* Código que ejecuta el proceso padre */

					*(plantilla+i) = miPid; //el padre guarda en el vector los pid de sus hijos, para mas adelante en el programa poder tener los pid y enviarles la señal de matar proceso y saber a cuál
				}


		}

	/* CÓDIGO COMÚN PERO QUE AL ESTAR LOS DEMÁS PROCESOS EN ESPERA DE SEÑALES NO LLEGAN AQUÍ Y SOLO LO EJECUTA EL CHEF */
	
	sleep(3);
	
	numeroAleatorioIngredientes = calculaAleatorios(0,1);		//Se genera un aleatorio entre 0 y 1
	
	printf("(CHEF) Bien, Voy a comenzar a preparar los platos del día\n");
	printf("(CHEF) Mmmmm... Lo primero deberé asegurarme de que tengo todos los ingredientes y el vino\n");
 
		if(numeroAleatorioIngredientes == 0){ 		//Si 0 le faltan ingredientes

			//necesito sacar el pid del somelier para mandarle la señal. Al crearlo en el for, sé que el fork() devuelve en el código del padre de esa iteracion el pid del hijo, y sé que estará en la posicion 0 del vector plantilla.
			printf("(CHEF) ¡Porras! ¡Me faltan ingredientes! ¡Somelier!\n");
			int error;

			error = kill(*(plantilla+0), SIGUSR1);

				if(error == -1){
					perror("(CHEF) Error en el envío de la señal SIGUSR1 al Somelier\n");
				}
			

			PIDSomelier = wait(&estado); 		//el chef espera a que el somelier le diga de vuelta lo que ha encotrado o no el mozo
			
			
			
			valorRetorno = WEXITSTATUS(estado);
			
			//EL SOMELIER HA MUERTO, CON LO CUAL TENEMOS EN MENTE QUE LA PRIMERA POSICION DEL PUNTERO YA TIENE UN PID QUE NO VALE





				if(PIDSomelier == -1){
					perror("(CHEF) Se ha producido un error durante la llamada a wait: Somelier\n");
				}

				if(valorRetorno == 2){
					printf("(CHEF) Muy bien equipo, no pasa nada, nos faltan ingredientes\n");

				}else{
					printf("(CHEF) Muy bien equipo, tenemos los ingredientes\n");

				}


		}else{ 		//Si 1 solamente le falta el vino

			printf("(CHEF) ¡Diantres! ¡Pero si no me queda vino! ¡Somelier!\n");

			int error;

			error = kill(*(plantilla+0), SIGUSR2);

				if(error == -1){
					perror("(CHEF) Error en el envío de la señal SIGUSR2 al Somelier\n");
				}
			


			PIDSomelier = wait(&estado);			//el chef espera a que el somelier le diga de vuelta lo que ha encotrado o no el mozo
			valorRetorno = WEXITSTATUS(estado);
			
			//EL SOMELIER HA MUERTO, CON LO CUAL TENEMOS EN MENTE QUE LA PRIMERA POSICION DEL PUNTERO YA TIENE UN PID QUE NO VALE





				if(PIDSomelier == -1){
					perror("(CHEF) Se ha producido un error durante la llamada a wait: Somelier\n");
				}

				if(valorRetorno == 1){

					printf("(CHEF) Muy bien equipo, nos falta vino, Cerramos hoy.\n");
						//SE ACABA EL PROGRAMA MATANDO A TODOS LOS HIJOS DEL CHEF QUE QUEDAN VIVOS, QUE SERÍAN LOS PINCHES Y EL JEFE DE SALA
						
						for (int i = 1 ; i < numeroHijosChef; i++){ //Desde 1 porque el somelier ya murió
							int error;
							error = kill(*(plantilla+i), SIGKILL);

								if(error == -1){
									perror("(CHEF) Error al intentar matar a los hijos vivos\n");
								}
						
						}

					exit(0); //se acaba el programa

				}else{
					printf("(CHEF) Muy bien equipo, tenemos Vino\n");

				}



		}






	/* EN EL CASO DE QUE O FALTEN INGREDIENTES O NO FALTE NADA SEGUIMOS LA EJECUCION DEL CHEF */

	printf("(CHEF) ¡¡Pinches!! ¡Comenzad a preparar los platos por favor!\n"); 

		//MANDO LAS SEÑALES TODAS SEGUIDAS A LOS PINCHES PARA QUE SE PONGAN A HACER LOS PLATOS

		for(int i = 2; i < (pinchesCocina+2); i++){ 	//porque el pid de los pinches está en el puntero de la posicion 2 en adelante, con lo cual si hay 5 pinches será de la 2 a la 7 sin incluir la ultima como se indica con el menor.
			int error;
			error = kill(*(plantilla+i), SIGUSR2); 		//les digo que dejen de estar pausados y se pongan a trabajar usando de nuevo la señal sigusr2 pero enmascarada en cada pinche (si la pongo en el main me la enmascara el chef, y ya tengo 1 sigus1 enmascarada por el chef)
				if(error == -1){
					perror("(CHEF) Error en el envío de la señal SIGUSR2 a los pinches\n");
				}
		}

		//EL CHEF QUEDA A LA ESPERA DE QUE TODOS LOS PINCHES ACABEN O NO LOS PLATOS, Y REGISTRA EL NUMERO DE PLATOS ACABADOS.
		//EN EL MOMENTO QUE UNO ACABE SE HACE TODO LO DE UNA ITERACION Y SE VUELVE A ESPERAR Y CUANDO ACABE OTRO IGUAL Y ASÍ HASTA QUE ACABEN TODOS
	
	contador = 0;		//inicio un contador de platos bien cocinados		

		for(int i = 0; i < pinchesCocina; i++){ 		//tiene que esperar por la situacion de fin de todos los pinches
		PIDPincheMalPlato = wait(&status);
		platoTerminado = WEXITSTATUS(status);
			
			if(PIDPincheMalPlato == -1){ 		//si saliese -1 indicaría que no quedan hijos vivos por los que esperar.
				perror("(CHEF) Se ha producido un error durante la llamada a wait: Pinches\n");

			}
			if(platoTerminado == 1){
				//plato bien cocinado y lo cuenta el chef
				contador++;
			}
		}

	//una vez recogido si los pinches cocinan o no bien el plato y contados los platos bien cocinados, si el numero de platos bien cocinados es 0 se cierra el restaurante

		if(contador == 0){
			//no hay ningún plato bien cocinado
			printf("(CHEF) ¡¡Horror!! ¡no hay ningún plato bien cocinado!\n");
			printf("(CHEF) Cerramos Hoy\n");
			printf("(CHEF) <<Cerrando Restaurante>>\n");
			
			//se mata al jefe de sala que es el unico hijo del chef que quda vivo (y cuyo pid corresponde con la posicion 1 del puntero plantilla)
			int error;
			
			error = kill(*(plantilla+1), SIGKILL);

				if(error == -1){
					perror("(CHEF) Error al intentar matar al jefe de Sala\n");
				}


		}else{
			//Hay al menos un plato bien cocinado
			//imprime el número de platos bien cocinados
			printf("(CHEF) El número de platos completados es %d\n", contador);

		//se avisa al jefe de sala (SIGUSR1) para que monte las mesas	
			printf("(CHEF) ¡¡Jefe De Sala!! Ya puede montar las mesas, ¡¡apresurese!! ¡¡los clientes están a punto de llegar!!\n");

			int error;			
			error = kill(*(plantilla+1), SIGUSR1);	
				if(error == -1){
					perror("(CHEF) Error en el envío de la señal SIGUSR1 al Jefe de Sala\n");
				}
			
			
			jefeDeSala = wait(NULL); //pongo null porque no nos interesa el valor de retorno, simplemente que se lleve a cabo la accion de montar las mesas
			
				if(jefeDeSala == -1){
					perror("(CHEF) Se ha producido un error durante la llamada a wait: Jefe de Sala\n");
				
				}
			
			printf("(CHEF) PUEDE ABRIRSE EL RESTAURANTE\n");
		}



free(plantilla); //libero la memoria correspondiente al puntero que contenía los pid de la plantilla del restaurante


return 0;
}


//FUNCIONES DEL PROGRAMA


int calculaAleatorios(int min, int max){

	return rand() % (max-min+1) + min;

}
