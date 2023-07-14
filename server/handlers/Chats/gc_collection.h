#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "group_chat.h"

// exluding the constructor and the destructor functions, functions that operate on gc_collection start with gccol
// the constructor and the destructor are named as per contract 
struct gc_collection{
    struct group_chat *gcs;
    uint32_t count;
    uint32_t allocated_for;
};

void gc_collection_init(struct gc_collection *gccol);
void gc_collection_destroy(struct gc_collection *gccol);

// Doesn't care about the order of ids
struct group_chat *gccol_find_chat_by_group_id(struct gc_collection *gccol, uint32_t fid);

//transfers ownership over fields of group_chat
void gccol_add_gchat(struct gc_collection *gccol, struct group_chat *gc);
void gccol_remove_gchat(struct gc_collection *gccol, struct group_chat *gc);
bool gccol_contains_gchat(struct gc_collection *gccol, struct group_chat *gc);

void gccol_alocate_for(struct gc_collection *gccol, uint32_t chats);