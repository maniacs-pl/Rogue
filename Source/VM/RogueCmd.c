//=============================================================================
//  RogueCmd.c
//
//  2015.08.29 by Abe Pralle
//=============================================================================
#include "Rogue.h"

//-----------------------------------------------------------------------------
//  RogueCmdType
//-----------------------------------------------------------------------------
void RogueCmdPrintFn_default( void* cmd, RogueStringBuilder* builder )
{
  RogueStringBuilder_print_c_string( builder, ((RogueCmd*)cmd)->type->name );
}

RogueType* RogueCmdTypeFn_default( void* cmd )
{
  return 0;
}

RogueType* RogueCmdTypeFn_integer( void* cmd )
{
  return ((RogueCmd*)cmd)->type->vm->type_Integer;
}

RogueType* RogueCmdTypeFn_same_as_operand( void* cmd )
{
  return ((RogueCmdUnaryOp*)cmd)->operand->type->type( ((RogueCmdUnaryOp*)cmd)->operand );
}

RogueCmdType* RogueCmdType_create( RogueVM* vm, RogueTokenType token_type,
  const char* name, RogueInteger object_size )
{
  RogueCmdType* cmd_type = (RogueCmdType*) RogueAllocator_allocate( &vm->allocator,
      sizeof(RogueCmdType) );
  cmd_type->vm = vm;
  cmd_type->token_type = token_type;
  cmd_type->object_size = object_size;
  cmd_type->name = name;
  cmd_type->print = RogueCmdPrintFn_default;
  cmd_type->type  = RogueCmdTypeFn_default;
  return cmd_type;
}

void RogueCmdLiteralInteger_print( void* cmd, RogueStringBuilder* builder )
{
  RogueStringBuilder_print_integer( builder, ((RogueCmdLiteralInteger*)cmd)->value );
}

//-----------------------------------------------------------------------------
//  RogueCmd
//-----------------------------------------------------------------------------
void* RogueCmd_create( RogueCmdType* of_type  )
{
  RogueCmd* cmd;
  RogueVM* vm = of_type->vm;

  cmd = (RogueCmd*) RogueAllocator_allocate( &vm->allocator, of_type->object_size );

  cmd->type = of_type;
  cmd->allocation.next_allocation = vm->cmd_objects;
  vm->cmd_objects = cmd;

  return cmd;
}

void  RogueCmd_throw_error( RogueCmd* THIS, const char* message )
{
  RogueParsePosition position;
  position.line = THIS->line;
  position.column = THIS->column;
  ROGUE_THROW( THIS->type->vm, THIS->filepath, position, message );
}

