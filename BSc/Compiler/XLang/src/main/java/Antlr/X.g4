grammar X;

@header{
	import XType.Type;
	import XType.ListType;
	import XType.PrimitiveType;
	import XType.PTFType;
	import Xlang.ExpressionInfo;
	import Xlang.ProgramManager;
	import Xlang.ScopeType;
	import Xlang.OperationManager;
	import Utils.Pair;
}

// program

@members{
    ProgramManager programManager = new ProgramManager();
}

program
    :
    {
        programManager.setParser(this);
    }
    (section)*
    {
        programManager.generateCode();
    }
    ;

section
    : (variable_declaration_statement ';')
    | (function_declaration_statement ';')
    | (forward_declaration_typedef_statement ';')
    | (typedef_statement ';')
    | (return_statement ';')
    | function_definition
    | ';'
    ;

// declarations

variable_declaration_statement
    : type_context=variable_type (assignment_context=variable_name_or_assignment_statement ','
    {
        programManager.handleVariableDeclaration($type_context.type, $assignment_context.assignment);

    } )* assignment_context=variable_name_or_assignment_statement
    {
        programManager.handleVariableDeclaration($type_context.type, $assignment_context.assignment);
    }
    ;

variable_name_or_assignment_statement returns [Pair<String, ExpressionInfo> assignment]
    : assignment_context=variable_assignment_statement {$assignment = $assignment_context.assignment;}
    | name=variable_name {$assignment = new Pair<String, ExpressionInfo>($name.text, null);}
    ;

variable_assignment_statement returns [Pair<String, ExpressionInfo> assignment]
    : name=variable_name '=' expression_context=expression_element
    {
        $assignment = new Pair<String, ExpressionInfo>($name.text, $expression_context.info);
    }
    ;


variable_type returns [Type type]
    : name=VALIDNAME {$type = programManager.getEquivalentType($name.text);}
    | ld_type=list_declaration {$type = $ld_type.type;}
    | ptf_type=pointer_to_function_declaration {$type = $ptf_type.type;}
    | sd_type=string_declaration {$type = $sd_type.type;}
    | hld_type=homogeneous_list_declaration {$type = $hld_type.type;}
    ;

homogeneous_list_declaration returns [ListType type]
    : ('list' '(' count=INTEGERVALUE ')' 'of' type_context=variable_type)
    {
        $type = new ListType($type_context.type, Integer.parseInt($count.text));
    }
    ;

string_declaration returns [ListType type]
    : 'string' '(' count=INTEGERVALUE ')'
    {
        $type = new ListType(PrimitiveType.createCharacter(), Integer.parseInt($count.text));
    }
    ;

list_declaration returns[ListType type]
    locals [ArrayList<Pair<Type, String>> blocks = new ArrayList<Pair<Type, String>>()]
    : '[' (block_context=list_declaration_block {$blocks.add($block_context.block);} ',' )*
    block_context=list_declaration_block {$blocks.add($block_context.block);} ']'
    {
        $type = new ListType($blocks);
    }
    ;

list_declaration_block returns [Pair<Type, String> block]
    : type_context=variable_type {$block = new Pair<Type, String>($type_context.type, null);}
    | name=VALIDNAME ':' type_context=variable_type {$block=new Pair<Type, String>($type_context.type, $name.text);}
    ;

pointer_to_function_declaration returns[PTFType type]
    locals [ArrayList<Type> arguments = new ArrayList<Type>()]
    : ('<' ((argument=variable_type {$arguments.add($argument.type);}'*')*
    argument=variable_type {$arguments.add($argument.type);}) '->' return_value_context=variable_type '>')
    {
        $type = new PTFType($arguments, $return_value_context.type);

        if ($type.isUnknown()) {
            programManager.getErrorManager()
            .error("Invalid PTF declaration. void should use only in return type or when PTF has no input argument.");
        }
    }
    ;

function_declaration_statement
    : (return_type=variable_type func_name=variable_name func_arguments=function_declaration_arguments)
    {
        programManager.handleFunctionDeclaration($return_type.type, $func_name.text, $func_arguments.arguments);
    }
    ;

