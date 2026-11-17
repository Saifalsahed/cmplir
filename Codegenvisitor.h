#ifndef CODE_GEN_VISITOR_H
#define CODE_GEN_VISITOR_H

#include "CSubsetBaseVisitor.h"
#include "Optimizer.h"
#include "CSubsetParser.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;


class CodeGenVisitor : public CSubsetBaseVisitor
{
private:
    ostringstream code;   
    ostringstream dataSeg; 

    
    struct LocalVarSlot
    {
        int disp; 
        int size;
    };

    
    struct VarLoc
    {
        bool isGlobal;
        bool isArray;
        int disp;
        int size;
        string name;
    };
    vector<unordered_map<string, LocalVarSlot>> scopeStack; 
    unordered_map<string, int> globalSize;

    bool inFunction = false;
    int nextParamOffset = 8;   // [ebp+8], [ebp+12], ... for parameters of current function
    int nextLocalOffset = 0;   // running total of bytes used below ebp in current function
    int labelCounter = 0;      // unique suffix generator (kept for future phase-2 branches)
    string currentReturnLabel; // where `return expr;` jumps to
    int currentCallArgCount=0;
    
    void emit(const string &line);     // one instruction into .text, indented
    void emitData(const string &line); // one line into .data
    void emitComment(int line);        // "; Line N" marker, handy for debugging asm
    string ebpAddr(int disp) const;    // "[ebp+8]" or "[ebp-12]"
    string newLabel();
    void emitLabel(const string &name);

    
    void declareVariable(const string &name, int sizeWords = 1);
    VarLoc resolve(const string &name);
    string locationOf(const VarLoc &loc);                                                
    void genArrayAddressToEdi(const VarLoc &loc, CSubsetParser::ExpressionContext *idx); 
    void genVariableAddressToEdi(CSubsetParser::VariableContext *varCtx);                

    
    int sizeOfDeclarationList(CSubsetParser::Declaration_listContext *ctx);
    int sizeOfStatement(CSubsetParser::StatementContext *ctx);
    int sizeOfStatements(CSubsetParser::StatementsContext *ctx);

    void beginFunction(const string &name, int frameSize);
    void endFunction();

    // string newLabel();
    // void emitLabel(const string &name);
    void genJumpRel(CSubsetParser::Rel_expressionContext *ctx,
                    const string &trueLbl, const string &falseLbl);
    void genJumpLogicOp(CSubsetParser::LogicOpContext *ctx,
                        const string &trueLbl, const string &falseLbl);

    // static vector<string> splitLines(const string &text);              // add
    // vector<string> peepholeOptimize(const vector<string> &input);      // add
    // vector<string> mergeConsecutiveLabels(const vector<string> &input);// add

public:
    CodeGenVisitor();
    void writeAsm(const string &path);

    void writeOptimizedAsm(const string &path);

    any visitStart(CSubsetParser::StartContext *ctx) override;

    any visitProgramSingle(
        CSubsetParser::ProgramSingleContext *ctx) override;

    any visitProgramMulti(
        CSubsetParser::ProgramMultiContext *ctx) override;

    // units — thin pass-throughs, added for parity with MyVisitor's exhaustive
    // one-override-per-label style (default visitChildren would do the same
    // dispatch, but this makes the mapping to your .g4 labels explicit)
    any visitUnitVarDecl(CSubsetParser::UnitVarDeclContext *ctx) override;
    any visitUnitFuncDecl(CSubsetParser::UnitFuncDeclContext *ctx) override;
    any visitUnitFuncDef(CSubsetParser::UnitFuncDefContext *ctx) override;

    any visitFuncDeclParams(CSubsetParser::FuncDeclParamsContext *ctx) override;
    any visitFuncDecLNoParams(CSubsetParser::FuncDecLNoParamsContext *ctx) override;
    any visitFuncDefParams(CSubsetParser::FuncDefParamsContext *ctx) override;
    any visitFuncDefNoParams(CSubsetParser::FuncDefNoParamsContext *ctx) override;
    // declarations (shared path for globals and locals)

    // parameter list — binds names to [ebp+8], [ebp+12], ...
    any visitParamListSingle(CSubsetParser::ParamListSingleContext *ctx) override;
    any visitParamListMulti(CSubsetParser::ParamListMultiContext *ctx) override;
    any visitParamListSingleNoId(CSubsetParser::ParamListSingleNoIdContext *ctx) override;
    any visitParamListMultiNoId(CSubsetParser::ParamListMultiNoIdContext *ctx) override;

