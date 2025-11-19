#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "akinator_struct.h"
#include "akinator_dump.h"

FILE *AKINATOR_BASE_FILE = OpenOutputFile();
const char *DEFAULT_OUTPUT_TREE_FILE_NAME = "Akinator_output_file.txt";

FILE *INPUT_LOGFILE = OpenInputLogFile();  // TODO: Input !!!!!!!!! NOT IMPUT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const char *DEFAULT_INPUT_LOGFILE_NAME = "ImputLogfileAkinator.html";

//=============================================================================

FILE* OpenOutputFile() {

    FILE *file_ptr = fopen(DEFAULT_OUTPUT_TREE_FILE_NAME, "w");
    atexit(CloseOutputFile);

    return file_ptr;
}

void CloseOutputFile() {

    fclose(OUTPUT_FILE_FOR_TREE);

    printf("Output file closed succesfully\n");
}


//-----------------------------------------------------------------------------

FILE* OpenInputLogFile() {

    FILE *file_ptr = fopen(DEFAULT_INPUT_LOGFILE_NAME, "w");
    atexit(CloseInputLogFile);

    return file_ptr;
}

void CloseInputLogFile() {

    fclose(INPUT_LOGFILE);
    printf("Input logfile closed succesfully\n");
}

//=============================================================================

bool CheckIfAnswerCorrect() {

    char symbol = getchar();
    bool result = true;

    while (symbol != '\n' && symbol != EOF && symbol != '\0') {
        if (!isspace(symbol))
            result = false;
        symbol = getchar();
    }

    return result;
}


char *GetAnswerString() {

    int curr_size = 16, curr_pos = 0;
    char *answer = (char *) calloc(curr_size, sizeof(char));

    if (answer == NULL)
        return answer;

    int curr_char = 0;
    scanf(" %c", &curr_char);

    while (curr_char != '\0' && curr_char != '\n') {
        answer[curr_pos++] = curr_char;

        if (curr_pos >= curr_size) {
            curr_size += 16;
            answer = (char *) realloc(answer, sizeof(char) * curr_size);

            if (answer == NULL)
                return answer;
        }

        curr_char = getchar();
    }

    answer[curr_pos] = '\0';

    return answer;
}

//=============================================================================

void PrintTreeNode(TreeNode_t *node) {

    assert(node);

    fprintf(OUTPUT_FILE_FOR_TREE, " ( ");

    fprintf(OUTPUT_FILE_FOR_TREE, " \"" TREE_TYPE_OUTPUT "\" ", node->data);

    if (node->left != NULL)
        PrintTreeNode(node->left);
    else
        fprintf(OUTPUT_FILE_FOR_TREE, " nil ");

    if (node->right != NULL)
        PrintTreeNode(node->right);
    else
        fprintf(OUTPUT_FILE_FOR_TREE, " nil ");

    fprintf(OUTPUT_FILE_FOR_TREE, " ) ");
}

int PrintAkinatorBaseInOutputFile(Tree_t *tree) {

    Return_If_Tree_Error(tree)

    PrintTreeNode(tree->root);

    Return_Tree_Error(tree);
}

//=============================================================================

TreeNode_t *GetAkinatorBaseFromFile() {

    size_t buffer_size = 0;
    char *buffer = GetBuffer(INPUT_FILE_FOR_TREE, &buffer_size);

    if (buffer == NULL) {
        printf("Input file error\n");
        return NULL;
    }

ON_DEBUG(buffer_begin = buffer);

    return ReadCurrNode(&buffer);
}

void SkipSpaces(char **curr_pos) {

    assert(curr_pos);

    while (isspace(**curr_pos) && **curr_pos != '\0')
        (*curr_pos)++;

ON_DEBUG(Print_Curr_Input_Information(*curr_pos, "Dump after skipping spaces"));

}

TreeNode_t *ReadCurrNode(char **curr_symbol) {

    assert(curr_symbol);

    SkipSpaces(curr_symbol);

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump in the begin of recursion"));

    if (**curr_symbol == '(') {

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump in the begin of reading new node"));

        TreeNode_t *new_node = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));
        (*curr_symbol)++;

        size_t data_len = 0;
        sscanf(*curr_symbol, " \"%*[^\"]\"%n", &data_len);

        new_node->data = (char *) calloc(data_len, sizeof(char));
        sscanf(*curr_symbol, " \"%[^\"]", new_node->data);
        new_node->data_hash = CountStringHashDJB2(new_node->data);

        (*curr_symbol) += data_len;

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Tree dump after creating new node without sons"));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));

ON_DEBUG(fprintf(INPUT_LOGFILE, "data size is %d, data is %s[%p]\n", data_len, new_node->data, &new_node->data));

        new_node->left = ReadCurrNode(curr_symbol);

        new_node->right = ReadCurrNode(curr_symbol);

        SkipSpaces(curr_symbol);

        if (**curr_symbol != ')') {
            printf("Read from input file error: SYNTAX ERROR\n");
            return NULL;
        }
        (*curr_symbol)++;

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump after reading new node"));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));

        return new_node;
    }

    else {

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump before nil"));

        if (strncmp(*curr_symbol, "nil", strlen("nil")) == 0)
            *curr_symbol += strlen("nil");

        else
            printf("Read from input file error: SYNTAX ERROR\n");

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump after nil"));

        return NULL;
    }

    return NULL;
}
