#include "Codegenvisitor.h"


CodeGenVisitor::CodeGenVisitor()
{
    dataSeg << "segment readable writeable\n";

    code << "segment readable executable\n"
         << "entry start\n\n"
         << "include 'printProc.lib'\n\n";   

    code << "start:\n"
         << "    call main\n"
         << "    mov ebx, eax\n"
         << "    mov eax, 1\n"
         << "    int 0x80\n\n";
}

void CodeGenVisitor::emit(const string &line) { code << "    " << line << "\n"; }
void CodeGenVisitor::emitData(const string &line) { dataSeg << line << "\n"; }
void CodeGenVisitor::emitComment(int line) { code << "    ; Line " << line << "\n"; }
string CodeGenVisitor::newLabel() {return "Lec"+to_string(labelCounter++);}
void CodeGenVisitor::emitLabel(const string &name) {code<<name<<":\n";  }

string CodeGenVisitor::ebpAddr(int disp) const
{
    if (disp < 0) return "[ebp" + to_string(disp) + "]"; // 
    return "[ebp+" + to_string(disp) + "]";
}

void CodeGenVisitor::writeAsm(const string &path)
{
    ofstream out(path);
    out << "format ELF executable 3\n\n";
    out << dataSeg.str();
    //out << "number_buffer rb 32\n\n";
    out << code.str();
}

void CodeGenVisitor::writeOptimizedAsm(const string &path)
{
    string optimized = Optimizer::optimize(code.str());
    ofstream out(path);
     out << "format ELF executable 3\n\n";
     out << dataSeg.str();
    out << optimized;
}


any CodeGenVisitor::visitStart(CSubsetParser::StartContext *ctx)
{
    //cout << "visitStart\n";

    visit(ctx->program());

    return any();
}

any CodeGenVisitor::visitProgramSingle(
    CSubsetParser::ProgramSingleContext *ctx)
{
    //cout << "visitProgramSingle\n";

    visit(ctx->unit());
    return any();
}


any CodeGenVisitor::visitProgramMulti(
    CSubsetParser::ProgramMultiContext *ctx)
{
    //cout << "visitProgramMulti\n";

    visit(ctx->program());
    visit(ctx->unit());

    return any();
}

// Keep simple symbol info for generating variable addresses.
// Since Phase 1 has no arrays, each variable uses one 4-byte slot.

void CodeGenVisitor::declareVariable(const string &name, int sizeWords)
{
    if (!inFunction)
    {
        emitData(name + " dd " + to_string(sizeWords) + " DUP(0)");
        globalSize[name] = sizeWords;
    }
    else
    {
        nextLocalOffset += sizeWords * 4;
        int disp = -nextLocalOffset;
        scopeStack.back()[name] = LocalVarSlot{disp, sizeWords};
    }
}


CodeGenVisitor::VarLoc CodeGenVisitor::resolve(const string &name)
{
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
    {
        auto found = it->find(name);
        if (found != it->end())
        {
            return VarLoc{
                false,
                found->second.size > 1,
                found->second.disp,
                found->second.size,
                name
            };
        }
    }

    int sz = globalSize.count(name) ? globalSize[name] : 1;
    return VarLoc{true, sz > 1, 0, sz, name};
}

string CodeGenVisitor::locationOf(const VarLoc &loc)
{
    if (loc.isGlobal)
        return "[" + loc.name + "]";
    return ebpAddr(loc.disp);
}
// Leaves the effective address of arr[idx] in EDI. This was called from
// visitVarArray but never defined -- added here.
void CodeGenVisitor::genArrayAddressToEdi(const VarLoc &loc, CSubsetParser::ExpressionContext *idx)
{
    visit(idx); // EAX = index value
    emit("imul eax, 4");
    if (loc.isGlobal)
        emit("lea edi, [" + loc.name + "]");
    else
        emit("lea edi, " + ebpAddr(loc.disp));
    emit("add edi, eax");
}


