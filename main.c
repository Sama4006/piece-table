#include <stdio.h>
#include <stdlib.h>
#include "piecetable.h"

int main() {
    printf("=== PIECE TABLE PROJECT ===\n");

    // 1. Initialize
    printf("\n[Step 1] Loading file with 'Hello World'\n");
    PieceTable *pt = create_piece_table("Hello World");
    
    char *txt = pt_get_text(pt);
    printf("Result: \"%s\"\n", txt);
    free(txt);

    // 2. Insert Middle (Split)
    printf("\n[Step 2] Inserting 'Beautiful ' at index 6\n");
    pt_insert(pt, 6, "Beautiful ");
    
    txt = pt_get_text(pt);
    printf("Result: \"%s\"\n", txt);
    free(txt);
    
    // Debug view to show the professor the pieces
    dynarrprint(pt->pieces); 

    // 3. Insert End
    printf("\n[Step 3] Inserting '!!!' at the end\n");
    // Calculate end length manually or track it
    int end_pos = 0;
    for(int i=0; i<pt->pieces->size; i++) end_pos += get(pt->pieces, i).length;
    
    pt_insert(pt, end_pos, "!!!");
    
    txt = pt_get_text(pt);
    printf("Result: \"%s\"\n", txt);
    free(txt);

    // Cleanup
    pt_free(pt);
    return 0;
}