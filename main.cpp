#include "main.h"
#include <memory>
#include <fstream>
#include <algorithm>

vec3* getPosition()
{
	return new vec3(1.0,3.0,2.0);
}

vec3* addPosition(vec3* p1, vec3* p2)
{
	vec3* pa = p1;
	vec3* pb = p2;
	vec3* r = new vec3(
			pa->x+pb->x,
			pa->y+pb->y,
			pa->z+pb->z
	);
	return r;
}

void printPosition(vec3* p)
{
	std::cout << *p;
	std::cout << " printed" << std::endl;
}

std::vector<std::string> split(std::string toSplit, std::vector<char> onMe)
{
	std::vector<std::string> parts;
	std::string upto = "";
	for(int i = 0; i < toSplit.length(); i++)
	{
		bool split = false;
		for(char c : onMe)
		{
			if(toSplit[i] == c)
			{
				parts.push_back(upto);
				upto = "";
				split = true;
				break;
			}
		}
		if(!split)
		{
			upto += toSplit[i];
		}
	}
	if(upto.size() > 0)
		parts.push_back(upto);
	return parts;
}

std::string deleteChars(std::string clearMe, std::vector<char> clearThese)
{
	std::string newString = "";
	for(int i = 0; i < clearMe.length(); i++)
	{
		char cc = clearMe[i];
		bool add = true;
		for(char c : clearThese)
		{
			if(cc == c)
			{
				add = false;
				break;
			}
		}
		if(add)
			newString += cc;
	}
	return newString;
}

struct CommandTemplate
{
	int commandNamePosition;
	std::string commandName;
	int partsSize;
	BaseCommand* commandType;
	std::vector<int> varParts;
};

void ScriptEngine::compile()
{
	std::vector<CommandTemplate> commandTemplates = {
			{1,"get",2, new GetCommand(), {0}},
			{1,"add",4, new AddCommand(), {0,2,3}},
			{0,"print",2, new PrintCommand(), {1}},
	};
	for(std::string s : script)
	{
		std::vector<std::string> parts = split(s, {'=','(',',',')'});
		unsigned int partsSize = (unsigned int) parts.size();
		// command name position, command name, parts size, command type, {variables}
		for(CommandTemplate ct : commandTemplates)
		{
			if(parts[ct.commandNamePosition] == ct.commandName)
			{
				if(partsSize == ct.partsSize)
				{
					std::vector<std::string> varNames;

					std::for_each(ct.varParts.begin(), ct.varParts.end(), [&varNames,parts](int n){
						varNames.push_back(parts[n]);
					});
					BaseCommand* b = ct.commandType->copy();
					b->set(&vars, varNames);
					commands.push_back(b);
				}
			}
		}
	}
}

void ScriptEngine::run()
{
	for(BaseCommand* command : commands)
	{
		(*command)();
	}
}

int main()
{
	std::vector<std::string> scriptLines;
	std::ifstream stream("../script.scr", std::ios::in);
	if(stream.is_open())
	{
		std::string line;
		while(std::getline(stream, line))
			scriptLines.push_back(deleteChars(line, {' ', '\t', ';'}));
		stream.close();
	}

	ScriptEngine scriptEngine;
	scriptEngine.script = scriptLines;
	scriptEngine.compile();
	try
	{
		scriptEngine.run();
	} catch(char const* reason) {
		std::cout << "Cancelling execution *** " << reason << " ***" << std::endl;
	}
	return 0;
}

void GetCommand::operator()()
{
	(*vars)[varNames[0]] = getPosition();
}

GetCommand::GetCommand()
{

}

void AddCommand::operator()()
{
	(*vars)[varNames[0]] = addPosition(getVar<vec3>(varNames[1]), getVar<vec3>(varNames[2]));
}

AddCommand::AddCommand()
{

}

void PrintCommand::operator()()
{
	printPosition(getVar<vec3>(varNames[0]));
}

PrintCommand::PrintCommand()
{

}
