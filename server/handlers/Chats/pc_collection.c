#include "pc_collection.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#define MAX(x, y) (x > y ? x : y)
#define NEXT_SIZE_OF_ARRAY_FOR(x) (2 * x)

void pc_collection_init(struct pc_collection *pccol){
    assert(pccol != NULL);

    pccol->count = 0u;
    pccol->allocated_for = 0u;
    pccol->pcs = NULL;
}

void pc_collection_destroy(struct pc_collection *pccol){
    assert(pccol != NULL);

    free(pccol->pcs);
    pc_collection_init(pccol);
}

// Doesn't care about the order of ids

//I dont know if I need to move it to private_chat.h, Ill decide it later
bool pc_users_are(struct private_chat *pc, uint32_t fid, uint32_t sid){
    assert(pc != NULL);

    return (pc->users[0] == fid && pc->users[1] == sid)
            || (pc->users[1] == fid && pc->users [0] == sid);
}

struct private_chat *pccol_find_chat_by_two_users(struct pc_collection *pccol, uint32_t fid, uint32_t sid){
    assert(pccol != NULL);
    
    for (uint32_t i = 0; i < pccol->count; i++){
        if (pc_users_are(pccol->pcs + i, fid, sid)){
            return pccol->pcs + i;
        }
    }

    return NULL;
}

struct private_chat *pccol_add_pchat(struct pc_collection *pccol, struct private_chat *pc){
    assert(pccol != NULL);
    assert(pc != NULL);

    // add more space if there isnt enough
    uint32_t required_space = pccol->count + 1u;
    if (required_space == 0){
        printf("Exceeded user storage capacity. This needs to be fixed. Please revisit the implementation of user_collection \n");
        exit(1);
    }

    if (required_space > pccol->allocated_for){
        pccol_alocate_for(pccol, required_space);
    }

    // add pc to collection
    pccol->pcs[pccol->count] = *pc;
    pccol->count++;
    return pccol->pcs + (pccol->count - 1);
}

void pccol_fill_in_space(struct pc_collection *pccol, uint32_t starting_from){

    for (uint32_t i = starting_from; i < pccol->count - 1; i++){
        pccol->pcs[i] = pccol->pcs[i + 1];
    }
    pccol->count--;
}

void pccol_remove_pchat(struct pc_collection *pccol, struct private_chat *pc){
    assert(pccol != NULL);

    if (pc != NULL){
        int64_t target = -1;

        for (uint32_t i = 0; i < pccol->count && target < 0; i++){
            if (pc_users_are(pccol->pcs + i, pc->users[0], pc->users[1])){
                target = i;
            }
        }

        if (target >= 0){
            private_chat_destroy(pccol->pcs + target);
            pccol_fill_in_space(pccol, target);
        }
    }
}

bool pccol_contains_pchat(struct pc_collection *pccol, struct private_chat *pc){
    assert(pccol != NULL);
    assert(pc != NULL);

    return pccol_find_chat_by_two_users(pccol, pc->users[0], pc->users[1]) != NULL;
}

void pccol_alocate_for(struct pc_collection *pccol, uint32_t chats){
    assert(pccol != NULL);
    assert(pccol->count <= chats);

    // max is in case it overflows
    uint32_t new_size = NEXT_SIZE_OF_ARRAY_FOR(chats);
    new_size = MAX(chats, new_size);

    pccol->pcs = realloc(pccol->pcs, new_size * sizeof(struct private_chat));
    pccol->allocated_for = new_size;
}

struct private_chat *create_new_private_chat_between_users(struct pc_collection *pcs, uint32_t fid, uint32_t sid){
    struct private_chat pc;
    private_chat_init(&pc);

    pc_set_recipients(&pc, fid, sid);
    return pccol_add_pchat(pcs, &pc);
}