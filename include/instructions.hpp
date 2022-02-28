#include <string>
#include <vector>
#include <memory>
#include <iostream>
#pragma once

using std::string;
enum class InstType {
Add,
Alloc,
Call,
CondJump,
DirectJump,
Div,
EQ,
FunLabel,
GT,
GetClassMemberVar,
GetLocal,
InvokeSuper,
JumpAfterIf,
JumpFalse,
LT,
Label,
Mul,
Pop,
Push,
PushThis,
RelativeJump,
Return,
SetClassMemberVar,
SetLocal,
Sub,
};
struct VMInst
{
  InstType type;
  virtual std::string to_string() const { return "VMInst"; }
protected:
  VMInst(InstType t): type(t) {};
  virtual ~VMInst() = default;
};
struct Add : VMInst
{
public:
  Add():VMInst(InstType::Add) {}

  std::string to_string() const override 
  { 
    return "Add:";
  }


};

struct Alloc : VMInst
{
public:
  Alloc(string _class_type):VMInst(InstType::Alloc), class_type(_class_type) {}

  std::string to_string() const override 
  { 
    return "Alloc:"+class_type;
  }

string class_type;
};

struct Call : VMInst
{
public:
  Call(string _target, int _argc):VMInst(InstType::Call), target(_target), argc(_argc) {}

  std::string to_string() const override 
  { 
    return "Call:"+target +std::to_string(argc);
  }

string target;
int argc;
};

struct CondJump : VMInst
{
public:
  CondJump(string _cond, int _addr):VMInst(InstType::CondJump), cond(_cond), addr(_addr) {}

  std::string to_string() const override 
  { 
    return "CondJump:"+cond +std::to_string(addr);
  }

string cond;
int addr;
};

struct DirectJump : VMInst
{
public:
  DirectJump(string _target):VMInst(InstType::DirectJump), target(_target) {}

  std::string to_string() const override 
  { 
    return "DirectJump:"+target;
  }

string target;
};

struct Div : VMInst
{
public:
  Div():VMInst(InstType::Div) {}

  std::string to_string() const override 
  { 
    return "Div:";
  }


};

struct EQ : VMInst
{
public:
  EQ():VMInst(InstType::EQ) {}

  std::string to_string() const override 
  { 
    return "EQ:";
  }


};

struct FunLabel : VMInst
{
public:
  FunLabel(string _name):VMInst(InstType::FunLabel), name(_name) {}

  std::string to_string() const override 
  { 
    return "FunLabel:"+name;
  }

string name;
};

struct GT : VMInst
{
public:
  GT():VMInst(InstType::GT) {}

  std::string to_string() const override 
  { 
    return "GT:";
  }


};

struct GetClassMemberVar : VMInst
{
public:
  GetClassMemberVar(int _id):VMInst(InstType::GetClassMemberVar), id(_id) {}

  std::string to_string() const override 
  { 
    return "GetClassMemberVar:"+std::to_string(id);
  }

int id;
};

struct GetLocal : VMInst
{
public:
  GetLocal(int _offset):VMInst(InstType::GetLocal), offset(_offset) {}

  std::string to_string() const override 
  { 
    return "GetLocal:"+std::to_string(offset);
  }

int offset;
};

struct InvokeSuper : VMInst
{
public:
  InvokeSuper(int _argc):VMInst(InstType::InvokeSuper), argc(_argc) {}

  std::string to_string() const override 
  { 
    return "InvokeSuper:"+std::to_string(argc);
  }

int argc;
};

struct JumpAfterIf : VMInst
{
public:
  JumpAfterIf():VMInst(InstType::JumpAfterIf) {}

  std::string to_string() const override 
  { 
    return "JumpAfterIf:";
  }


};

struct JumpFalse : VMInst
{
public:
  JumpFalse(int _offset):VMInst(InstType::JumpFalse), offset(_offset) {}

  std::string to_string() const override 
  { 
    return "JumpFalse:"+std::to_string(offset);
  }

int offset;
};

struct LT : VMInst
{
public:
  LT():VMInst(InstType::LT) {}

  std::string to_string() const override 
  { 
    return "LT:";
  }


};

struct Label : VMInst
{
public:
  Label(string _name):VMInst(InstType::Label), name(_name) {}

  std::string to_string() const override 
  { 
    return "Label:"+name;
  }

string name;
};

