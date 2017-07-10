//
// Created by Tim on 10/07/2017.
//

#ifndef FUNCTIONPOINTERTESTING_MAIN_H
#define FUNCTIONPOINTERTESTING_MAIN_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

struct vec3 {
	friend std::ostream &operator<<(std::ostream &os, const vec3 &vec31)
	{
		os << "x: " << vec31.x << " y: " << vec31.y << " z: " << vec31.z;
		return os;
	}

	vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	float x,y,z;};

struct BaseCommand
{
	std::map<std::string, void*>* vars;
	std::vector<std::string> varNames;

	template<typename T>
	T* getVar(std::string varName)
	{
		auto v = (*vars).find(varName);
		if(v == (*vars).end())
		{
			std::cout << varName << std::endl;
			throw "Non-existent variable accessed";
		}
		return (T*)v->second;
	}

	void set(std::map<std::string, void *> *vars, std::vector<std::string> varNames){
		BaseCommand::vars = vars;
		BaseCommand::varNames = varNames;
	}
	BaseCommand() {}

	virtual ~BaseCommand() {}
	virtual void operator()() {}

	virtual BaseCommand* copy() {
		return new BaseCommand;
	}
};

struct GetCommand : BaseCommand
{
	GetCommand();

	void set(std::map<std::string, void *> *vars, const std::vector<std::string> &varNames)
		{BaseCommand::set(vars, varNames);}
	void operator()();

	GetCommand* copy() {
		return new GetCommand;
	}
};

struct AddCommand : BaseCommand
{
	AddCommand();

	void set(std::map<std::string, void *> *vars, const std::vector<std::string> &varNames)
			{BaseCommand::set(vars, varNames);}

	void operator()();

	AddCommand* copy() {
		return new AddCommand;
	}
};

struct PrintCommand : public BaseCommand
{
	PrintCommand();

	void set(std::map<std::string, void *> *vars, const std::vector<std::string> &varNames)
		{BaseCommand::set(vars, varNames);}

	void operator()();

	PrintCommand* copy() {
		return new PrintCommand;
	}
};

struct ScriptEngine
{
	std::vector<std::string> script;

	std::vector<BaseCommand*> commands;
	std::map<std::string, void*> vars;

	void compile();
	void run();
};

#endif //FUNCTIONPOINTERTESTING_MAIN_H