// Gets the address of a scalar variable in EDI so ++/-- can modify it directly.

void CodeGenVisitor::genVariableAddressToEdi(CSubsetParser::VariableContext *varCtx)
{
    if (auto v = dynamic_cast<CSubsetParser::VarArrayContext *>(varCtx))
    {
        VarLoc loc = resolve(v->ID()->getText());
        genArrayAddressToEdi(loc, v->expression());
    }
    else if (auto v = dynamic_cast<CSubsetParser::VarIdContext *>(varCtx))
    {
        VarLoc loc = resolve(v->ID()->getText());
        if (loc.isGlobal)
            emit("lea edi, [" + loc.name + "]");
        else
            emit("lea edi, " + ebpAddr(loc.disp));
    }
}
 
// Calculates the total stack space needed for local variables before generating the function.
int CodeGenVisitor::sizeOfDeclarationList(CSubsetParser::Declaration_listContext *ctx)
{
    if (!ctx) return 0;
    if (dynamic_cast<CSubsetParser::DeclSingleIdContext *>(ctx)) return 4;
    if (auto c = dynamic_cast<CSubsetParser::DeclSingleArrContext *>(ctx))
        return stoi(c->CONST_INT()->getText()) * 4;
    if (auto c = dynamic_cast<CSubsetParser::DeclListIdContext *>(ctx))
        return sizeOfDeclarationList(c->declaration_list()) + 4;
    if (auto c = dynamic_cast<CSubsetParser::DeclListArrContext *>(ctx))
        return sizeOfDeclarationList(c->declaration_list()) + stoi(c->CONST_INT()->getText()) * 4;
    return 0; // DeclError -- unreachable in an error-free program
}
 
int CodeGenVisitor::sizeOfStatement(CSubsetParser::StatementContext *ctx)
{
    if (!ctx) return 0;
    if (auto c = dynamic_cast<CSubsetParser::StmtVarDeclContext *>(ctx))
        return sizeOfDeclarationList(c->var_declaration()->declaration_list());
    if (auto c = dynamic_cast<CSubsetParser::StmtCompoundContext *>(ctx))
    {
        if (auto body = dynamic_cast<CSubsetParser::CompoundWithStatementsContext *>(c->compound_statement()))
            return sizeOfStatements(body->statements());
        return 0;
    }
    if (auto c = dynamic_cast<CSubsetParser::StmtIfContext *>(ctx))
        return sizeOfStatement(c->statement());
    if (auto c = dynamic_cast<CSubsetParser::StmtIfElseContext *>(ctx))
        return sizeOfStatement(c->statement(0)) + sizeOfStatement(c->statement(1));
    if (auto c = dynamic_cast<CSubsetParser::StmtWhileContext *>(ctx))
        return sizeOfStatement(c->statement());
    if (auto c = dynamic_cast<CSubsetParser::StmtForContext *>(ctx))
        return sizeOfStatement(c->statement());
    return 0;
}
 

int CodeGenVisitor::sizeOfStatements(CSubsetParser::StatementsContext *ctx)
{
    if (!ctx) return 0;
    if (auto c = dynamic_cast<CSubsetParser::StatementsSingleContext *>(ctx))
        return sizeOfStatement(c->statement());
    if (auto c = dynamic_cast<CSubsetParser::StatementsMultiContext *>(ctx))
        return sizeOfStatements(c->statements()) + sizeOfStatement(c->statement());
    return 0;
}



void CodeGenVisitor::beginFunction(const string &name, int frameSize)
{
    inFunction = true;
    nextLocalOffset = 0;
    scopeStack.clear();
    scopeStack.push_back({});
    currentReturnLabel = "L_ret_" + name;

    code << "\n" << name << ":\n";
    emit("push ebp");
    emit("mov ebp, esp");
    if (frameSize > 0) emit("sub esp, " + to_string(frameSize));
}

