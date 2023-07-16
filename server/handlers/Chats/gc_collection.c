#include "gc_collection.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(x, y) (x > y ? x : y)
#define NEXT_SIZE_OF_ARRAY_FOR(x) (2 * x)


void gc_collection_init(struct gc_collection *gccol){
    assert(gccol != NULL);

    gccol->count = 0u;
    gccol->allocated_for = 0u;
    gccol->gcs = NULL;
}

void gc_collection_destroy(struct gc_collection *gccol){
    assert(gccol != NULL);

    free(gccol->gcs);
    gc_collection_init(gccol);    
}

// Doesn't care about the order of ids
struct group_chat *gccol_find_chat_by_group_id(struct gc_collection *gccol, uint32_t fid){
    assert(gccol != NULL);
    
    for (uint32_t i = 0; i < gccol->count; i++){
        if (gccol->gcs[i].chat_id == fid){
            return gccol->gcs + i;
        }
    }

    return NULL;    
}

void gccol_add_gchat(struct gc_collection *gccol, struct group_chat *gc){
    assert(gccol != NULL);
    assert(gc != NULL);

    // add more space if there isnt enough
    uint32_t required_space = gccol->count + 1u;
    if (required_space == 0){
        printf("Exceeded user storage capacity. This needs to be fixed. Please revisit the implementation of user_collection \n");
        exit(1);
    }

    if (required_space > gccol->allocated_for){
        gccol_alocate_for(gccol, required_space);
    }

    // add pc to collection
    gccol->gcs[gccol->count] = *gc;
    gc->chat_id = gccol->count;
    gccol->count++;
}

void gccol_fill_in_space(struct gc_collection *gccol, uint32_t starting_from){

    for (uint32_t i = starting_from; i < gccol->count - 1; i++){
        gccol->gcs[i] = gccol->gcs[i + 1];
    }
    gccol->count--;
}

void gccol_remove_gchat(struct gc_collection *gccol, struct group_chat *gc){
    assert(gccol != NULL);

    if (gc != NULL){
        int64_t target = -1;

        for (uint32_t i = 0; i < gccol->count && target < 0; i++){
            if (gccol->gcs[i].chat_id == gc->chat_id){
                target = i;
            }
        }

        if (target >= 0){
            group_chat_destroy(gccol->gcs + target);
            gccol_fill_in_space(gccol, target);
        }
    }
}

bool gccol_contains_gchat(struct gc_collection *gccol, struct group_chat *gc){
    assert(gccol != NULL);
    assert(gc != NULL);

    return gccol_find_chat_by_group_id(gccol, gc->chat_id) != NULL;
}

void gccol_alocate_for(struct gc_collection *gccol, uint32_t chats){
    assert(gccol != NULL);
    assert(gccol->count <= chats);

    // max is in case it overflows
    uint32_t new_size = NEXT_SIZE_OF_ARRAY_FOR(chats);
    new_size = MAX(chats, new_size);

    gccol->gcs = realloc(gccol->gcs, new_size * sizeof(struct group_chat));
    gccol->allocated_for = new_size;    
}