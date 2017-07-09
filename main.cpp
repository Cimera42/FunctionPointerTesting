#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <memory>

struct vec3 {
	friend std::ostream &operator<<(std::ostream &os, const vec3 &vec31)
	{
		os << "x: " << vec31.x << " y: " << vec31.y << " z: " << vec31.z;
		return os;
	}

	vec3(float x, float y, float z) : x(x), y(y), z(z) {}

	float x,y,z;};

vec3* getPosition()
{
	return new vec3(1.0,3.0,2.0);
}

class BaseCommand
{
public:
	void* result;

	void *getResult() const
	{
		return result;
	}

	virtual ~BaseCommand() {}

	virtual void operator()() {}
};

vec3* addPosition(BaseCommand* p1, BaseCommand* p2)
{
	(*p1)();
	(*p2)();
	vec3* pa = static_cast<vec3*>(p1->getResult());
	vec3* pb = static_cast<vec3*>(p2->getResult());
	vec3* r = new vec3(
			pa->x+pb->x,
			pa->y+pb->y,
			pa->z+pb->z
	);
	return r;
}

void printPosition(BaseCommand* p)
{
	(*p)();
	std::cout << *static_cast<vec3*>(p->getResult());
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

class GetCommand : public BaseCommand
{
public:
	std::function<vec3*(void)> func;

	GetCommand(std::function<vec3*(void)> f) : func(f){}

	void operator()() {result = func();}
};

class AddCommand : public BaseCommand
{
public:
	BaseCommand* a;
	BaseCommand* b;
	std::function<vec3*(BaseCommand*,BaseCommand*)> func;
	
	AddCommand(std::function<vec3*(BaseCommand*,BaseCommand*)> f, BaseCommand* aa, BaseCommand* bb) 
		: func(f), a(aa), b(bb) {}

	void operator()() {result = func(a,b);}
};

class PrintCommand : public BaseCommand
{
public:
	BaseCommand* a;
	std::function<void(BaseCommand*)> func;

	PrintCommand(std::function<void(BaseCommand*)> f, BaseCommand* aa) : func(f), a(aa) {}

	void operator()() {func(a);}
};

void compile(std::vector<std::string> script)
{
	std::map<std::string, BaseCommand*> vars;
	for(std::string s : script)
	{
		if(s.find("get") != s.npos)
		{
			std::vector<std::string> parts = split(s, {' '});
			if(parts.size() == 3)
			{
				vars[parts[0]] = new GetCommand(getPosition);
			}
		}
		else if(s.find("add") != s.npos)
		{
			std::vector<std::string> parts = split(s, {' ','(',',',')'});
			if(parts.size() == 5)
			{
				vars[parts[0]] = new AddCommand(addPosition,vars[parts[3]],vars[parts[4]]);
			}
		}
		else if(s.find("print") != s.npos)
		{
			std::vector<std::string> parts = split(s, {' ','(',',',')'});
			if(parts.size() == 2)
			{
				BaseCommand* a = new PrintCommand(printPosition, vars[parts[1]]);
				(*a)();
			}
		}
	}
}

int main()
{
	//Get (a) (vec3)
	//Get (b) (vec3)
	//Add (a, b) (c) (vec3)
	//Print (c) (void)

	std::vector<std::string> script = {"a = get",
	                                   "b = get",
	                                   "c = add(a,b)",
	                                   "print(c)",
	                                   "d = add(a,c)",
	                                   "print(d)",
	                                   "d = add(d,d)",
	                                   "print(d)",
	};

	compile(script);
	return 0;
}