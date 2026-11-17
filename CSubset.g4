grammar CSubset;
import Lexer;

start : program ;

program
    : program unit  # ProgramMulti
    | unit          # ProgramSingle
    ;

unit
    : var_declaration  # UnitVarDecl
    | func_declaration # UnitFuncDecl
    | func_definition  # UnitFuncDef
    ;

func_declaration
    : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON # FuncDeclParams
    | type_specifier ID LPAREN RPAREN SEMICOLON # FuncDecLNoParams
    ;

func_definition
    : type_specifier ID LPAREN parameter_list RPAREN compound_statement # FuncDefParams
    | type_specifier ID LPAREN RPAREN compound_statement # FuncDefNoParams
    ;

parameter_list
    : parameter_list COMMA type_specifier ID # ParamListMulti
    | parameter_list COMMA type_specifier    # ParamListMultiNoId
    | type_specifier ID                      # ParamListSingle
    | type_specifier                         # ParamListSingleNoId
    | type_specifier ADDOP                   # ParamListError
    ;

compound_statement
    : LCURL statements RCURL     # CompoundWithStatements
    | LCURL RCURL                # CompoundEmpty
    ;

var_declaration
    : type_specifier declaration_list SEMICOLON ;

type_specifier
    : INT      # IntType
    | FLOAT    # FloatType
    | VOID     # VoidType
    ;

declaration_list
    : declaration_list COMMA ID                             #DeclListId
    | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD     #DeclListArr
    | ID                                                    #DeclSingleId
    | ID LTHIRD CONST_INT RTHIRD  # DeclSingleArr
    | declaration_list ADDOP ID                                  # DeclError
    ;

statements
    : statement              #StatementsSingle
    | statements statement   #StatementsMulti
    ;

statement
    : var_declaration                                            # StmtVarDecl
    | expression_statement                                       # StmtExprStmt
    | compound_statement                                         # StmtCompound
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement # StmtFor
    | IF LPAREN expression RPAREN statement                      #StmtIf
    | IF LPAREN expression RPAREN statement ELSE statement       #StmtIfElse
    | WHILE LPAREN expression RPAREN statement                   #StmtWhile
    | PRINTLN LPAREN ID RPAREN SEMICOLON                         #StmtPrintln
    | RETURN expression SEMICOLON                                #StmtReturn
    ;   

expression_statement
    : SEMICOLON                 # ExprStmtEmpty
    | expression SEMICOLON      # ExprStmtExpr
    | expression                # ExprError
    ;

variable
    : ID                            # VarId
    | ID LTHIRD expression RTHIRD   # VarArray
    ;

expression
    : logic_expression                      # ExprLogic
    | variable ASSIGNOP logic_expression    # ExprAssign
    ;

logic_expression
    : rel_expression                            # LogicRel
    | rel_expression LOGICOP rel_expression     # LogicOp
    ;

rel_expression
    : simple_expression                             # RelSimple
    | simple_expression RELOP simple_expression     # RelOp
    ;

simple_expression
    : term                           #  SimpleTerm
    | simple_expression ADDOP term   #  SimpleAddop
    | simple_expression ADDOP ASSIGNOP #simpleExpressionError
    ;

term
    : unary_expression              # TermUnary
    | term MULOP unary_expression   # TermMulop
    ;

unary_expression
    : ADDOP unary_expression        # UnaryAddop
    | NOT unary_expression          #UnaryNot
    | factor                        #UnaryFactor
    ;

factor
    : variable                          # FactorVar
    | ID LPAREN argument_list RPAREN    # FactorCall
    | LPAREN expression RPAREN          # FactorParen
    | CONST_INT                         # FactorInt
    | CONST_FLOAT                       # FactorFloat
    | variable INCOP                    # FactorInc
    | variable DECOP                    # FactorDec
    ;

argument_list
    : arguments  # ArgListNonEmpty
    |            # ArgListEmpty
    ;

arguments
    : arguments COMMA logic_expression # ArgsMulti
    | logic_expression                 # ArgsSingle
    ;
