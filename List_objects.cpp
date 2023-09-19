#include "List_objects.h"

#include <algorithm>

std::set<char> LETERS{
'а','б','в','г','д','е','ё','ж','з','и','й','к','л','м','н','о',
'п','р','с','т','у','ф','х','ц','ч','ш','щ','ъ','ы','ь','э','ю','я',

'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М',
'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Э', 'Ю', 'Я' };

std::ostream& operator<<(std::ostream& out, Object obj) {
	out << obj.name << ' ';
	out << obj.x << ' ';
	out << obj.y << ' ';
	out << obj.type << ' ';
	out << obj.time;
	return out;
}

void ObjectListHandler::Read(std::string f_name) {
	std::ifstream in;
	in.open(f_name);

	if (in.is_open()) {
		std::string line;
		while (std::getline(in, line))
		{
			Object obj = StringToObj(line);
			objects_.push_back(obj);
		}
	}
	in.close();
}

void ObjectListHandler::Add(const std::string& obj) {
	objects_.push_back(StringToObj(obj));
}

void ObjectListHandler::CreateGroupeAndSave(std::ofstream& out, std::string request, int n_type) {
	if (objects_.empty()) {
		return;
	}

	if (request == "RAW") {
		for (auto& obj : objects_) {
			out << obj << std::endl;
		}
	}
	else if (request == "DIST") {
		CreateGroupByDist();
		SaveDist(out);
	}
	else if (request == "NAME") {
		CreateGroupByName();
		SaveName(out);
	}
	else if (request == "TIME") {
		CreateGroupByTime();
		SaveTime(out);
	}
	else if (request == "TYPE") {
		CreateGroupByType(n_type);
		SaveType(out);
	}
}

bool ObjectListHandler::CreateGroup(std::string g_name, int n_type) {

	if (g_name == "DIST") {
		CreateGroupByDist();
		return true;
	}
	else if (g_name == "NAME") {
		CreateGroupByName();
		return true;
	}
	else if (g_name == "TIME") {
		CreateGroupByTime();
		return true;
	}
	else if (g_name == "TYPE") {
		CreateGroupByType(n_type);
		return true;
	}
	else {
		return false;
	}
}

void ObjectListHandler::SaveDist(std::ostream& out) {
	for (auto& line : group_dist_) {
		out << line.first << std::endl;
		for (auto& tup : line.second) {
			out << "Distance " << std::get<0>(tup) << " from ";
			out << "{" << *std::get<1>(tup) << "}" << " to ";
			out << "{" << *std::get<2>(tup) << "}" << std::endl;
		}
	}
}

void ObjectListHandler::SaveName(std::ostream& out) {
	for (auto& line : group_name_) {
		out << line.first << std::endl;
		for (auto& obj : line.second) {
			out << *obj << std::endl;
		}
	}
}

void ObjectListHandler::SaveTime(std::ostream& out) {
	for (auto& line : group_time_) {
		switch (line.first)
		{
		case 0:
			out << "today";
			break;
		case 1:
			out << "yesterday";
			break;
		case 2:
			out << "this_week";
			break;
		case 3:
			out << "this_month";
			break;
		case 4:
			out << "this_year";
			break;
		case 5:
			out << "earlier";
			break;
		default:
			break;
		}
		out << std::endl;
		for (auto& obj : line.second) {
			out << *obj << std::endl;
		}
	}
}

void ObjectListHandler::SaveType(std::ostream& out) {
	for (auto& line : group_type_) {
		out << line.first << std::endl;
		for (auto& obj : line.second) {
			out << *obj << std::endl;
		}
	}
}

Object ObjectListHandler::StringToObj(const std::string& str) {
	Object result;
	std::stringstream temp(str);

	for (int i = 0; i < 5; ++i) {
		std::string word;
		std::getline(temp, word, ' ');
		switch (i)
		{
		case 0:
			result.name = word;
			break;
		case 1:
			result.x = std::stod(word);
			break;
		case 2:
			result.y = std::stod(word);
			break;
		case 3:
			result.type = word;
			break;
		case 4:
			result.time = std::stod(word);
			break;
		default:
			break;
		}
	}
	return result;
}

void ObjectListHandler::CreateGroupByDist() {

	for (int i = 0; i < objects_.size() - 1; ++i) {
		Object* obj_i = &objects_[i];
		for (int j = i + 1; j < objects_.size(); ++j) {
			Object* obj_j = &objects_[j];
			double dist = ComputeDist(obj_i->x, obj_i->y, obj_j->x, obj_j->y);
			if (dist < 100) {
				auto my_t = std::make_tuple(dist, obj_i, obj_j);
				group_dist_["100"].emplace(my_t);
			}
			else if (dist < 1000) {
				group_dist_["1000"].emplace(std::make_tuple(dist, obj_i, obj_j));
			}
			else if (dist < 10000) {
				group_dist_["10000"].emplace(std::make_tuple(dist, obj_i, obj_j));
			}
			else {
				group_dist_["too_far"].emplace(std::make_tuple(dist, obj_i, obj_j));
			}
		}
	}
}

void ObjectListHandler::CreateGroupByName() {
	for (auto& obj : objects_) {
		char let = obj.name[0];
		if (LETERS.find(let) != LETERS.end()) {
			std::string temp(1, let);
			group_name_[temp].insert(&obj);
		}
		else {
			group_name_["#"].insert(&obj);
		}
	}
}

void ObjectListHandler::CreateGroupByType(int n) {
	for (auto& obj : objects_) {
		if (n > 0) {
			auto count = std::count_if(objects_.begin(), objects_.end(), [obj](Object& elem)
				{ return elem.type == obj.type; });
			if (count > n) {
				group_type_[obj.type].insert(&obj);
			}
			else {
				group_type_["Different"].insert(&obj);
			}
		}
		else {
			group_type_[obj.type].insert(&obj);
		}
	}
}


void ObjectListHandler::CreateGroupByTime() {
	std::time_t cur = std::time(nullptr);
#pragma warning(suppress : 4996)
	std::tm cur_time = *std::gmtime(&cur);
	int cur_week = std::ceil(cur_time.tm_yday / 7);

	for (auto& obj : objects_) {
#pragma warning(suppress : 4996)
		std::tm* time = gmtime(&obj.time);
		int week_obj = std::ceil(time->tm_yday / 7);

		if (time->tm_year == cur_time.tm_year &&
			time->tm_yday == cur_time.tm_yday) {
			group_time_[today].insert(&obj);
		}
		else if (time->tm_year == cur_time.tm_year &&
			time->tm_yday == cur_time.tm_yday - 1) {
			group_time_[yesterday].insert(&obj);
		}
		else if (time->tm_year == cur_time.tm_year &&
			time->tm_mon == cur_time.tm_mon &&
			cur_week == week_obj) {
			group_time_[this_week].insert(&obj);
		}
		else if (time->tm_year == cur_time.tm_year &&
			time->tm_mon == cur_time.tm_mon) {
			group_time_[this_month].insert(&obj);
		}
		else if (time->tm_year == cur_time.tm_year) {
			group_time_[this_year].insert(&obj);
		}
		else {
			group_time_[earlier].insert(&obj);
		}
	}
}
