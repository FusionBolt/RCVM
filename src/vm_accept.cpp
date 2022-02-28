#include "rcvm.h"

using namespace RCVM;
void VMInstVisitor::accept(const VMInst &inst)
{
    switch(inst.type)
    {
    case InstType::Add:
      visit(static_cast<const Add&>(inst));break;

    case InstType::Alloc:
      visit(static_cast<const Alloc&>(inst));break;

    case InstType::Call:
      visit(static_cast<const Call&>(inst));break;

    case InstType::CondJump:
      visit(static_cast<const CondJump&>(inst));break;

    case InstType::DirectJump:
      visit(static_cast<const DirectJump&>(inst));break;

    case InstType::Div:
      visit(static_cast<const Div&>(inst));break;

    case InstType::EQ:
      visit(static_cast<const EQ&>(inst));break;

    case InstType::FunLabel:
      visit(static_cast<const FunLabel&>(inst));break;

    case InstType::GT:
      visit(static_cast<const GT&>(inst));break;

    case InstType::GetClassMemberVar:
      visit(static_cast<const GetClassMemberVar&>(inst));break;

    case InstType::GetLocal:
      visit(static_cast<const GetLocal&>(inst));break;

    case InstType::InvokeSuper:
      visit(static_cast<const InvokeSuper&>(inst));break;

    case InstType::JumpAfterIf:
      visit(static_cast<const JumpAfterIf&>(inst));break;

    case InstType::JumpFalse:
      visit(static_cast<const JumpFalse&>(inst));break;

    case InstType::LT:
      visit(static_cast<const LT&>(inst));break;

    case InstType::Label:
      visit(static_cast<const Label&>(inst));break;

    case InstType::Mul:
      visit(static_cast<const Mul&>(inst));break;

    case InstType::Pop:
      visit(static_cast<const Pop&>(inst));break;

    case InstType::Push:
      visit(static_cast<const Push&>(inst));break;

    case InstType::PushThis:
      visit(static_cast<const PushThis&>(inst));break;

    case InstType::RelativeJump:
      visit(static_cast<const RelativeJump&>(inst));break;

    case InstType::Return:
      visit(static_cast<const Return&>(inst));break;

    case InstType::SetClassMemberVar:
      visit(static_cast<const SetClassMemberVar&>(inst));break;

    case InstType::SetLocal:
      visit(static_cast<const SetLocal&>(inst));break;

    case InstType::Sub:
      visit(static_cast<const Sub&>(inst));break;

      default:
        throw std::runtime_error("not supported inst" + inst.to_string());
    }
}
