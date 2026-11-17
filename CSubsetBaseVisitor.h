
// Generated from CSubset.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "CSubsetVisitor.h"


/**
 * This class provides an empty implementation of CSubsetVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  CSubsetBaseVisitor : public CSubsetVisitor {
public:

  virtual std::any visitStart(CSubsetParser::StartContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProgramMulti(CSubsetParser::ProgramMultiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProgramSingle(CSubsetParser::ProgramSingleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnitVarDecl(CSubsetParser::UnitVarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnitFuncDecl(CSubsetParser::UnitFuncDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnitFuncDef(CSubsetParser::UnitFuncDefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDeclParams(CSubsetParser::FuncDeclParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDecLNoParams(CSubsetParser::FuncDecLNoParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDefParams(CSubsetParser::FuncDefParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncDefNoParams(CSubsetParser::FuncDefNoParamsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamListMulti(CSubsetParser::ParamListMultiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamListMultiNoId(CSubsetParser::ParamListMultiNoIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamListError(CSubsetParser::ParamListErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamListSingle(CSubsetParser::ParamListSingleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParamListSingleNoId(CSubsetParser::ParamListSingleNoIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompoundWithStatements(CSubsetParser::CompoundWithStatementsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompoundEmpty(CSubsetParser::CompoundEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVar_declaration(CSubsetParser::Var_declarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIntType(CSubsetParser::IntTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFloatType(CSubsetParser::FloatTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVoidType(CSubsetParser::VoidTypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclSingleArr(CSubsetParser::DeclSingleArrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclError(CSubsetParser::DeclErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclListId(CSubsetParser::DeclListIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclListArr(CSubsetParser::DeclListArrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclSingleId(CSubsetParser::DeclSingleIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatementsSingle(CSubsetParser::StatementsSingleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatementsMulti(CSubsetParser::StatementsMultiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtVarDecl(CSubsetParser::StmtVarDeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtExprStmt(CSubsetParser::StmtExprStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtCompound(CSubsetParser::StmtCompoundContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtFor(CSubsetParser::StmtForContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtIf(CSubsetParser::StmtIfContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtIfElse(CSubsetParser::StmtIfElseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtWhile(CSubsetParser::StmtWhileContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtPrintln(CSubsetParser::StmtPrintlnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmtReturn(CSubsetParser::StmtReturnContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStmtEmpty(CSubsetParser::ExprStmtEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprStmtExpr(CSubsetParser::ExprStmtExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprError(CSubsetParser::ExprErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarId(CSubsetParser::VarIdContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitVarArray(CSubsetParser::VarArrayContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprLogic(CSubsetParser::ExprLogicContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExprAssign(CSubsetParser::ExprAssignContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicRel(CSubsetParser::LogicRelContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicOp(CSubsetParser::LogicOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelSimple(CSubsetParser::RelSimpleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelOp(CSubsetParser::RelOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleTerm(CSubsetParser::SimpleTermContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleAddop(CSubsetParser::SimpleAddopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSimpleExpressionError(CSubsetParser::SimpleExpressionErrorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTermUnary(CSubsetParser::TermUnaryContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTermMulop(CSubsetParser::TermMulopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryAddop(CSubsetParser::UnaryAddopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryNot(CSubsetParser::UnaryNotContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryFactor(CSubsetParser::UnaryFactorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorVar(CSubsetParser::FactorVarContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorCall(CSubsetParser::FactorCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorParen(CSubsetParser::FactorParenContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorInt(CSubsetParser::FactorIntContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorFloat(CSubsetParser::FactorFloatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorInc(CSubsetParser::FactorIncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFactorDec(CSubsetParser::FactorDecContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgListNonEmpty(CSubsetParser::ArgListNonEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgListEmpty(CSubsetParser::ArgListEmptyContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgsMulti(CSubsetParser::ArgsMultiContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgsSingle(CSubsetParser::ArgsSingleContext *ctx) override {
    return visitChildren(ctx);
  }


};