    // declarations (shared path for globals and locals)
    any visitVar_declaration(CSubsetParser::Var_declarationContext *ctx) override;
    any visitDeclListId(CSubsetParser::DeclListIdContext *ctx) override;
    any visitDeclSingleId(CSubsetParser::DeclSingleIdContext *ctx) override;
    any visitDeclListArr(CSubsetParser::DeclListArrContext *ctx) override;
    any visitDeclSingleArr(CSubsetParser::DeclSingleArrContext *ctx) override;

    // functions — only the no-params alt is in Phase 1

    // compound / statements
    any visitCompoundWithStatements(CSubsetParser::CompoundWithStatementsContext *ctx) override;
    any visitCompoundEmpty(CSubsetParser::CompoundEmptyContext *ctx) override;
    any visitStatementsSingle(CSubsetParser::StatementsSingleContext *ctx) override;
    any visitStatementsMulti(CSubsetParser::StatementsMultiContext *ctx) override;

    any visitStmtVarDecl(CSubsetParser::StmtVarDeclContext *ctx) override;
    any visitStmtExprStmt(CSubsetParser::StmtExprStmtContext *ctx) override;
    any visitStmtCompound(CSubsetParser::StmtCompoundContext *ctx) override;
    any visitStmtIf(CSubsetParser::StmtIfContext *ctx) override;
    any visitStmtIfElse(CSubsetParser::StmtIfElseContext *ctx) override;
    any visitStmtWhile(CSubsetParser::StmtWhileContext *ctx) override;
    any visitStmtFor(CSubsetParser::StmtForContext *ctx) override;
    any visitStmtPrintln(CSubsetParser::StmtPrintlnContext *ctx) override;
    any visitStmtReturn(CSubsetParser::StmtReturnContext *ctx) override;

    any visitExprStmtEmpty(CSubsetParser::ExprStmtEmptyContext *ctx) override;
    any visitExprStmtExpr(CSubsetParser::ExprStmtExprContext *ctx) override;

    // variables — scalar only; VarArray is out of Phase 1 scope
    any visitVarId(CSubsetParser::VarIdContext *ctx) override;
    any visitVarArray(CSubsetParser::VarArrayContext *ctx) override;

    // expressions
    any visitExprLogic(CSubsetParser::ExprLogicContext *ctx) override;
    any visitExprAssign(CSubsetParser::ExprAssignContext *ctx) override;

    any visitLogicRel(CSubsetParser::LogicRelContext *ctx) override;
    any visitLogicOp(CSubsetParser::LogicOpContext *ctx) override;

    any visitRelSimple(CSubsetParser::RelSimpleContext *ctx) override;
    any visitRelOp(CSubsetParser::RelOpContext *ctx) override;

    any visitSimpleTerm(CSubsetParser::SimpleTermContext *ctx) override;
    any visitSimpleAddop(CSubsetParser::SimpleAddopContext *ctx) override;

    any visitTermUnary(CSubsetParser::TermUnaryContext *ctx) override;
    any visitTermMulop(CSubsetParser::TermMulopContext *ctx) override;

    any visitUnaryAddop(CSubsetParser::UnaryAddopContext *ctx) override;
    any visitUnaryNot(CSubsetParser::UnaryNotContext *ctx) override;
    any visitUnaryFactor(CSubsetParser::UnaryFactorContext *ctx) override;

    any visitFactorVar(CSubsetParser::FactorVarContext *ctx) override;
    any visitFactorCall(CSubsetParser::FactorCallContext *ctx) override;
    any visitFactorParen(CSubsetParser::FactorParenContext *ctx) override;
    any visitFactorInt(CSubsetParser::FactorIntContext *ctx) override;
    any visitFactorFloat(CSubsetParser::FactorFloatContext *ctx) override;
    any visitFactorInc(CSubsetParser::FactorIncContext *ctx) override;
    any visitFactorDec(CSubsetParser::FactorDecContext *ctx) override;

    any visitArgListNonEmpty(CSubsetParser::ArgListNonEmptyContext *ctx) override;
    any visitArgListEmpty(CSubsetParser::ArgListEmptyContext *ctx) override;
    any visitArgsMulti(CSubsetParser::ArgsMultiContext *ctx) override;
    any visitArgsSingle(CSubsetParser::ArgsSingleContext *ctx) override;
};

#endif