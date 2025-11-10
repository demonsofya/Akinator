#ifndef AKINATOR_STRUCT_H_INCLUDED
#define AKINATOR_STRUCT_H_INCLUDED

//=============================================================================

typedef char* TreeElem_t;
#define TREE_TYPE_OUTPUT "%s"

#define OUTPUT_FILE stderr

//-----------------------------------------------------------------------------

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

//-----------------------------------------------------------------------------

#include "lib/stack/stack.h"


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

Tree_t *TreeCtor();
void DeleteNode(Tree_t *tree, TreeNode_t *node);
void TreeNodeDtor(Tree_t *tree, TreeNode_t *node);

char *GetAnswerString();
int InsertNewVariant(Tree_t *tree, TreeNode_t *node);
int FindObject(Tree_t *tree);

int GiveDefinitionForObject(Tree_t *tree);
int PrintObjectCharacteristics(Tree_t *tree, Stack_t *object_path);
TreeNode_t *FindObjectByName(Tree_t *tree, TreeNode_t *curr_node,
                             const char *object_name, Stack_t *object_path);

int CountStringHashDJB2(const char *curr_string);


#endif // AKINATOR_STRUCT_H_INCLUDED
