#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "CSubsetLexer.h"
#include "CSubsetParser.h"
//#include "MyVisitor.h"
#include "Codegenvisitor.h"

using namespace antlr4;
using namespace std;

ofstream lexLogFile; // used by Lexer.g4's writeIntoLexLogFile()

// own Error Listener , so that error don't printed in the terminal 

// class FileErrorListener : public BaseErrorListener
// {
// private:
//     MyVisitor &visitor;

// public:
//     FileErrorListener(MyVisitor &v) : visitor(v) {}

//     virtual void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
//                              size_t charPositionInLine, const string &msg, std::exception_ptr e) override
//     {
//         visitor.logError((int)line, "syntax error: " + msg);
//     }
// };

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        cerr << "Error opening input file: " << argv[1] << endl;
        return 1;
    }

    // int totalLines = 0;
    // string line;

    // while (getline(inputFile, line))
    // {
    //     totalLines++;
    // }

    // inputFile.clear();
    // inputFile.seekg(0);

    // // Construct the visitor 1st (generates the log and error file)
    // MyVisitor visitor;
    // FileErrorListener fileErrorListener(visitor);

    ANTLRInputStream input(inputFile);
    CSubsetLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CSubsetParser parser(&tokens);


    // lexer.removeErrorListeners();
    // parser.removeErrorListeners();

    // lexer.addErrorListener(&fileErrorListener);
    // parser.addErrorListener(&fileErrorListener);

    CSubsetParser::StartContext *tree = parser.start();

    // visitor.visit(tree);

    // visitor.writeSummary(totalLines);

    // cout << "Parsing completed." << endl;


   // if (visitor.getTotalErrors() == 0)
{
    // NOTE: MyVisitor.h doesn't currently expose totalErrors publicly.
    // Add a one-line getter to MyVisitor.h (this does NOT change any
    // existing checking logic, just exposes the count):
    //
    //     int getTotalErrors() const { return totalErrors; }
    //

    //cout<<"I was here";
    CodeGenVisitor codegen;
    codegen.visit(tree);
    codegen.writeAsm("my_code.asm");
    codegen.writeOptimizedAsm("optimized_code.asm");
}

    

    inputFile.close();
    return 0;
}