function_declaration_arguments returns[ArrayList<Type> arguments]
    locals [ArrayList<Type> _arguments = new ArrayList<>()]
    : '(' ((((variable_name ':')? var_type=variable_type ','
    {
        $_arguments.add($var_type.type);
    })*
        (variable_name ':')? var_type=variable_type
    {
        $_arguments.add($var_type.type);
    })? ')')
    {
        $arguments = $_arguments;
    }
    ;

forward_declaration_typedef_statement
    : 'typedef' name=variable_name {programManager.handleTypedefForwardDeclaration($name.text);}
    ;


// expressions


expression_element returns [ExpressionInfo info]
    : info_context=variable_assignment_expression {$info = $info_context.info;}
    ;

variable_assignment_expression returns [ExpressionInfo info]
    : left_info_context=calculation_expression {programManager.handleLeftAssignment();}
        '=' right_info_context=variable_assignment_expression
    {
        $info = programManager.handleExpressionAssignment($left_info_context.info, $right_info_context.info);
    }
    | info_context=calculation_expression {$info = $info_context.info;}
    ;

calculation_expression returns [ExpressionInfo info]
    : info_context=logical_or_calculation_expression {$info = $info_context.info;}
    ;

logical_or_calculation_expression returns [ExpressionInfo info]
    : (and_info_context=logical_and_calculation_expression '||' or_info_context=logical_or_calculation_expression)
    {
       $info = programManager.handleLogicalOperation($and_info_context.info, $or_info_context.info, "||");
    }
    | info_context=logical_and_calculation_expression {$info = $info_context.info;}
    ;

logical_and_calculation_expression returns [ExpressionInfo info]
    : (eq_info_context=equality_calculation_expression'&&' and_info_context=logical_and_calculation_expression)
    {
       $info = programManager.handleLogicalOperation($eq_info_context.info, $and_info_context.info, "&&");
    }
    | info_context=equality_calculation_expression {$info = $info_context.info;}
    ;

equality_calculation_expression returns [ExpressionInfo info]
    : (comp_info_context=comparative_calculation_expression op=('==' | '!=')
        eq_info_context=equality_calculation_expression)
    {
        $info = programManager.handleEqualityOperation($comp_info_context.info, $eq_info_context.info, $op.text);
    }
    | info_context=comparative_calculation_expression {$info = $info_context.info;}
    ;

comparative_calculation_expression returns [ExpressionInfo info]
    : (add_info_context=additive_calculation_expression op=('<' | '>')
        comp_info_context=comparative_calculation_expression)
    {
        $info = programManager.handleComparativeOperation($add_info_context.info, $comp_info_context.info, $op.text);
    }
    | info_context=additive_calculation_expression {$info = $info_context.info;}
    ;

additive_calculation_expression returns [ExpressionInfo info]
    : (multi_info_context=multiplicative_calculation_expression op=('+' | '-' )
        add_info_context=additive_calculation_expression)
    {
        $info = programManager.handleNumericOperation($multi_info_context.info, $add_info_context.info, $op.text);
    }
    | info_context=multiplicative_calculation_expression {$info = $info_context.info;}
    ;

multiplicative_calculation_expression returns [ExpressionInfo info]
    : (unary_info_context=unary_calculation_expression op=('*' | '/')
        multi_info_context=multiplicative_calculation_expression)
    {
        $info = programManager.handleNumericOperation($unary_info_context.info, $multi_info_context.info, $op.text);
    }
    | info_context=unary_calculation_expression {$info = $info_context.info;}
    ;

unary_calculation_expression returns [ExpressionInfo info]
    : op=('!' | '-') unary_info_context=unary_calculation_expression
    {
        $info = programManager.handleUnaryOperation($unary_info_context.info, $op.text);
    }
    | info_context=retrieve_element_expression {$info = $info_context.info;}
    ;


retrieve_element_expression returns [ExpressionInfo info]
    : (info_context=primary_calculation_expression element_context=retrieve_element[$info_context.info])
    {
        $info = $element_context.info;
    }
    ;


retrieve_element [ExpressionInfo target_info] returns [ExpressionInfo info]
     locals [ExpressionInfo result]
     :( (('.' name=variable_name)
        {
            $result = programManager.handleListDotOperation($target_info, $name.text);
        }
     |  ('#' index=integer_constant)
        {
            $result = programManager.handleListSharpOperation($target_info, Integer.parseInt($index.text));
        }
     |  ('(' types_context=function_call_agruments ')')
        {
            $result = programManager.handleFunctionCall($target_info, $types_context.types);
        })
     result_context=retrieve_element[$result] {$result = $result_context.info;})
        {
            $info = $result;
        }
     | {$info = $target_info;}
     ;

