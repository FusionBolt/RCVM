#include <string>
#include <vector>
#include <memory>
#include <iostream>
#pragma once

using std::string;
enum class InstType {
Add,
Addr,
Alloc,
Call,
CondJump,
DirectJump,
Div,
FunLabel,
GetLocal,
Label,
LocalVarOperator,
Mul,
Pop,
Push,
Return,
SetLocal,
Sub,
UnsetAddr,
};
struct VMInst
{
  InstType type;
  virtual std::string to_string() { return "VMInst"; }
protected:
  VMInst(InstType t): type(t) {};
  virtual ~VMInst() = default;
};
struct Add : VMInst
{
public:
  Add():VMInst(InstType::Add) {}

  std::string to_string() override 
  { 
    return "";
  }


};

struct Addr : VMInst
{
public:
  Addr(string _seg, int _offset):VMInst(InstType::Addr), seg(_seg), offset(_offset) {}

  std::string to_string() override 
  { 
    return seg +std::to_string(offset);
  }

string seg;
int offset;
};

struct Alloc : VMInst
{
public:
  Alloc(string _class_type):VMInst(InstType::Alloc), class_type(_class_type) {}

  std::string to_string() override 
  { 
    return class_type;
  }

string class_type;
};

struct Call : VMInst
{
public:
  Call(string _klass, string _target):VMInst(InstType::Call), klass(_klass), target(_target) {}

  std::string to_string() override 
  { 
    return klass +target;
  }

string klass;
string target;
};

struct CondJump : VMInst
{
public:
  CondJump(string _cond, int _addr):VMInst(InstType::CondJump), cond(_cond), addr(_addr) {}

  std::string to_string() override 
  { 
    return cond +std::to_string(addr);
  }

string cond;
int addr;
};

struct DirectJump : VMInst
{
public:
  DirectJump(string _target):VMInst(InstType::DirectJump), target(_target) {}

  std::string to_string() override 
  { 
    return target;
  }

string target;
};

struct Div : VMInst
{
public:
  Div():VMInst(InstType::Div) {}

  std::string to_string() override 
  { 
    return "";
  }


};

struct FunLabel : VMInst
{
public:
  FunLabel(string _name):VMInst(InstType::FunLabel), name(_name) {}

  std::string to_string() override 
  { 
    return name;
  }

string name;
};

struct GetLocal : VMInst
{
public:
  GetLocal(int _offset):VMInst(InstType::GetLocal), offset(_offset) {}

  std::string to_string() override 
  { 
    return std::to_string(offset);
  }

int offset;
};

struct Label : VMInst
{
public:
  Label(string _name):VMInst(InstType::Label), name(_name) {}

  std::string to_string() override 
  { 
    return name;
  }

string name;
};

struct LocalVarOperator : VMInst
{
public:
  LocalVarOperator(int _offset):VMInst(InstType::LocalVarOperator), offset(_offset) {}

  std::string to_string() override 
  { 
    return std::to_string(offset);
  }

int offset;
};

struct Mul : VMInst
{
public:
  Mul():VMInst(InstType::Mul) {}

  std::string to_string() override 
  { 
    return "";
  }


};

struct Pop : VMInst
{
public:
  Pop(int _pos):VMInst(InstType::Pop), pos(_pos) {}

  std::string to_string() override 
  { 
    return std::to_string(pos);
  }

int pos;
};

struct Push : VMInst
{
public:
  Push(int _value):VMInst(InstType::Push), value(_value) {}

  std::string to_string() override 
  { 
    return std::to_string(value);
  }

int value;
};

struct Return : VMInst
{
public:
  Return():VMInst(InstType::Return) {}

  std::string to_string() override 
  { 
    return "";
  }


};

struct SetLocal : VMInst
{
public:
  SetLocal(int _offset):VMInst(InstType::SetLocal), offset(_offset) {}

  std::string to_string() override 
  { 
    return std::to_string(offset);
  }

int offset;
};

struct Sub : VMInst
{
public:
  Sub():VMInst(InstType::Sub) {}

  std::string to_string() override 
  { 
    return "";
  }


};

struct UnsetAddr : VMInst
{
public:
  UnsetAddr(string _unset_addr):VMInst(InstType::UnsetAddr), unset_addr(_unset_addr) {}

  std::string to_string() override 
  { 
    return unset_addr;
  }

string unset_addr;
};
std::shared_ptr<VMInst> get_inst(const std::vector<std::string> &list)
{
if (list[0] == "Add") return std::make_shared<Add>();

if (list[0] == "Addr") return std::make_shared<Addr>(list[1], std::stoi(list[2]));

if (list[0] == "Alloc") return std::make_shared<Alloc>(list[1]);

if (list[0] == "Call") return std::make_shared<Call>(list[1], list[2]);

if (list[0] == "CondJump") return std::make_shared<CondJump>(list[1], std::stoi(list[2]));

if (list[0] == "DirectJump") return std::make_shared<DirectJump>(list[1]);

if (list[0] == "Div") return std::make_shared<Div>();

if (list[0] == "FunLabel") return std::make_shared<FunLabel>(list[1]);

if (list[0] == "GetLocal") return std::make_shared<GetLocal>(std::stoi(list[1]));

if (list[0] == "Label") return std::make_shared<Label>(list[1]);

if (list[0] == "LocalVarOperator") return std::make_shared<LocalVarOperator>(std::stoi(list[1]));

if (list[0] == "Mul") return std::make_shared<Mul>();

if (list[0] == "Pop") return std::make_shared<Pop>(std::stoi(list[1]));

if (list[0] == "Push") return std::make_shared<Push>(std::stoi(list[1]));

if (list[0] == "Return") return std::make_shared<Return>();

if (list[0] == "SetLocal") return std::make_shared<SetLocal>(std::stoi(list[1]));

if (list[0] == "Sub") return std::make_shared<Sub>();

if (list[0] == "UnsetAddr") return std::make_shared<UnsetAddr>(list[1]);

throw std::runtime_error("Unknown inst type" + list[0]);
}