void CodeGenVisitor::endFunction()
{
    code << currentReturnLabel << ":\n";
    emit("mov esp, ebp");
    emit("pop ebp");
    emit("ret");
    scopeStack.pop_back();
    inFunction = false;
}



// Units — pass-throughs to var_declaration / func_definition


any CodeGenVisitor::visitUnitVarDecl(CSubsetParser::UnitVarDeclContext *ctx)
{
    visit(ctx->var_declaration());
    return any();
}

any CodeGenVisitor::visitUnitFuncDecl(CSubsetParser::UnitFuncDeclContext *ctx)
{
    visit(ctx->func_declaration());
    return any();
}

any CodeGenVisitor::visitUnitFuncDef(CSubsetParser::UnitFuncDefContext *ctx)
{
    visit(ctx->func_definition());
    return any();
}

any CodeGenVisitor::visitFuncDeclParams(CSubsetParser::FuncDeclParamsContext *ctx)
{
    return any();
}

any CodeGenVisitor::visitFuncDecLNoParams(CSubsetParser::FuncDecLNoParamsContext *ctx)
{
    return any();
}

//function def with params


any CodeGenVisitor::visitFuncDefParams(CSubsetParser::FuncDefParamsContext *ctx)
{
    string funcName = ctx->ID()->getText();
    int frameSize = 0;
    if (auto body = dynamic_cast<CSubsetParser::CompoundWithStatementsContext *>(ctx->compound_statement()))
        frameSize = sizeOfStatements(body->statements());

    beginFunction(funcName, frameSize);

    nextParamOffset = 8;            // reset before this function's parameters
    visit(ctx->parameter_list());   // dispatches into the overrides below

    visit(ctx->compound_statement());
    endFunction();
    return any();
}


any CodeGenVisitor::visitFuncDefNoParams(CSubsetParser::FuncDefNoParamsContext *ctx)
{
    string funcName = ctx->ID()->getText();
    int frameSize = 0;
    if (auto body = dynamic_cast<CSubsetParser::CompoundWithStatementsContext *>(ctx->compound_statement()))
        frameSize = sizeOfStatements(body->statements());

    beginFunction(funcName, frameSize);
    visit(ctx->compound_statement());
    endFunction();
    return any();
}



//parameter list handle 


any CodeGenVisitor::visitParamListSingle(CSubsetParser::ParamListSingleContext *ctx)
{
    if (ctx->ID())
    {
        scopeStack.back()[ctx->ID()->getText()] = LocalVarSlot{nextParamOffset,1};
        nextParamOffset += 4;
    }
    return any();
}

any CodeGenVisitor::visitParamListMulti(CSubsetParser::ParamListMultiContext *ctx)
{
    visit(ctx->parameter_list());
    if (ctx->ID())
    {
        scopeStack.back()[ctx->ID()->getText()] = LocalVarSlot{nextParamOffset,1};
        nextParamOffset += 4;
    }
    return any();
}

any CodeGenVisitor::visitParamListSingleNoId(CSubsetParser::ParamListSingleNoIdContext *ctx)
{
    return any();
}

any CodeGenVisitor::visitParamListMultiNoId(CSubsetParser::ParamListMultiNoIdContext *ctx)
{
    visit(ctx->parameter_list());
    return any();
}
// Handles both global and local scalar declarations.


any CodeGenVisitor::visitVar_declaration(CSubsetParser::Var_declarationContext *ctx)
{
    visit(ctx->declaration_list());
    return any();
}

any CodeGenVisitor::visitDeclSingleId(CSubsetParser::DeclSingleIdContext *ctx)
{
    declareVariable(ctx->ID()->getText());
    return any();
}

any CodeGenVisitor::visitDeclListId(CSubsetParser::DeclListIdContext *ctx)
{
    visit(ctx->declaration_list());
    declareVariable(ctx->ID()->getText());
    return any();
}

