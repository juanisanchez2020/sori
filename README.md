# Sistemas Operativos y Redes

## Trabajo Práctico de hilos y semáforos

En el presente trabajo se pide simular una competencia entre tres equipos de cocineros en C, donde cada uno debe realizar una hamburguesa para entregar al jurado, el primero en hacerlo será el ganador. Además la receta que deben seguir los equipos deberá ser leida desde un archivo .txt y se debe escribir en un nuevo archivo .txt los pasos de la competencia.

Veamos la receta para armar una hamburguesa:

## 1. Cortar:
2 Dientes de ajo
1 Cebolla
Perejil
Lechuga
Tomate

## 2. Mezclar
Ajo, cebolla, perejil y 1/2kg de carne picada

## 3. Salar Carne

## 4. Armar Medallones

## 5. Cocinar 5' a la plancha

## 6. Hornear pan 10'

## 7. Armar Hamburguesa

El problema es que la cocina no tiene equipamiento individual, los equipos deberan ordenarse para poder utilizar los elementos para cocinar. La cocina cuenta con un solo salero, una plancha para cocinar los medallones y un solo horno. Tambien hay que tener que pasos no pueden hacerse sin terminar otro antes y cuales son los que no dependen de pasos previos.

De acuerdo con el enunciado antes de armar un medallón primero hay que cortar los dientes de ajo, la cebolla y el perejil, luego se deben mezclar con la carne, salar y recien despues de todo esto armar el medallón. A su vez, para armar una hamburguesa, primero se debe cocinar el medallon, hornear el pan y cortar la lechuga y el tomate.

Para resolver todas estas  complicaciones se utilizaron hilos para simular los equipos y semáforos para sincronizar la receta y el uso de los elementos compartidos entre los equipos. El programa cuenta con un main donde se crean los hilos de los tres equipos, se sincronizan y llaman a la función ejecutarReceta(). Dicha función cuenta con un semaforo por cada acción, y un hilo por cada paso de la receta a ejecutar. Ademas setea las acciones y sus ingredientes levantandolos desde el archivo receta.txt y luego cada hilo llama a la función correspondiente.
Dentro de estas funciones que simulan los pasos de la receta, es donde se encuentran los semáforos para garantizar el correcto funcionamiento del programa y que mas de un proceso no intente utilizar un recurso a la vez.

Veamos como quedaron en pseudocodigo las funciones de la receta:

### cortarIngredientes {
	
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);
		sem_post(signal a accion mezclar);
		
### };

### mezclar{
		
		sem_wait(evalua si hay signal en semaforo mezclar);
    
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);
    
		sem_post(signa a mutex salar);
### };

### salar {

		sem_wait(evalua si mutex salar tiene signal);
    
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);

		sem_post(signal a mutex salar);
		sem_post(signal a armar medallon);
### };

### armarMed {

		sem_wait(evalua si armarMed tiene signal);
    
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);

		sem_post(signal a cocinarMed);
### };

### hornearPan {

		sem_wait(mutex para que un solo equipo utilice el horno);
    
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);

		sem_post(signal al semaforo hornearPan);
		sem_post(signal a mutex para que un solo equipo utilice el horno);
### };

### cocinarMed {
		
		sem_wait(evalua si el semaforo cocinarMed tiene signal);
		sem_wait(mutex para que un solo equipo utilice la plancha);
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);

		sem_post(signal a mutex para que un solo equipo utilice la plancha);
		sem_post(da signal al semaforo cocinarMed);
### };

### cortarExtras {

		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);	
### };

### armarHam {
	
		sem_wait(evalua si semaforo cocinarMed tiene signal);
		sem_wait(evalua si semaforo hornearPan tiene signal);
		sem_wait(evalua si semaforo cortarExtras tiene signal);
    
		sem_wait(semaforo para escribir en resultado.txt);  
		imprimirAccion;
		sem_post(semaforo para escribir en resultado.txt);
### };

Mientras los procesos ejecutan la receta, cada acción se va guardando en el archivo resultado.txt para que una vez finalizados, se pueda observar cual fue el equipo ganador.