struct Mul : VMInst
{
public:
  Mul():VMInst(InstType::Mul) {}

  std::string to_string() const override 
  { 
    return "Mul:";
  }


};

struct Pop : VMInst
{
public:
  Pop(int _pos):VMInst(InstType::Pop), pos(_pos) {}

  std::string to_string() const override 
  { 
    return "Pop:"+std::to_string(pos);
  }

int pos;
};

struct Push : VMInst
{
public:
  Push(int _value):VMInst(InstType::Push), value(_value) {}

  std::string to_string() const override 
  { 
    return "Push:"+std::to_string(value);
  }

int value;
};

struct PushThis : VMInst
{
public:
  PushThis():VMInst(InstType::PushThis) {}

  std::string to_string() const override 
  { 
    return "PushThis:";
  }


};

struct RelativeJump : VMInst
{
public:
  RelativeJump(int _offset):VMInst(InstType::RelativeJump), offset(_offset) {}

  std::string to_string() const override 
  { 
    return "RelativeJump:"+std::to_string(offset);
  }

int offset;
};

struct Return : VMInst
{
public:
  Return():VMInst(InstType::Return) {}

  std::string to_string() const override 
  { 
    return "Return:";
  }


};

struct SetClassMemberVar : VMInst
{
public:
  SetClassMemberVar(int _id):VMInst(InstType::SetClassMemberVar), id(_id) {}

  std::string to_string() const override 
  { 
    return "SetClassMemberVar:"+std::to_string(id);
  }

int id;
};

struct SetLocal : VMInst
{
public:
  SetLocal(int _offset):VMInst(InstType::SetLocal), offset(_offset) {}

  std::string to_string() const override 
  { 
    return "SetLocal:"+std::to_string(offset);
  }

int offset;
};

struct Sub : VMInst
{
public:
  Sub():VMInst(InstType::Sub) {}

  std::string to_string() const override 
  { 
    return "Sub:";
  }


};
inline std::shared_ptr<VMInst> get_inst(const std::vector<std::string> &list)
{
if (list[0] == "Add") return std::make_shared<Add>();

if (list[0] == "Alloc") return std::make_shared<Alloc>(list[1]);

if (list[0] == "Call") return std::make_shared<Call>(list[1], std::stoi(list[2]));

if (list[0] == "CondJump") return std::make_shared<CondJump>(list[1], std::stoi(list[2]));

if (list[0] == "DirectJump") return std::make_shared<DirectJump>(list[1]);

if (list[0] == "Div") return std::make_shared<Div>();

if (list[0] == "EQ") return std::make_shared<EQ>();

if (list[0] == "FunLabel") return std::make_shared<FunLabel>(list[1]);

if (list[0] == "GT") return std::make_shared<GT>();

if (list[0] == "GetClassMemberVar") return std::make_shared<GetClassMemberVar>(std::stoi(list[1]));

if (list[0] == "GetLocal") return std::make_shared<GetLocal>(std::stoi(list[1]));

if (list[0] == "InvokeSuper") return std::make_shared<InvokeSuper>(std::stoi(list[1]));

if (list[0] == "JumpAfterIf") return std::make_shared<JumpAfterIf>();

if (list[0] == "JumpFalse") return std::make_shared<JumpFalse>(std::stoi(list[1]));

if (list[0] == "LT") return std::make_shared<LT>();

if (list[0] == "Label") return std::make_shared<Label>(list[1]);

if (list[0] == "Mul") return std::make_shared<Mul>();

if (list[0] == "Pop") return std::make_shared<Pop>(std::stoi(list[1]));

if (list[0] == "Push") return std::make_shared<Push>(std::stoi(list[1]));

if (list[0] == "PushThis") return std::make_shared<PushThis>();

if (list[0] == "RelativeJump") return std::make_shared<RelativeJump>(std::stoi(list[1]));

if (list[0] == "Return") return std::make_shared<Return>();

if (list[0] == "SetClassMemberVar") return std::make_shared<SetClassMemberVar>(std::stoi(list[1]));

if (list[0] == "SetLocal") return std::make_shared<SetLocal>(std::stoi(list[1]));

if (list[0] == "Sub") return std::make_shared<Sub>();

throw std::runtime_error("Unknown inst type" + list[0]);
}