any CodeGenVisitor::visitDeclSingleArr(CSubsetParser::DeclSingleArrContext *ctx)
{
    declareVariable(ctx->ID()->getText(), stoi(ctx->CONST_INT()->getText()));
    return any();
}

any CodeGenVisitor::visitDeclListArr(CSubsetParser::DeclListArrContext *ctx)
{
    visit(ctx->declaration_list());
    declareVariable(ctx->ID()->getText(), stoi(ctx->CONST_INT()->getText()));
    return any();
}

// Compound statement / statement list


any CodeGenVisitor::visitCompoundWithStatements(CSubsetParser::CompoundWithStatementsContext *ctx)
{
    visit(ctx->statements());
    return any();
}

any CodeGenVisitor::visitCompoundEmpty(CSubsetParser::CompoundEmptyContext *ctx)
{
    return any();
}

any CodeGenVisitor::visitStatementsSingle(CSubsetParser::StatementsSingleContext *ctx)
{
    visit(ctx->statement());
    return any();
}

any CodeGenVisitor::visitStatementsMulti(CSubsetParser::StatementsMultiContext *ctx)
{
    visit(ctx->statements());
    visit(ctx->statement());
    return any();
}

// Phase 1 supports variable declarations, expressions, blocks, println, and return.
// Loops and conditional statements are not part of this phase.

any CodeGenVisitor::visitStmtVarDecl(CSubsetParser::StmtVarDeclContext *ctx)
{
    
    visit(ctx->var_declaration());
    return any();
}

any CodeGenVisitor::visitStmtExprStmt(CSubsetParser::StmtExprStmtContext *ctx)
{
    visit(ctx->expression_statement());
    return any();
}

any CodeGenVisitor::visitStmtCompound(CSubsetParser::StmtCompoundContext *ctx)
{
    scopeStack.push_back({}); // nested block scope
    visit(ctx->compound_statement());
    scopeStack.pop_back();
    return any();
}

//if else part 
any CodeGenVisitor::visitStmtIf(CSubsetParser::StmtIfContext *ctx)
{
    string falseLabel = newLabel();
    string endLabel = newLabel();

    emitComment(ctx->getStart()->getLine());
    visit(ctx->expression());
    emit("cmp eax, 0");
    emit("je " + falseLabel);

    visit(ctx->statement());
    emit("jmp " + endLabel);

    emitLabel(falseLabel);
    emitLabel(endLabel);
    return any();
}

any CodeGenVisitor::visitStmtIfElse(CSubsetParser::StmtIfElseContext *ctx)
{
    string elseLabel = newLabel();
    string endLabel = newLabel();

    emitComment(ctx->getStart()->getLine());
    visit(ctx->expression());
    emit("cmp eax, 0");
    emit("je " + elseLabel);

    visit(ctx->statement(0));
    emit("jmp " + endLabel);

    emitLabel(elseLabel);
    visit(ctx->statement(1));
    emitLabel(endLabel);
    return any();
}

any CodeGenVisitor::visitStmtWhile(CSubsetParser::StmtWhileContext *ctx)
{
    string startLabel = newLabel();
    string endLabel = newLabel();

    emitComment(ctx->getStart()->getLine());
    emitLabel(startLabel);
    visit(ctx->expression());
    emit("cmp eax, 0");
    emit("je " + endLabel);

    visit(ctx->statement());
    emit("jmp " + startLabel);

    emitLabel(endLabel);
    return any();
}

