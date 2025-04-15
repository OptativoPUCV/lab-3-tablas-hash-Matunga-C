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

    return NULL;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {   


    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
