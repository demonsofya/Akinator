#ifndef AKINATOR_DUMP_H_INCLUDED
#define AKINATOR_DUMP_H_INCLUDED


#include "akinator_struct.h"
#include "lib/stack/stack.h"

//=============================================================================


#define Return_If_Tree_Error(tree)                                              \
    {                                                                           \
        int error = TreeVerify(tree);                                           \
        if (error != 0) {                                                       \
            TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "", NoTreeError);  \
            return error;                                                       \
        }                                                                       \
    }

#define Return_Tree_Error(tree)                                                 \
    {                                                                           \
        int error = TreeVerify(tree);                                           \
        if (error != 0)                                                         \
            TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "", NoTreeError);  \
        return error;                                                           \
    }

//-----------------------------------------------------------------------------


enum TreeErrors {
    NoTreeError         = 0,
    TreePtrError        = 1 << 0,
    RootPtrError        = 1 << 1,
    NodePtrError        = 1 << 2,
    SizeTreeError       = 1 << 3,
    AnswerAkinatorError = 1 << 4,
    InsertAkinatorError = 1 << 5,
    ObjectAkinatorError = 1 << 6
};

FILE* OpenLogFile();
void CloseLogFile();

int TreeVerify(Tree_t *tree);
void TreeDump(Tree_t *tree, const char *file_name, const char *function_name,
              int line_number, const char *calling_reason_string, int curr_error);

void TreeDumpPrintErrors(Tree_t *tree, const char *file_name, const char *function_name,
              int line_number, const char *calling_reason_string, int curr_error);

void TreeDumpCreateGraphFile(Tree_t *tree);
char *CreateDotFileName(const char *file_type);

void PrintDotFileHeader(FILE *dot_file_ptr, char *DotFilePngName);
int DrawDotNode(const TreeNode_t *node, FILE *dot_file_ptr, int rank_num, int node_num);
void PrintDotFileEnd(FILE *dot_file_ptr);

void CreateImageFromDotFile(char *DotFileName, char *DotFilePngName);

char *GetNodeName(int node_num);

#endif // AKINATOR_DUMP_H_INCLUDED
