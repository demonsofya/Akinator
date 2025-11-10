#include <stdio.h>

#include "akinator_struct.h"
#include "akinator_dump.h"

int main()
{

    Tree_t *tree = TreeCtor();

    while (true) {
        fprintf(OUTPUT_FILE, "Guess your character[g]\nPrint object definition[p]\n");

        int curr_answer = 0;
        scanf(" %c", &curr_answer);
        //getchar();
ON_DEBUG(fprintf(stderr, "curr answer is %c\n", curr_answer));

        if (curr_answer == 'g')
            FindObject(tree);

        else if (curr_answer == 'p')
            GiveDefinitionForObject(tree);

        else
            break;
    }
    TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "", NoTreeError);
    return 0;
}
