#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>

struct vec3 {
	friend std::ostream &operator<<(std::ostream &os, const vec3 &vec31)
	{
		os << "x: " << vec31.x << " y: " << vec31.y << " z: " << vec31.z;
		return os;
	}

	float x,y,z;};

vec3 getPosition()
{
	return vec3{1.0,3.0,2.0};
}

vec3 addPosition(vec3 p1, vec3 p2)
{
	vec3 pa = p1;
	vec3 pb = p2;
	return vec3{
			pa.x+pb.x,
			pa.y+pb.y,
			pa.z+pb.z
	};
}

void printPosition(vec3 p)
{
	std::cout << p;
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
	return parts;
}

int main()
{
	//Get (a) (vec3)
	//Get (b) (vec3)
	//Add (a, b) (c) (vec3)
	//Print (c) (void)

	std::vector<std::string> script = {"a = get","b = get","c = add(a,b)","print(c)"};

	std::map<std::string, std::function<void(void)> > vars;
	for(std::string s : script)
	{
		if(s.find("get") != s.npos)
		{
			std::vector<std::string> parts = split(s, {' '});
			if(parts.size() == 3)
			{
				vars[parts[0]] = std::bind(getPosition);
			}
		}
		else if(s.find("add") != s.npos)
		{
			std::vector<std::string> parts = split(s, {' ','(',',',')'});
			if(parts.size() == 5)
			{
				vars[parts[0]] = std::bind(addPosition,vars[parts[3]],vars[parts[4]]);
			}
		}
	}

	auto func = std::bind(printPosition, std::bind(addPosition,std::bind(getPosition),std::bind(getPosition)));
	func();
	return 0;
}