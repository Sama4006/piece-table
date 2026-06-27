#include "piecetable.h"
#include <string.h>
#include <stdio.h>

// Helper: Manage the Add Buffer
static void sb_append(StringBuffer *sb, const char *text) {
    int len = strlen(text);
    if (sb->length + len >= sb->capacity) {
        sb->capacity = (sb->capacity * 2) + len;
        sb->data = (char*)realloc(sb->data, sb->capacity);
    }
    memcpy(sb->data + sb->length, text, len);
    sb->length += len;
}

PieceTable* create_piece_table(const char *initial_text) {
    PieceTable *pt = (PieceTable*)malloc(sizeof(PieceTable));
    
    // 1. Setup Original Buffer
    pt->orig_len = strlen(initial_text);
    pt->original_buffer = (char*)malloc(pt->orig_len + 1);
    strcpy(pt->original_buffer, initial_text);

    // 2. Setup Add Buffer
    pt->add_buffer.capacity = 1024;
    pt->add_buffer.length = 0;
    pt->add_buffer.data = (char*)malloc(pt->add_buffer.capacity);

    // 3. Setup Pieces Array
    pt->pieces = make_dynarr();

    // 4. Initial Piece
    if (pt->orig_len > 0) {
        Piece p = {BUFFER_ORIGINAL, 0, pt->orig_len};
        append(pt->pieces, p);
    }

    return pt;
}

void pt_insert(PieceTable *pt, int pos, const char *text) {
    // 1. Store text in Add Buffer
    int add_start = pt->add_buffer.length;
    sb_append(&pt->add_buffer, text);
    int add_len = strlen(text);
    
    Piece new_piece = {BUFFER_ADD, add_start, add_len};

    // 2. Find the piece to split
    int current_pos = 0;
    int piece_idx = -1;
    int offset = 0;

    for (int i = 0; i < pt->pieces->size; i++) {
        Piece p = get(pt->pieces, i);
        if (current_pos + p.length > pos) {
            piece_idx = i;
            offset = pos - current_pos;
            break;
        }
        current_pos += p.length;
    }

    // 3. Execute Logic
    if (piece_idx == -1) {
        // Append to end
        append(pt->pieces, new_piece);
    } 
    else if (offset == 0) {
        // Insert before this piece
        insert_at(pt->pieces, piece_idx, new_piece);
    } 
    else {
        // SPLIT Logic
        Piece original_p = get(pt->pieces, piece_idx);
        
        // Left part (modify in place)
        Piece left_p = original_p;
        left_p.length = offset;
        set(pt->pieces, piece_idx, left_p);

        // Right part (create new)
        Piece right_p = original_p;
        right_p.start += offset;
        right_p.length -= offset;

        // Insert New Middle, then Right Part
        insert_at(pt->pieces, piece_idx + 1, new_piece);
        insert_at(pt->pieces, piece_idx + 2, right_p);
    }
}

char* pt_get_text(PieceTable *pt) {
    // Calculate total size
    int total_len = 0;
    for(int i=0; i<pt->pieces->size; i++) {
        total_len += get(pt->pieces, i).length;
    }

    char *result = (char*)malloc(total_len + 1);
    int cursor = 0;

    for(int i=0; i<pt->pieces->size; i++) {
        Piece p = get(pt->pieces, i);
        char *src_buf = (p.source == BUFFER_ORIGINAL) ? pt->original_buffer : pt->add_buffer.data;
        memcpy(result + cursor, src_buf + p.start, p.length);
        cursor += p.length;
    }
    result[total_len] = '\0';
    return result;
}

void pt_free(PieceTable *pt) {
    free(pt->original_buffer);
    free(pt->add_buffer.data);
    free_arr(pt->pieces);
    free(pt->pieces);
    free(pt);
}