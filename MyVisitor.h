#ifndef MY_VISITOR_H
#define MY_VISITOR_H

#include "CSubsetBaseVisitor.h"
#include "CSubsetParser.h"
#include "symbol_table.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

class MyVisitor : public CSubsetBaseVisitor
{
private:
    ofstream logFile;
    ofstream errorFile;
    SymbolTable *symTable;
    string currentDeclType;
    vector<string> currentParamTypes; 
    bool exprHasVoidCall = false;
    int totalErrors = 0;
    int totalLines = 0;

    vector<string> collectParamTypes(CSubsetParser::Parameter_listContext *ctx);
    string inferExprType(const string &text);
    vector<string> splitTopLevel(const string &s);

   
    void logRule(int line, const string &label, const string &text);
    string safeVisitString(antlr4::tree::ParseTree *node);
    // CONST_FLOAT to 2 decimal points ( for input3.txt )
    static string formatFloat(const string &raw);

public:
    MyVisitor();
    ~MyVisitor();

    any visitStart(CSubsetParser::StartContext *ctx) override;

    void logError(int line, const string &message);

    any visitProgramMulti(CSubsetParser::ProgramMultiContext *ctx) override;
    any visitProgramSingle(CSubsetParser::ProgramSingleContext *ctx) override;

    any visitUnitVarDecl(CSubsetParser::UnitVarDeclContext *ctx) override;
    any visitUnitFuncDecl(CSubsetParser::UnitFuncDeclContext *ctx) override;
    any visitUnitFuncDef(CSubsetParser::UnitFuncDefContext *ctx) override;

    any visitFuncDeclParams(CSubsetParser::FuncDeclParamsContext *ctx) override;
    any visitFuncDecLNoParams(CSubsetParser::FuncDecLNoParamsContext *ctx) override;

    any visitFuncDefParams(CSubsetParser::FuncDefParamsContext *ctx) override;
    any visitFuncDefNoParams(CSubsetParser::FuncDefNoParamsContext *ctx) override;

    any visitParamListMulti(CSubsetParser::ParamListMultiContext *ctx) override;
    any visitParamListMultiNoId(CSubsetParser::ParamListMultiNoIdContext *ctx) override;
    any visitParamListSingle(CSubsetParser::ParamListSingleContext *ctx) override;
    any visitParamListSingleNoId(CSubsetParser::ParamListSingleNoIdContext *ctx) override;

    any visitCompoundWithStatements(CSubsetParser::CompoundWithStatementsContext *ctx) override;
    any visitCompoundEmpty(CSubsetParser::CompoundEmptyContext *ctx) override;

    any visitVar_declaration(CSubsetParser::Var_declarationContext *ctx) override;

    any visitIntType(CSubsetParser::IntTypeContext *ctx) override;
    any visitFloatType(CSubsetParser::FloatTypeContext *ctx) override;
    any visitVoidType(CSubsetParser::VoidTypeContext *ctx) override;

    any visitDeclListId(CSubsetParser::DeclListIdContext *ctx) override;
    any visitDeclListArr(CSubsetParser::DeclListArrContext *ctx) override;
    any visitDeclSingleId(CSubsetParser::DeclSingleIdContext *ctx) override;
    any visitDeclSingleArr(CSubsetParser::DeclSingleArrContext *ctx) override;

    any visitStatementsSingle(CSubsetParser::StatementsSingleContext *ctx) override;
    any visitStatementsMulti(CSubsetParser::StatementsMultiContext *ctx) override;

    any visitStmtVarDecl(CSubsetParser::StmtVarDeclContext *ctx) override;
    any visitStmtExprStmt(CSubsetParser::StmtExprStmtContext *ctx) override;
    any visitStmtCompound(CSubsetParser::StmtCompoundContext *ctx) override;
    any visitStmtFor(CSubsetParser::StmtForContext *ctx) override;
    any visitStmtIf(CSubsetParser::StmtIfContext *ctx) override;
    any visitStmtIfElse(CSubsetParser::StmtIfElseContext *ctx) override;
    any visitStmtWhile(CSubsetParser::StmtWhileContext *ctx) override;
    any visitStmtPrintln(CSubsetParser::StmtPrintlnContext *ctx) override;
    any visitStmtReturn(CSubsetParser::StmtReturnContext *ctx) override;

    any visitExprStmtEmpty(CSubsetParser::ExprStmtEmptyContext *ctx) override;
    any visitExprStmtExpr(CSubsetParser::ExprStmtExprContext *ctx) override;

    any visitVarId(CSubsetParser::VarIdContext *ctx) override;
    any visitVarArray(CSubsetParser::VarArrayContext *ctx) override;

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

    any visitParamListError(CSubsetParser::ParamListErrorContext *ctx) override;
    any visitDeclError(CSubsetParser::DeclErrorContext *ctx) override;
    any visitSimpleExpressionError(CSubsetParser::SimpleExpressionErrorContext *ctx) override;
    any visitExprError(CSubsetParser::ExprErrorContext *ctx) override;
    void writeSummary(int totalLines)
    {
        if (logFile.is_open())
        {
            logFile << "Total lines: " << totalLines << "\n";
            logFile << "Total errors: " << totalErrors << "\n";
        }
    }

    int getTotalErrors(){
        return totalErrors;
    }
};

#endif