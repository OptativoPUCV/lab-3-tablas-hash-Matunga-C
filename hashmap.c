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

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


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
    if(map == NULL) return NULL;
    for(size_t i = 0; i < map->capacity; i++){ //recorre la tabla
        if(map->buckets[i] != NULL){ //si encuentra un par
            map->current = i; //actualiza el indice del ultimo dato accedido
            return map->buckets[i]; //devuelve el par encontrado
        }
    }
    return NULL; //si no se encuentra ningun par, devuelve NULL
}

Pair * nextMap(HashMap * map) {
    if(map == NULL) return NULL;
    for(long i = map->current + 1; i < map->capacity; ++i){
        if(map->buckets[i] != NULL){
            map->current = i; //actualiza el indice del ultimo dato accedido
            return map->buckets[i]->value; //devuelve el par encontrado
        }
    }
    return NULL;
}
