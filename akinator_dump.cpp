#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "akinator_dump.h"
#include "akinator_struct.h"

//=============================================================================

#define SEREGA LOGFILE
FILE *LOGFILE = OpenLogFile();

const char DEFAULT_LOG_FILE_NAME[] = "AkinatorLogFile.html";

//-----------------------------------------------------------------------------

const char *ERROR_RED_COLOR     = "CC0000";
const char *LAVANDER_PINK_COLOR = "fff0f5";
const char *SOFT_PINK_COLOR     = "ff97bb";

const int MAX_DOT_NAME_SIZE      = 100;
const int MAX_DOT_FILE_NAME_SIZE = 100;
const int MAX_DOT_COMMAND_SIZE   = 100;

//-----------------------------------------------------------------------------

FILE* OpenLogFile() {

    FILE *file_ptr = fopen(DEFAULT_LOG_FILE_NAME, "w");
    atexit(CloseLogFile);

    return file_ptr;
}

void CloseLogFile() {

    printf("Programm ended\n");
    fclose(SEREGA);
}

//=============================================================================

int TreeVerify(Tree_t *tree) {

    int error = NoTreeError;

    if (tree == NULL)
        return TreePtrError;

    if (tree->root == NULL)
        error |= RootPtrError;

    if (tree->nodes_count <= 0)
        error |= SizeTreeError;

    return error;
}

//=============================================================================

void TreeDump(Tree_t *tree, const char *file_name, const char *function_name,
              int line_number, const char *calling_reason_string, int curr_error) {

    TreeDumpPrintErrors(tree, file_name, function_name, line_number, calling_reason_string, curr_error);

    TreeDumpCreateGraphFile(tree);
}

//-----------------------------------------------------------------------------

void TreeDumpPrintErrors(Tree_t *tree, const char *file_name, const char *function_name,
                         int line_number, const char *calling_reason_string, int curr_error) {

    int error = TreeVerify(tree) | curr_error;

    fprintf(SEREGA, "<h2><font color=\"#%s\"><p align=\"center\">===========DUMP==========="
                    "</p></font></h2>\n<h3><p align=\"center\">%s</p></h3>\n\n",
                    ERROR_RED_COLOR, calling_reason_string);

    fprintf(SEREGA, "<h4>TreeDump() from %s at %s:%d:</h4>\n<pre>", file_name, function_name,
                                                                    line_number);

    if (tree == NULL || error & TreePtrError) {
        fprintf(SEREGA, "\nERROR: tree pointer error. Tree pointer is [%p]\n\n", tree);
        return;
    }

    fprintf(SEREGA, "tree [%p]\n", tree);

    if (error & SizeTreeError)
        fprintf(SEREGA, "\nERROR: tree array size error. Size is %d\n\n", tree->nodes_count);
    else
        fprintf(SEREGA, "tree size is %d\n\n", tree->nodes_count);

    if (error & RootPtrError || tree->root == NULL)
        fprintf(SEREGA, "\nERROR: tree root error. Root pointer is [%p]\n\n", tree->root);
    else
        fprintf(SEREGA, "Root pointer is [%p]\n\n", tree->root);

    if (error & NodePtrError)
        fprintf(SEREGA, "ERROR: error with tree node\n\n");


    fprintf(SEREGA, "</pre>\n");
}

//-----------------------------------------------------------------------------

void TreeDumpCreateGraphFile(Tree_t *tree) {

    if (tree == NULL)
        return;

    char *DotFileName = CreateDotFileName("txt");
    char *DotFilePngName = CreateDotFileName("png");

    if (DotFileName == NULL || DotFilePngName == NULL) {
        fprintf(SEREGA, "ERROR: pointer dot file name error\n\n");
        return;
    }


    FILE *dot_file_ptr = fopen(DotFileName, "w");

    if (dot_file_ptr == NULL) {
        fprintf(SEREGA, "ERROR: dot file pointer error\n\n");
        return;
    }

    PrintDotFileHeader(dot_file_ptr, DotFilePngName);
    DrawDotNode(tree->root, dot_file_ptr, 1, 1);
    PrintDotFileEnd(dot_file_ptr);

    fclose(dot_file_ptr);

    CreateImageFromDotFile(DotFileName, DotFilePngName);

    free(DotFileName);
    free(DotFilePngName);
}

