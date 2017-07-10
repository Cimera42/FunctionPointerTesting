//
// Created by Tim on 10/07/2017.
//

#ifndef FUNCTIONPOINTERTESTING_MAIN_H
#define FUNCTIONPOINTERTESTING_MAIN_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

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
	std::function<void(BaseCommand*)> func;

	template<typename T>
	T* getVar(int varIndex)
	{
		auto v = (*vars).find(varNames[varIndex]);
		if(v == (*vars).end())
		{
			std::cout << varNames[varIndex] << std::endl;
			throw "Non-existent variable accessed";
		}
		return (T*)v->second;
	}
	
	template <typename T>
	void setVar(int varIndex, T* value)
	{
		(*vars)[varNames[varIndex]] = value;
	}

	void set(std::map<std::string, void *> *vars, std::vector<std::string> varNames){
		BaseCommand::vars = vars;
		BaseCommand::varNames = varNames;
	}
	BaseCommand(std::function<void(BaseCommand*)> func) : func(func) {}

	virtual ~BaseCommand() {}
	virtual void operator()() {
		func(this);
	}

	virtual BaseCommand* copy() {
		return new BaseCommand(func);
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