function_call_agruments returns [ArrayList<ExpressionInfo> types]
    locals [ArrayList<ExpressionInfo> result = new ArrayList<ExpressionInfo>()]
    : ( ((info_context=expression_element ','
    {
        $result.add($info_context.info);
    })* info_context=expression_element
    {
        $result.add($info_context.info);
    })?)
    { $types = $result; }
    ;

primary_calculation_expression returns [ExpressionInfo info]
    :'(' info_context=expression_element ')'
    {
        $info = $info_context.info;
    }
    | name=variable_name
    {
        $info = programManager.createVariableExpressionInfo($name.text);
    }
    | integer_constant
    {
        $info = ExpressionInfo.createInteger();
    }
    | infos_context=list_constant
    {
        $info = ExpressionInfo.createList($infos_context.infos);
    }
    | char_constant
    {
        $info = ExpressionInfo.createCharacter();
    }
    | str=string_constant
    {
        $info = ExpressionInfo.createString($str.text.substring(1, $str.text.length()-1));
    }
    ;

variable_name
    : VALIDNAME
    ;

list_constant returns [ArrayList<ExpressionInfo> infos]
    locals [ArrayList<ExpressionInfo> _infos = new ArrayList<ExpressionInfo>()]
    : ('[' (info_context=expression_element ','
    {
        $_infos.add($info_context.info);
    }
    )* info_context=expression_element
    {
        $_infos.add($info_context.info);
    }']') {$infos = $_infos;}
    ;

char_constant
    : value=CHARVALUE
    {
        programManager.handleCharacterConstant($value.text);
    }
    ;

integer_constant
    : value=INTEGERVALUE
    {
        programManager.handleIntegerConstant(Integer.parseInt($value.text));
    }
    ;

string_constant
    : value=STRINGVALUE
    {
        programManager.handleStringConstant($value.text.substring(1, $value.text.length()-1));
    }
    ;

// function

function_definition
    : ((return_type=variable_type func_name=variable_name func_arguments=function_definition_arguments)
    {
        programManager.handleFunctionDefinition($return_type.type, $func_name.text, $func_arguments.arguments);
    }
    scope[$return_type.type, $func_arguments.arguments, ScopeType.FUNC])
    ;

function_definition_arguments returns[ArrayList<Pair<String, Type>> arguments]
    locals [ArrayList<Pair<String, Type>> _arguments = new ArrayList<>()]
    : '(' ((( name=variable_name_in_function ':' var_type=variable_type ','
    {
        $_arguments.add(new Pair<String, Type>($name.varName, $var_type.type));
    })*
        name=variable_name_in_function ':' var_type=variable_type
    {
        $_arguments.add(new Pair<String, Type>($name.varName, $var_type.type));
    })? ')')
    {
        $arguments = $_arguments;
    }
    ;

variable_name_in_function returns [String varName]
    : name=variable_name {$varName = $name.text;}
    | pbrv=pass_by_reference_variable_name {$varName = $pbrv.varName;}
    ;

pass_by_reference_variable_name returns [String varName]
    : '$' name=VALIDNAME {$varName=$name.text;}
    ;

scope [Type returnType, ArrayList<Pair<String, Type>> arguments, ScopeType scopeType]
    :
    {
        programManager.handleBeginScope($arguments, $scopeType);
    }
    ('{'((scope_statement ';')
        | if_flow_control
        | if_else_flow_control
        | while_flow_control
        | foreach_flow_control
        | scope[null, new ArrayList<Pair<String, Type>>(), ScopeType.NORMAL])*
        '}')
    {
        programManager.handleEndScope();
    }
    ;

scope_statement
    : variable_declaration_statement
    | info_context=expression_element {programManager.handleExpressionStatement($info_context.info);}
    | break_statement {programManager.handleBreakableScope();}
    | return_statement
    | typedef_statement
    |
    ;


return_statement
    : 'return' info_context=expression_element {programManager.handleReturnStatement($info_context.info);}
    | 'return' {programManager.handleReturnStatement(new ExpressionInfo (new PrimitiveType("void")));}
    ;


// typedef

