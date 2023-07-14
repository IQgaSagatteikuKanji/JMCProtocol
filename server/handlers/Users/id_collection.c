#include "id_collection.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void id_collection_init(struct id_collection *icol){
    assert(icol != NULL);

    icol->ids = NULL;
    icol->number_of_ids = 0u;
    icol->allocated_for = 0u;
}

void id_collection_destroy(struct id_collection *icol){
    assert(icol != NULL);

    free(icol->ids);
    id_collection_init(icol);
}

bool icol_contains(struct id_collection *icol, uint32_t id){
    bool contains = false;

    for(uint32_t i = 0u; i < icol->number_of_ids && !contains; i++){
        contains |= icol->ids[i] == id;
    }

    return contains;
}

void icol_add_user(struct id_collection *icol, uint32_t id){
    assert(icol != NULL);
    
    // add more space if there isnt enough
    uint32_t required_space = icol->number_of_ids + 1u;
    if(required_space == 0){
        printf("Exceeded id storage capacity. This needs to be fixed. Please revisit the implementation of id_collection \n");
        exit(1);
    }

    if(required_space > icol->allocated_for){
        icol_allocate_for(icol,required_space);
    }

    // add user to the collection
    icol->ids[icol->number_of_ids] = id;
    icol->number_of_ids++;
}


void icol_fill_in_space(struct id_collection *icol, uint32_t starting_from){
    
    for(uint32_t i = starting_from; i < icol->number_of_ids - 1; i++){
        icol->ids[i] = icol->ids[i + 1];
    }
    icol->number_of_ids--;
}

void icol_remove_user(struct id_collection *icol, uint32_t id){
    assert(icol != NULL);

    int64_t target = -1;

    for(uint32_t i = 0; i < icol->number_of_ids && target < 0; i++){
        if(icol->ids[i] == id){
            target = i;
        }
    }

    if(target >= 0){
        icol_fill_in_space(icol, target);
    }
}

#define MAX(x, y) (x > y ? x : y)
#define NEXT_SIZE_OF_ARRAY_FOR(x) (2 * x)

void icol_allocate_for(struct id_collection *icol, uint32_t ids){
    assert(icol != NULL);

    // max is in case it overflows
    uint32_t new_size = NEXT_SIZE_OF_ARRAY_FOR(ids);
    new_size = MAX(ids, new_size);

    icol->ids = realloc(icol->ids, new_size * sizeof(uint32_t));
}

bool icol_is_valid_entry(struct id_collection *icol, uint32_t id){
    assert(icol != NULL);

    return id < icol->number_of_ids;
}