any CodeGenVisitor::visitStmtFor(CSubsetParser::StmtForContext *ctx)
{
    string startLabel = newLabel();
    string endLabel = newLabel();

    // Fixed: expression_statement() returns the base context, which has no
    // .expression() accessor (only ExprStmtExprContext/ExprErrorContext do).
    // Must dynamic_cast to detect the empty ";" case instead.
    auto condCtx = ctx->expression_statement(1);
    bool condEmpty = dynamic_cast<CSubsetParser::ExprStmtEmptyContext *>(condCtx) != nullptr;

    visit(ctx->expression_statement(0)); // init -- ExprStmtEmpty is already a no-op, no guard needed

    emitLabel(startLabel);
    emitComment(ctx->getStart()->getLine());

    if (!condEmpty)
    {
        visit(condCtx);
        emit("cmp eax, 0");
        emit("je " + endLabel);
    }

    visit(ctx->statement());  // body
    visit(ctx->expression()); // update -- grammar guarantees this is always present

    emit("jmp " + startLabel);
    if (!condEmpty) emitLabel(endLabel); // no exit label needed for an unconditional for(;;)
    return any();
}

any CodeGenVisitor::visitStmtPrintln(CSubsetParser::StmtPrintlnContext *ctx)
{
    VarLoc loc = resolve(ctx->ID()->getText());
    emitComment(ctx->getStart()->getLine());
    emit("mov eax, " + locationOf(loc));
    emit("call print_number");
    return any();
}

any CodeGenVisitor::visitStmtReturn(CSubsetParser::StmtReturnContext *ctx)
{
    emitComment(ctx->getStart()->getLine());
    visit(ctx->expression()); // EAX = return value
    emit("jmp " + currentReturnLabel);
    return any();
}

any CodeGenVisitor::visitExprStmtEmpty(CSubsetParser::ExprStmtEmptyContext *ctx)
{
    return any();
}

any CodeGenVisitor::visitExprStmtExpr(CSubsetParser::ExprStmtExprContext *ctx)
{   
    emitComment(ctx->getStart()->getLine()); 
    visit(ctx->expression()); 
    return any();
}



any CodeGenVisitor::visitVarId(CSubsetParser::VarIdContext *ctx)
{
    VarLoc loc = resolve(ctx->ID()->getText());
    emit("mov eax, " + locationOf(loc));
    return any();
}

any CodeGenVisitor::visitVarArray(CSubsetParser::VarArrayContext *ctx)
{
    VarLoc loc = resolve(ctx->ID()->getText());
    genArrayAddressToEdi(loc, ctx->expression());
    emit("mov eax, [edi]");
    return any();
}
//Expressions

any CodeGenVisitor::visitExprLogic(CSubsetParser::ExprLogicContext *ctx)
{
    visit(ctx->logic_expression());
    return any();
}

 
any CodeGenVisitor::visitExprAssign(CSubsetParser::ExprAssignContext *ctx)
{
    emitComment(ctx->getStart()->getLine());
    // Fixed: array-element assignment (arr[i] = ...) was completely
    // unhandled before -- the old code unconditionally dynamic_cast'd to
    // VarIdContext and dereferenced the (null, for an array) result.
    if (auto v = dynamic_cast<CSubsetParser::VarArrayContext *>(ctx->variable()))
    {
        VarLoc loc = resolve(v->ID()->getText());
        genArrayAddressToEdi(loc, v->expression()); // EDI = &arr[i]
        emit("push edi");
        visit(ctx->logic_expression());             // EAX = RHS value
        emit("pop edi");
        emit("mov [edi], eax");
    }
    else if (auto v = dynamic_cast<CSubsetParser::VarIdContext *>(ctx->variable()))
    {
        VarLoc loc = resolve(v->ID()->getText());
        visit(ctx->logic_expression()); // EAX = RHS value
        emit("mov " + locationOf(loc) + ", eax");
    }
    return any();
}


any CodeGenVisitor::visitLogicRel(CSubsetParser::LogicRelContext *ctx)
{
    visit(ctx->rel_expression());
    return any();
}

any CodeGenVisitor::visitLogicOp(CSubsetParser::LogicOpContext *ctx)
{
    //emitComment(ctx->getStart()->getLine());

    string Ltrue = newLabel();
    string Lfalse = newLabel();
    string Lend = newLabel();

    genJumpLogicOp(ctx, Ltrue, Lfalse);

    emitLabel(Ltrue);
    emit("mov eax, 1");
    emit("jmp " + Lend);
    emitLabel(Lfalse);
    emit("mov eax, 0");
    emitLabel(Lend);

    return any();
}