typedef_statement
    : ('typedef' name=variable_name 'as' type=variable_type)
    {
        programManager.handleTypedefStatement($name.text, $type.type);
    }
    ;


// flow controls

if_flow_control
    locals [ArrayList<Pair<String, Type>> arguments = new ArrayList<>(), String ifLabel]
    : (('if' '(' info_context=expression_element ')')
    {
        $ifLabel = programManager.handleBeginIf($info_context.info);
    }
    ( scope[null, $arguments, ScopeType.IF]
    | inline_scope[$arguments, ScopeType.IF]
    | if_else_flow_control
    | if_flow_control
    ))
    {
        programManager.handleEndIf($ifLabel);
    }
    ;


else_flow_control
    locals [ArrayList<Pair<String, Type>> arguments = new ArrayList<>()]
    : 'else'
    (scope[null, $arguments, ScopeType.IF]
    |inline_scope[$arguments, ScopeType.IF]
    |if_flow_control
    |if_else_flow_control
    )
    ;

if_else_flow_control
    locals [ArrayList<Pair<String, Type>> arguments = new ArrayList<>(), Pair<String, String> labels]
    :((
    ('if' '(' info_context=expression_element ')')
    {
        $labels = programManager.handleBeginIfElse($info_context.info);
    }
    (scope[null, $arguments, ScopeType.IF] | inline_scope[$arguments, ScopeType.IF] | if_else_flow_control)
    )
    {
        programManager.handleBeginElse($labels.getLeft(), $labels.getRight());
    }
    else_flow_control
    )
    {
        programManager.handleEndIf($labels.getRight());
    }
    ;

while_flow_control
    locals [ArrayList<Pair<String, Type>> arguments = new ArrayList<>(), String startLabel, String endLabel]
    :
    {
        $startLabel = programManager.handleBeginWhile();
    }
    ('while' '(' info_context=expression_element ')')
    {
        programManager.handleConditionType($info_context.info);
        $endLabel = programManager.handleWhileBranch();
    }
    (scope[null, $arguments, ScopeType.WHILE] | inline_scope[$arguments, ScopeType.WHILE] | if_else_flow_control |
        if_flow_control)
    {
        programManager.handleEndWhile($startLabel, $endLabel);
    }
    ;

foreach_flow_control
    locals [ArrayList<Pair<String, Type>> arguments = new ArrayList<>(), Pair<String, String> labels]
    : 'foreach' '(' (var_type=variable_type name=variable_name)
    {
        $arguments.add(new Pair<String, Type>($name.text, $var_type.type));
        programManager.handleBeginForeach($var_type.type.getSize());
    }
    'in' retrieve_context=retrieve_element_expression ')'
    {
        $arguments.add(new Pair<String, Type>("__FOR_EACH_LIST_VAR__", $retrieve_context.info.getType()));
        $arguments.add(new Pair<String, Type>("__FOR_EACH_LIST_INDEX_VAR__", PrimitiveType.createInteger()));
        $labels = programManager.handleForeachStatement($var_type.type, $name.text, $retrieve_context.info);
    }
    (scope[null, $arguments, ScopeType.FOREACH] | inline_scope[$arguments, ScopeType.FOREACH] |
        if_else_flow_control | if_flow_control)
    {
        programManager.handleEndForeach($var_type.type, $retrieve_context.info, $labels);
    }
    ;

inline_scope [ArrayList<Pair<String, Type>> arguments, ScopeType scopeType]
    :
    {
        programManager.handleBeginScope($arguments, $scopeType);
    }
     (while_flow_control
    | foreach_flow_control
    | scope_statement ';')
    {
        programManager.handleEndScope();
    }
    ;

// break

break_statement
    : 'break'
    ;


// LEXER RULE


INLINECOMMENT
    : ('//' ~[\n]* [\n]) -> skip
    ;

MULTILINECOMMENT
    : '/*' .*? '*/' -> skip
    ;

VALIDNAME
    : [a-zA-Z_][a-zA-Z0-9_]*
    ;

INTEGERVALUE
    :[0-9]+
    ;

CHARVALUE
    :'\'' ~[\n']+ '\''
    ;

STRINGVALUE
    : '"' ~[\n"]+ '"'
    ;

WHITESPACE
    :[ \r\t]+ -> skip
    ;
NEWLINE
    : [\n]
     -> skip
    ;