//-----------------------------------------------------------------------------


char *CreateDotFileName(const char *file_type) {

    static int dot_file_number = 0;

    char *file_name = (char *) calloc(MAX_DOT_FILE_NAME_SIZE, sizeof(char));

    snprintf(file_name, MAX_DOT_FILE_NAME_SIZE, "DotFileNum_%d.%s", dot_file_number, file_type);

    dot_file_number++;

    return file_name;
}

//-----------------------------------------------------------------------------

void PrintDotFileHeader(FILE *dot_file_ptr, char *DotFilePngName) {

    assert(dot_file_ptr);
    assert(DotFilePngName);

    fprintf(SEREGA, "<img src=\"%s\" />\n", DotFilePngName);

    fprintf(dot_file_ptr, "digraph {\nrankdir=\"TB\";\n");
    fprintf(dot_file_ptr, "node [shape = \"Mrecord\", style = \"filled\", fillcolor = \"#%s\""
                          ", color = \"#%s\", fontcolor = red, fontsize=20, margin=0.1];\n", LAVANDER_PINK_COLOR, SOFT_PINK_COLOR);
}

//-----------------------------------------------------------------------------

int DrawDotNode(const TreeNode_t *node, FILE *dot_file_ptr, int rank_num, int node_num) {

    assert(node);

    char *curr_node_name = GetNodeName(node_num);
    int curr_node_num = node_num;

    if (node->left != NULL && node->right != NULL)
        fprintf(dot_file_ptr, "%s [label = \"{ "TREE_TYPE_OUTPUT"? | { yes | no } }\", rank = %d]\n",
                                curr_node_name, node->data, rank_num);
    else
        fprintf(dot_file_ptr, "%s [label = \"{ "TREE_TYPE_OUTPUT" | { 0 | 0 } }\", rank = %d]\n",
                                curr_node_name, node->data, rank_num);
   ON_DEBUG(fprintf(stderr, "curr data %s\n", node->data));
    if (node->left != NULL) {
        node_num = DrawDotNode(node->left, dot_file_ptr, rank_num + 1, node_num + 1);
        char *left_node_name = GetNodeName(curr_node_num + 1);
        fprintf(dot_file_ptr,  "%s -> %s\n", curr_node_name, left_node_name);
        curr_node_num = node_num;
        free(left_node_name);
    }

    if (node->right != NULL) {
        node_num = DrawDotNode(node->right, dot_file_ptr, rank_num + 1, node_num + 1);
        char *right_node_name = GetNodeName(curr_node_num + 1);
        fprintf(dot_file_ptr,  "%s -> %s\n", curr_node_name, right_node_name);
        free(right_node_name);
    }

    free(curr_node_name);
    return node_num;
}

//-----------------------------------------------------------------------------

void PrintDotFileEnd(FILE *dot_file_ptr) {

    if (dot_file_ptr == NULL)
        return;

    fprintf(dot_file_ptr, "}");
}

//=============================================================================

void CreateImageFromDotFile(char *DotFileName, char *DotFilePngName) {

    if (DotFileName == NULL || DotFilePngName == NULL)
        return;

    char command[MAX_DOT_COMMAND_SIZE] = "";
    snprintf(command, MAX_DOT_COMMAND_SIZE, "dot %s -T png > %s", DotFileName, DotFilePngName);

    system(command);

    ON_DEBUG(fprintf(stderr, "%s\n", command));
}

//=============================================================================

char *GetNodeName(int node_num) {

    char *node = (char *) calloc(MAX_DOT_NAME_SIZE, sizeof(char));
    if (node == NULL)
        return node;

    snprintf(node, MAX_DOT_NAME_SIZE, "node_%d", node_num);

    return node;
}