any CodeGenVisitor::visitRelSimple(CSubsetParser::RelSimpleContext *ctx)
{
    visit(ctx->simple_expression());
    return any();
}

any CodeGenVisitor::visitRelOp(CSubsetParser::RelOpContext *ctx)
{
    visit(ctx->simple_expression(0));
    emit("push eax");
    visit(ctx->simple_expression(1));
    emit("mov ecx, eax");
    emit("pop eax");
    emit("cmp eax, ecx");

    string op = ctx->RELOP()->getText();
    string setcc = "sete";
    if (op == "<") setcc = "setl";
    else if (op == "<=") setcc = "setle";
    else if (op == ">") setcc = "setg";
    else if (op == ">=") setcc = "setge";
    else if (op == "==") setcc = "sete";
    else if (op == "!=") setcc = "setne";

    emit(setcc + " al");
    emit("movzx eax, al");
    return any();
}

any CodeGenVisitor::visitSimpleTerm(CSubsetParser::SimpleTermContext *ctx)
{
    visit(ctx->term());
    return any();
}

any CodeGenVisitor::visitSimpleAddop(CSubsetParser::SimpleAddopContext *ctx)
{
    visit(ctx->simple_expression());
    emit("push eax");
    visit(ctx->term());
    emit("mov ecx, eax");
    emit("pop eax");
    emit(ctx->ADDOP()->getText() == "+" ? "add eax, ecx" : "sub eax, ecx");
    return any();
}


any CodeGenVisitor::visitTermUnary(CSubsetParser::TermUnaryContext *ctx)
{
    visit(ctx->unary_expression());
    return any();
}

any CodeGenVisitor::visitTermMulop(CSubsetParser::TermMulopContext *ctx)
{
    visit(ctx->term());
    emit("push eax");
    visit(ctx->unary_expression());
    emit("mov ecx, eax");
    emit("pop eax");

    string op = ctx->MULOP()->getText();
    if (op == "*")
    {
        emit("imul eax, ecx");
    }
    else if (op == "/")
    {
        emit("cdq");
        emit("idiv ecx");
    }
    else // "%"
    {
        emit("cdq");
        emit("idiv ecx");
        emit("mov eax, edx");
    }
    return any();
}

any CodeGenVisitor::visitUnaryAddop(CSubsetParser::UnaryAddopContext *ctx)
{
    visit(ctx->unary_expression());
    if (ctx->ADDOP()->getText() == "-") emit("neg eax");
    return any();
}

any CodeGenVisitor::visitUnaryNot(CSubsetParser::UnaryNotContext *ctx)
{
    visit(ctx->unary_expression());
    emit("cmp eax, 0");
    emit("sete al");
    emit("movzx eax, al");
    return any();
}

any CodeGenVisitor::visitUnaryFactor(CSubsetParser::UnaryFactorContext *ctx)
{
    visit(ctx->factor());
    return any();
}

any CodeGenVisitor::visitFactorVar(CSubsetParser::FactorVarContext *ctx)
{
    visit(ctx->variable());
    return any();
}

any CodeGenVisitor::visitFactorCall(CSubsetParser::FactorCallContext *ctx)
{
    int savedCount = currentCallArgCount; // preserve whatever the enclosing call had counted so far
    currentCallArgCount = 0;

    if (ctx->argument_list())
        visit(ctx->argument_list());

    emit("call " + ctx->ID()->getText());
    if (currentCallArgCount > 0)
        emit("add esp, " + to_string(currentCallArgCount * 4));

    currentCallArgCount = savedCount; // restore so the enclosing call keeps counting correctly
    return any();
}


