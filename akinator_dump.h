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

#define Continue_If_Answer_Error()                                              \
{                                                                               \
    if (!CheckIfAnswerCorrect()) {                                              \
            fprintf(OUTPUT_FILE, "Wrong answer format. Try again\n\n");         \
            continue;                                                           \
    }                                                                           \
}
//-----------------------------------------------------------------------------
//---------DEBUG----------
static char *buffer_begin = NULL;

#define Print_Curr_Input_Information(curr_symbol, dump_information_string)                      \
    {                                                                                           \
        fprintf(INPUT_LOGFILE, "<h2><font color=\"#CC0000\"><p align=\"center\">Input DUMP"     \
                    "</p></font></h2>\n<h3><p align=\"center\">%s</p></h3>\n\n",                \
                    dump_information_string);                                                   \
        fprintf(INPUT_LOGFILE, "<h4>Input dump from %s at %s:%d:</h4><pre>\n\n",                \
        __FILE__, __FUNCTION__, __LINE__);                                                      \
        fprintf(INPUT_LOGFILE, "Buffer from current position is \n%s\n\n", curr_symbol);        \
    }

//-----------------------------------------------------------------------------
//-----CONSTANTS--------
extern const char *ERROR_RED_COLOR;
extern const char *LAVANDER_PINK_COLOR;
extern const char *SOFT_PINK_COLOR;

extern const int MAX_DOT_NAME_SIZE;
extern const int MAX_DOT_FILE_NAME_SIZE;
extern const int MAX_DOT_COMMAND_SIZE;
//-----------------------------------------------------------------------------
//-----LOGFILE----------

FILE* OpenLogFile();
void CloseLogFile();

#define SEREGA LOGFILE
extern FILE *LOGFILE;

extern const char *DEFAULT_LOG_FILE_NAME;
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
