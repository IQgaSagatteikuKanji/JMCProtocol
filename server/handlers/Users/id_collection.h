#pragma once

#include <stdint.h>
#include <stdbool.h>


// I plan to substitute it for hashtable later 
// Do I need to make an iterator?
struct id_collection{
    uint32_t *ids;
    uint32_t number_of_ids;
    uint32_t allocated_for;
};


void id_collection_init(struct id_collection *icol);
void id_collection_destroy(struct id_collection *icol);

bool icol_contains(struct id_collection *icol, uint32_t id);

void icol_add_user(struct id_collection *icol, uint32_t id);
void icol_remove_user(struct id_collection *icol, uint32_t id);

void icol_allocate_for(struct id_collection *icol, uint32_t ids);

//checks that hashtable will need, but this realisation doesn't require
bool icol_is_valid_entry(struct id_collection *icol, uint32_t id);
