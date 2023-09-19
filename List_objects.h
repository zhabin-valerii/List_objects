#pragma once

#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <cmath>

struct Object {
	std::string name;
	double x;
	double y;
	std::string type;
	time_t time;
};

std::ostream& operator<<(std::ostream& out, Object obj);

enum time_enum {
	today,
	yesterday,
	this_week,
	this_month,
	this_year,
	earlier
};

class ObjectListHandler {
public:
	void Read(std::string f_name);
	
	void Add(const std::string& obj);
	
	void CreateGroupeAndSave(std::ofstream& out, std::string request, int n_type = 0 );
	
	bool CreateGroup(std::string g_name, int n_type = 0);
	
	void SaveDist(std::ostream& out);
	void SaveName(std::ostream& out);
	void SaveTime(std::ostream& out);
	void SaveType(std::ostream& out);

private:
	std::vector<Object> objects_;

	struct cmp_dist
	{
		bool operator() (const std::tuple<double, const Object*, const Object*>& lhs,
			const std::tuple<double, const Object*, const Object*>& rhs) const {
			return std::get<0>(lhs) < std::get<0>(rhs);
		}
	};
	
	struct cmp_name
	{
		bool operator() (const Object* lhs, const Object* rhs) const {
			return lhs->name < rhs->name;
		}
	};
	
	struct cmp_time
	{
		bool operator() (const Object* lhs, const Object* rhs) const {
			return lhs->time > rhs->time;
		}
	};
	
	std::map<std::string, std::set<std::tuple<double, const Object*, const Object*>, cmp_dist>> group_dist_;
	std::map<std::string, std::set<Object*, cmp_name>> group_name_;
	std::map<time_enum, std::set<Object*, cmp_time>> group_time_;
	std::map<std::string, std::set<Object*, cmp_name>> group_type_;
	
	Object StringToObj(const std::string& str);
	
	void CreateGroupByDist();
	
	void CreateGroupByName();
	
	void CreateGroupByType(int n);
	
	void CreateGroupByTime();
	
	static double ComputeDist(double x1, double y1, double x2, double y2) {
		return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}
};