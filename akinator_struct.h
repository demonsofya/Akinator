#ifndef AKINATOR_STRUCT_H_INCLUDED
#define AKINATOR_STRUCT_H_INCLUDED

//=============================================================================

typedef char* TreeElem_t;
#define TREE_TYPE_OUTPUT "%s"

#define OUTPUT_FILE      stderr
//#define INPUT_FILE_FOR_TREE     "Akinator_output_file.txt"

//-----------------------------------------------------------------------------

//#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

//-----------------------------------------------------------------------------

#include "lib\stack\stack.h"
#include "lib\onegin\buffer.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
//-------FILES----------
#define OUTPUT_FILE_FOR_TREE    AKINATOR_BASE_FILE
#define INPUT_FILE_FOR_TREE     "akinator_base.txt"

extern FILE *AKINATOR_BASE_FILE;
extern const char *DEFAULT_OUTPUT_TREE_FILE_NAME;

extern FILE *INPUT_LOGFILE;
extern const char *DEFAULT_INPUT_LOGFILE_NAME;

//-----------------------------------------------------------------------------

struct TreeNode_t {
    TreeElem_t data;
    TreeNode_t *left;
    TreeNode_t *right;
    TreeNode_t *parent;
    int data_hash;
};

struct Tree_t {
    TreeNode_t *root;
    int nodes_count;
};

void CloseOutputFile();
FILE* OpenOutputFile();

FILE* OpenInputLogFile();
void CloseInputLogFile();

Tree_t *TreeCtor();
void DeleteNode(Tree_t *tree, TreeNode_t *node);
void TreeNodeDtor(Tree_t *tree, TreeNode_t *node);

char *GetAnswerString();
bool CheckIfAnswerCorrect();

int InsertNewVariant(Tree_t *tree, TreeNode_t *node);
int FindObject(Tree_t *tree);

int GiveDefinitionForObject(Tree_t *tree);
int PrintObjectCharacteristicsSinceNode(Tree_t *tree, TreeNode_t *curr_node, Stack_t *object_path);
TreeNode_t *FindObjectByName(Tree_t *tree, TreeNode_t *curr_node,
                             const char *object_name, Stack_t *object_path);

int CountStringHashDJB2(const char *curr_string);

int PrintAkinatorBaseInOutputFile(Tree_t *tree);
void PrintTreeNode(TreeNode_t *node);

int PrintObjectsDifferent(Tree_t *tree, Stack_t *first_object_path, Stack_t *second_object_path);
int CompareTwoObjects(Tree_t *tree);

TreeNode_t *ReadCurrNode(char **curr_symbol);
void SkipSpaces(char **curr_pos);
TreeNode_t *GetAkinatorBaseFromFile();

#endif // AKINATOR_STRUCT_H_INCLUDED
