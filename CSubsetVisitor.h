
// Generated from CSubset.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CSubsetParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by CSubsetParser.
 */
class  CSubsetVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by CSubsetParser.
   */
    virtual std::any visitStart(CSubsetParser::StartContext *context) = 0;

    virtual std::any visitProgramMulti(CSubsetParser::ProgramMultiContext *context) = 0;

    virtual std::any visitProgramSingle(CSubsetParser::ProgramSingleContext *context) = 0;

    virtual std::any visitUnitVarDecl(CSubsetParser::UnitVarDeclContext *context) = 0;

    virtual std::any visitUnitFuncDecl(CSubsetParser::UnitFuncDeclContext *context) = 0;

    virtual std::any visitUnitFuncDef(CSubsetParser::UnitFuncDefContext *context) = 0;

    virtual std::any visitFuncDeclParams(CSubsetParser::FuncDeclParamsContext *context) = 0;

    virtual std::any visitFuncDecLNoParams(CSubsetParser::FuncDecLNoParamsContext *context) = 0;

    virtual std::any visitFuncDefParams(CSubsetParser::FuncDefParamsContext *context) = 0;

    virtual std::any visitFuncDefNoParams(CSubsetParser::FuncDefNoParamsContext *context) = 0;

    virtual std::any visitParamListMulti(CSubsetParser::ParamListMultiContext *context) = 0;

    virtual std::any visitParamListMultiNoId(CSubsetParser::ParamListMultiNoIdContext *context) = 0;

    virtual std::any visitParamListError(CSubsetParser::ParamListErrorContext *context) = 0;

    virtual std::any visitParamListSingle(CSubsetParser::ParamListSingleContext *context) = 0;

    virtual std::any visitParamListSingleNoId(CSubsetParser::ParamListSingleNoIdContext *context) = 0;

    virtual std::any visitCompoundWithStatements(CSubsetParser::CompoundWithStatementsContext *context) = 0;

    virtual std::any visitCompoundEmpty(CSubsetParser::CompoundEmptyContext *context) = 0;

    virtual std::any visitVar_declaration(CSubsetParser::Var_declarationContext *context) = 0;

    virtual std::any visitIntType(CSubsetParser::IntTypeContext *context) = 0;

    virtual std::any visitFloatType(CSubsetParser::FloatTypeContext *context) = 0;

    virtual std::any visitVoidType(CSubsetParser::VoidTypeContext *context) = 0;

    virtual std::any visitDeclSingleArr(CSubsetParser::DeclSingleArrContext *context) = 0;

    virtual std::any visitDeclError(CSubsetParser::DeclErrorContext *context) = 0;

    virtual std::any visitDeclListId(CSubsetParser::DeclListIdContext *context) = 0;

    virtual std::any visitDeclListArr(CSubsetParser::DeclListArrContext *context) = 0;

    virtual std::any visitDeclSingleId(CSubsetParser::DeclSingleIdContext *context) = 0;

    virtual std::any visitStatementsSingle(CSubsetParser::StatementsSingleContext *context) = 0;

    virtual std::any visitStatementsMulti(CSubsetParser::StatementsMultiContext *context) = 0;

    virtual std::any visitStmtVarDecl(CSubsetParser::StmtVarDeclContext *context) = 0;

    virtual std::any visitStmtExprStmt(CSubsetParser::StmtExprStmtContext *context) = 0;

    virtual std::any visitStmtCompound(CSubsetParser::StmtCompoundContext *context) = 0;

    virtual std::any visitStmtFor(CSubsetParser::StmtForContext *context) = 0;

    virtual std::any visitStmtIf(CSubsetParser::StmtIfContext *context) = 0;

    virtual std::any visitStmtIfElse(CSubsetParser::StmtIfElseContext *context) = 0;

    virtual std::any visitStmtWhile(CSubsetParser::StmtWhileContext *context) = 0;

    virtual std::any visitStmtPrintln(CSubsetParser::StmtPrintlnContext *context) = 0;

    virtual std::any visitStmtReturn(CSubsetParser::StmtReturnContext *context) = 0;

    virtual std::any visitExprStmtEmpty(CSubsetParser::ExprStmtEmptyContext *context) = 0;

    virtual std::any visitExprStmtExpr(CSubsetParser::ExprStmtExprContext *context) = 0;

    virtual std::any visitExprError(CSubsetParser::ExprErrorContext *context) = 0;

    virtual std::any visitVarId(CSubsetParser::VarIdContext *context) = 0;

    virtual std::any visitVarArray(CSubsetParser::VarArrayContext *context) = 0;

    virtual std::any visitExprLogic(CSubsetParser::ExprLogicContext *context) = 0;

    virtual std::any visitExprAssign(CSubsetParser::ExprAssignContext *context) = 0;

    virtual std::any visitLogicRel(CSubsetParser::LogicRelContext *context) = 0;

    virtual std::any visitLogicOp(CSubsetParser::LogicOpContext *context) = 0;

    virtual std::any visitRelSimple(CSubsetParser::RelSimpleContext *context) = 0;

    virtual std::any visitRelOp(CSubsetParser::RelOpContext *context) = 0;

    virtual std::any visitSimpleTerm(CSubsetParser::SimpleTermContext *context) = 0;

    virtual std::any visitSimpleAddop(CSubsetParser::SimpleAddopContext *context) = 0;

    virtual std::any visitSimpleExpressionError(CSubsetParser::SimpleExpressionErrorContext *context) = 0;

    virtual std::any visitTermUnary(CSubsetParser::TermUnaryContext *context) = 0;

    virtual std::any visitTermMulop(CSubsetParser::TermMulopContext *context) = 0;

    virtual std::any visitUnaryAddop(CSubsetParser::UnaryAddopContext *context) = 0;

    virtual std::any visitUnaryNot(CSubsetParser::UnaryNotContext *context) = 0;

    virtual std::any visitUnaryFactor(CSubsetParser::UnaryFactorContext *context) = 0;

    virtual std::any visitFactorVar(CSubsetParser::FactorVarContext *context) = 0;

    virtual std::any visitFactorCall(CSubsetParser::FactorCallContext *context) = 0;

    virtual std::any visitFactorParen(CSubsetParser::FactorParenContext *context) = 0;

    virtual std::any visitFactorInt(CSubsetParser::FactorIntContext *context) = 0;

    virtual std::any visitFactorFloat(CSubsetParser::FactorFloatContext *context) = 0;

    virtual std::any visitFactorInc(CSubsetParser::FactorIncContext *context) = 0;

    virtual std::any visitFactorDec(CSubsetParser::FactorDecContext *context) = 0;

    virtual std::any visitArgListNonEmpty(CSubsetParser::ArgListNonEmptyContext *context) = 0;

    virtual std::any visitArgListEmpty(CSubsetParser::ArgListEmptyContext *context) = 0;

    virtual std::any visitArgsMulti(CSubsetParser::ArgsMultiContext *context) = 0;

    virtual std::any visitArgsSingle(CSubsetParser::ArgsSingleContext *context) = 0;


};

