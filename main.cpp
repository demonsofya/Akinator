#include <stdio.h>

#include "akinator_struct.h"
#include "akinator_dump.h"

int main() {

    Tree_t *tree = TreeCtor();

    while (true) {
        fprintf(OUTPUT_FILE, "\nGuess your character [g]\n"
                             "Print object definition [p]\n"
                             "Save progress [s]\n"
                             "Load base from file [l]\n"
                             "Print difference between objects [d]\n"
                             "F to pay respects (end the programm)[f]\n\n");

        int curr_answer = 0;
        scanf(" %c", &curr_answer);
        Continue_If_Answer_Error();

        if (curr_answer == 's')
            PrintAkinatorBaseInOutputFile(tree);

        else if (curr_answer == 'g')
            FindObject(tree);

        else if (curr_answer == 'p')
            GiveDefinitionForObject(tree);

        else if (curr_answer == 'd')
            CompareTwoObjects(tree);

        else if (curr_answer == 'f')
            break;

        else if (curr_answer == 'l')
            tree->root = GetAkinatorBaseFromFile();

        else
            fprintf(OUTPUT_FILE, "\nNo such option.\n\n");
    }

    TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Tree dump after ending the programm", NoTreeError);
    return 0;
}