any CodeGenVisitor::visitFactorParen(CSubsetParser::FactorParenContext *ctx)
{
    visit(ctx->expression());
    return any();
}

any CodeGenVisitor::visitFactorInt(CSubsetParser::FactorIntContext *ctx)
{
    emit("mov eax, " + ctx->CONST_INT()->getText());
    return any();
}

any CodeGenVisitor::visitFactorFloat(CSubsetParser::FactorFloatContext *ctx)
{
    
    double v = stod(ctx->CONST_FLOAT()->getText());
    emit("mov eax, " + to_string((int)v));
    return any();
}

any CodeGenVisitor::visitFactorInc(CSubsetParser::FactorIncContext *ctx)
{
    genVariableAddressToEdi(ctx->variable()); // EDI = &var
    emit("mov eax, [edi]");                   // postfix: old value is the result
    emit("mov ecx, eax");
    emit("add ecx, 1");
    emit("mov [edi], ecx");
    return any();
}

any CodeGenVisitor::visitFactorDec(CSubsetParser::FactorDecContext *ctx)
{
    genVariableAddressToEdi(ctx->variable());
    emit("mov eax, [edi]");
    emit("mov ecx, eax");
    emit("sub ecx, 1");
    emit("mov [edi], ecx");
    return any();
}

any CodeGenVisitor::visitArgListNonEmpty(CSubsetParser::ArgListNonEmptyContext *ctx)
{
    visit(ctx->arguments());
    return any();
}

any CodeGenVisitor::visitArgListEmpty(CSubsetParser::ArgListEmptyContext *ctx)
{
    return any();
}

any CodeGenVisitor::visitArgsMulti(CSubsetParser::ArgsMultiContext *ctx)
{
    visit(ctx->logic_expression()); // this argument (rightmost-so-far) evaluated and pushed first
    emit("push eax");
    currentCallArgCount++;
    visit(ctx->arguments());        // then everything before it, ending with the leftmost pushed last
    return any();
}

any CodeGenVisitor::visitArgsSingle(CSubsetParser::ArgsSingleContext *ctx)
{
    visit(ctx->logic_expression());
    emit("push eax");
    currentCallArgCount++;
    return any();
}

void CodeGenVisitor::genJumpRel(CSubsetParser::Rel_expressionContext *ctx,
                                 const string &trueLbl, const string &falseLbl)
{
    if (auto c = dynamic_cast<CSubsetParser::RelOpContext *>(ctx))
    {
        visit(c->simple_expression(0));
        emit("push eax");
        visit(c->simple_expression(1));
        emit("mov ecx, eax");
        emit("pop eax");
        emit("cmp eax, ecx");

        string op = c->RELOP()->getText();
        string jcc = op == "<"  ? "jl"  :
                     op == "<=" ? "jle" :
                     op == ">"  ? "jg"  :
                     op == ">=" ? "jge" :
                     op == "==" ? "je"  : "jne";
        emit(jcc + " " + trueLbl);
        emit("jmp " + falseLbl);
        return;
    }

    auto c = dynamic_cast<CSubsetParser::RelSimpleContext *>(ctx);
    visit(c->simple_expression());
    emit("cmp eax, 0");
    emit("jne " + trueLbl);
    emit("jmp " + falseLbl);
}





void CodeGenVisitor::genJumpLogicOp(CSubsetParser::LogicOpContext *ctx,
                                     const string &trueLbl, const string &falseLbl)
{
    string mid = newLabel();
    if (ctx->LOGICOP()->getText() == "&&")
    {
        genJumpRel(ctx->rel_expression(0), mid, falseLbl);
        emitLabel(mid);
        genJumpRel(ctx->rel_expression(1), trueLbl, falseLbl);
    }
    else // "||"
    {

        genJumpRel(ctx->rel_expression(0), trueLbl, mid);
        emitLabel(mid);
        genJumpRel(ctx->rel_expression(1), trueLbl, falseLbl);
    }
}