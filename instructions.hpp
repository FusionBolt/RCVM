#include <string>
#include <vector>
#include <memory>
#pragma once

using std::string;
enum class InstType {
Add,
Addr,
Call,
CondJump,
DefineFun,
DirectJump,
Div,
FunEnd,
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
protected:
  VMInst(InstType t): type(t) {};
};
struct Add : VMInst
{
public:
  Add():VMInst(InstType::Add) {}

private:

};

struct Addr : VMInst
{
public:
  Addr():VMInst(InstType::Addr) {}

private:

};

struct Call : VMInst
{
public:
  Call():VMInst(InstType::Call) {}

private:

};

struct CondJump : VMInst
{
public:
  CondJump(string cond, int addr):VMInst(InstType::CondJump), _cond(cond), _addr(addr) {}

private:
string _cond;
int _addr;
};

struct DefineFun : VMInst
{
public:
  DefineFun():VMInst(InstType::DefineFun) {}

private:

};

struct DirectJump : VMInst
{
public:
  DirectJump(string target):VMInst(InstType::DirectJump), _target(target) {}

private:
string _target;
};

struct Div : VMInst
{
public:
  Div():VMInst(InstType::Div) {}

private:

};

struct FunEnd : VMInst
{
public:
  FunEnd():VMInst(InstType::FunEnd) {}

private:

};

struct GetLocal : VMInst
{
public:
  GetLocal():VMInst(InstType::GetLocal) {}

private:

};

struct Label : VMInst
{
public:
  Label():VMInst(InstType::Label) {}

private:

};

struct LocalVarOperator : VMInst
{
public:
  LocalVarOperator():VMInst(InstType::LocalVarOperator) {}

private:

};

struct Mul : VMInst
{
public:
  Mul():VMInst(InstType::Mul) {}

private:

};

struct Pop : VMInst
{
public:
  Pop(int pos):VMInst(InstType::Pop), _pos(pos) {}

private:
int _pos;
};

struct Push : VMInst
{
public:
  Push():VMInst(InstType::Push) {}

private:

};

struct Return : VMInst
{
public:
  Return():VMInst(InstType::Return) {}

private:

};

struct SetLocal : VMInst
{
public:
  SetLocal():VMInst(InstType::SetLocal) {}

private:

};

struct Sub : VMInst
{
public:
  Sub():VMInst(InstType::Sub) {}

private:

};

struct UnsetAddr : VMInst
{
public:
  UnsetAddr(string unset_addr):VMInst(InstType::UnsetAddr), _unset_addr(unset_addr) {}

private:
string _unset_addr;
};
std::unique_ptr<VMInst> get_inst(const std::vector<std::string> &list)
{
if (list[0] == "Add") return std::make_unique<Add>();

if (list[0] == "Addr") return std::make_unique<Addr>();

if (list[0] == "Call") return std::make_unique<Call>();

if (list[0] == "CondJump") return std::make_unique<CondJump>(list[1], std::stoi(list[2]));

if (list[0] == "DefineFun") return std::make_unique<DefineFun>();

if (list[0] == "DirectJump") return std::make_unique<DirectJump>(list[1]);

if (list[0] == "Div") return std::make_unique<Div>();

if (list[0] == "FunEnd") return std::make_unique<FunEnd>();

if (list[0] == "GetLocal") return std::make_unique<GetLocal>();

if (list[0] == "Label") return std::make_unique<Label>();

if (list[0] == "LocalVarOperator") return std::make_unique<LocalVarOperator>();

if (list[0] == "Mul") return std::make_unique<Mul>();

if (list[0] == "Pop") return std::make_unique<Pop>(std::stoi(list[1]));

if (list[0] == "Push") return std::make_unique<Push>();

if (list[0] == "Return") return std::make_unique<Return>();

if (list[0] == "SetLocal") return std::make_unique<SetLocal>();

if (list[0] == "Sub") return std::make_unique<Sub>();

if (list[0] == "UnsetAddr") return std::make_unique<UnsetAddr>(list[1]);

throw std::runtime_error("Unknown inst type" + list[0]);
}
