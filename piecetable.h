#ifndef PIECETABLE_H
#define PIECETABLE_H

#include "dynarr.h"

// Simple Append-Only String Buffer
typedef struct {
    char *data;
    int length;
    int capacity;
} StringBuffer;

// The Piece Table Structure
typedef struct {
    char *original_buffer;   // Read-Only
    int orig_len;
    StringBuffer add_buffer; // Append-Only
    PieceArr *pieces;        // The "View" of the document
} PieceTable;

PieceTable* create_piece_table(const char *initial_text);
void pt_insert(PieceTable *pt, int pos, const char *text);
char* pt_get_text(PieceTable *pt);
void pt_free(PieceTable *pt);

#endif