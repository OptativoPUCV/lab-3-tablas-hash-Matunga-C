#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if(map == NULL) return;
    if(key == NULL) return;
    if(value == NULL) return;
    if(map->size >= map->capacity) enlarge(map); //si la tabla esta llena, se agranda
    long index = hash(key, map->capacity);
    Pair * new = createPair(key, value);
    while(map->buckets[index] != NULL){ //busca un espacio libre
        if(is_equal(map->buckets[index]->key, key)){ //si la clave ya existe, se reemplaza el valor
            free(map->buckets[index]); //libera el espacio de la clave anterior
            break;
        }
        index = (index + 1) % map->capacity; //busca el siguiente espacio libre
    }
    map->buckets[index] = new; //inserta el nuevo par en la tabla
    map->size++;
    map->current = index; //actualiza el indice del ultimo dato accedido       
}

/*a - Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets (*old_buckets*);

b - Duplique el valor de la variable capacity.

c - Asigne a map->buckets un nuevo arreglo con la nueva capacidad.

d - Inicialice size a 0.

e - Inserte los elementos del arreglo *old_buckets* en el mapa (use la función insertMap que ya implementó). */

void enlarge(HashMap * map) {
    if(map == NULL) return;
    if(map->capacity == 0) return;
    if(map->size == 0) return;
    Pair ** old_buckets = map->buckets; //crea una variable auxiliar para guardar el arreglo de pares
    long old_capacity = map->capacity; //guarda la capacidad anterior
    map->capacity *= 2; //duplica la capacidad
    map->buckets = (Pair **)malloc(sizeof(Pair *) * map->capacity); //crea el nuevo arreglo de pares
    map->size = 0; //inicializa la cantidad de datos en la tabla
    for(int i = 0; i < map->capacity; ++i){
        map->buckets[i] = NULL; //inicializa los espacios de la tabla en NULL
    }
    for(int i = 0; i < old_capacity; ++i){ //recorre el arreglo anterior
        if(old_buckets[i] != NULL){ //si el par no es NULL
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value); //inserta el par en el nuevo arreglo
            free(old_buckets[i]); //libera el espacio del par anterior
        }
    }
    free(old_buckets); //libera el espacio del arreglo anterior
    enlarge_called=1; //indica que se llamo a la funcion enlarge
}


HashMap * createMap(long capacity) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap)); //crea la tabla
    map->buckets = (Pair **)malloc(sizeof(Pair *) * capacity); //crea el espacio para los pares
    map->size = 0; //inicializa la cantidad de datos en la tabla
    map->capacity = capacity; //inicializa la capacidad de la tabla
    map->current = -1; //inicializa el indice del ultimo dato accedido
    for(int i = 0; i < capacity; ++i){
        map->buckets[i] = NULL; //inicializa los espacios de la tabla en NULL
    }
    return map; //devuelve la tabla creada
}

void eraseMap(HashMap * map,  char * key) {    
    if(map == NULL) return;
    if(key == NULL) return;
    long index = hash(key, map->capacity); //busca el indice de la clave
    while(map->buckets[index] != NULL){ //mientras haya un par en la tabla
        if(is_equal(map->buckets[index]->key, key)){ //si la clave coincide
            map->buckets[index]->key = NULL; //elimina la clave
            map->buckets[index]->value = NULL; //elimina el valor
            map->size--; //disminuye la cantidad de datos en la tabla
            return; //sale de la funcion
        }
        index = (index + 1) % map->capacity; //busca el siguiente espacio
    }
}

Pair * searchMap(HashMap * map,  char * key) {
    if(map == NULL) return NULL;
    if(key == NULL) return NULL;
    long index = hash(key, map->capacity); //busca el indice de la clave
    while(map->buckets[index] != NULL){ //mientras haya un par en la tabla
        if(is_equal(map->buckets[index]->key, key)){ //si la clave coincide
            map->current = index; //actualiza el indice del ultimo dato accedido
            return map->buckets[index]; //devuelve el par encontrado
        }
        index = (index + 1) % map->capacity; //busca el siguiente espacio
    }
    return NULL; //si no se encuentra la clave, devuelve NULL
}

Pair * firstMap(HashMap * map) {  
    if (map == NULL || map->capacity == 0) return NULL; // si la tabla es NULL o vacia
    for (size_t i = 0; i < map->capacity; i++) { // recorre la tabla
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) { // si el par no es NULL 
            map->current = i; // actualiza el indice del ultimo dato accedido
            return map->buckets[i]; // devuelve el primer par encontrado
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    if (map == NULL || map->capacity == 0) return NULL; // si la tabla es NULL o vacia
    for (long i = map->current + 1; i < map->capacity; ++i) { // recorre la tabla desde el siguiente indice
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) { // si el par no es NULL
            map->current = i; // actualiza el indice del ultimo dato accedido
            return map->buckets[i]; // devuelve el siguiente par encontrado
        }
    }
    return NULL;
}

