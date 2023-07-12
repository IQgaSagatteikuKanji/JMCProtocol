#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "private_chat.h"

// exluding the constructor and the destructor functions, functions that operate on pc_collection start with pccol
// the constructor and the destructor are named as per contract 
struct pc_collection{
    struct private_chat *pcs;
    uint32_t count;
    uint32_t allocated_for;
};


void pc_collection_init(struct pc_collection *pccol);
void pc_collection_destroy(struct pc_collection *pccol);

// Doesn't care about the order of ids
struct private_chat *pccol_find_chat_by_two_users(struct pc_collection *pccol, uint32_t fid, uint32_t sid);

void pccol_add_pchat(struct pc_collection *pccol, struct private_chat *pc);
void pccol_remove_pchat(struct pc_collection *pccol, struct private_chat *pc);
bool pccol_contains_pchat(struct pc_collection *pccol, struct private_chat *pc);

void pccol_alocate_for(struct pc_collection *pccol, uint32_t chats);