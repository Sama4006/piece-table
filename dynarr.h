#ifndef DYNARR_H
#define DYNARR_H

#include <stdlib.h>
#include <stdio.h>

// --- CORE STRUCT DEFINITION ---
typedef enum {
    BUFFER_ORIGINAL,
    BUFFER_ADD
} BufferType;

typedef struct {
    BufferType source; // Which buffer is this piece pointing to?
    int start;         // Start index in that buffer
    int length;        // Length of the text
} Piece;

// --- DYNAMIC ARRAY DEFINITION ---
typedef struct {
    Piece *data;
    int size;
    int capacity;
} PieceArr;

// Constructors
PieceArr* make_dynarr(void);
void init_dynarr(PieceArr *arr);
void free_arr(PieceArr *arr);

// Core Ops
void append(PieceArr *arr, Piece val);
void insert_at(PieceArr *arr, int index, Piece val); // NEW: Required for splitting
Piece get(PieceArr *arr, int idx);
void set(PieceArr *arr, int idx, Piece val);
int length(PieceArr *arr);
int pop(PieceArr *arr); // Returns 1 on success, 0 on fail (changed from int val)

// Utils
void dynarrprint(PieceArr *arr);

#endif