#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <locale>
#include <iomanip>
#include <map>
#include <filesystem>

#include <sys/stat.h>
#include <unistd.h>

#include "csv.cpp"

// #define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while ( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

std::map<std::string_view, int> lecturer_map {
    {"講者A", 0},
	{"講者B", 1},
	{"講者C", 2},
	{"講者D", 3},
	{"講者E", 4},
	{"講者F", 5},
	{"講者G", 6},
	{"講者H", 7},
};

std::map<std::string_view, int> rps_map {
    {"石頭 ✊", 0},
    {"布 🧻", 1},
    {"剪刀 ✂️", 2}
};

auto rd = std::random_device {};
auto rng = std::default_random_engine { rd() };

inline bool file_exists(const std::string& filename)
{
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);
}

class Student {
public:
    std::string name;
    int id;
    int lecturer_id;
    int rps;
    std::vector<int> lecturer_ids;
    std::vector<int> priorities;

    Student(std::string name, int id, int priorities_size, int sections, int rps):
        name(name), id(id), rps(rps)
    {
        this->lecturer_ids.resize(sections);
        for (int i = 0; i < sections; i++) {
            this->lecturer_ids[i] = -1;
        }
        this->priorities.resize(priorities_size);
    }

    friend std::ostream& operator<< (std::ostream& stream, const Student& student)
    {
        stream << student.id + 1 << "," << student.name;
        // for (int i = 0; i < student.priorities.size(); i++) {
        //     stream << student.priorities[i] << ",";
        // }
        // stream << ",";
        for (size_t i = 0; i < student.lecturer_ids.size(); i++) {
            stream << "," << student.lecturer_ids[i] + 1;
        }
        // stream << student.rps;
        return stream;
    }
};

class Lecturer {
public:
    std::string name;
    Lecturer(std::string name, int id, int capacity, int sections);
    void list();
    int add(Student& student, int section);
    int vacancy(int section);

    friend std::ostream& operator<< (std::ostream& stream, const Lecturer& lecturer)
    {
        for (size_t s = 0; s < lecturer.size.size(); s++) {
            for (int i = 0; i < lecturer.size[s]; i++) {
                stream << s + 1 << "," << lecturer.students[s][i].id + 1 << "," << lecturer.students[s][i].name << std::endl;
            }
        }
        return stream;
    }
private:
    int id;
    std::vector<int> capacity;
    std::vector<int> size;
    std::vector<std::vector<Student>> students;
};

Lecturer::Lecturer(std::string name, int id, int capacity, int sections)
    : name(name), id(id)
{
    this->students.resize(sections);
    this->capacity.resize(sections);
    this->size.resize(sections);
    for (int i = 0; i < sections; i++) {
        this->students[i].reserve(this->capacity[i]);
        this->capacity[i] = capacity;
        this->size[i] = 0;
    }
}

int Lecturer::add(Student& student, int section)
{
    if (this->size[section] >= this->capacity[section]) {
        return 0;
    }
    student.lecturer_ids[section] = this->id;
    std::vector<int>::iterator pos = std::find(student.priorities.begin(), student.priorities.end(), this->id);
    if (pos != student.priorities.end()) {
        student.priorities.erase(pos);
    }
    this->students[section].push_back(student);
    this->size[section]++;
    return 1;
}

int Lecturer::vacancy(int section)
{
    return this->capacity[section] - this->size[section];
}

void Lecturer::list()
{
    std::cout << "Lecturer: " << this->name << std::endl;
    std::cout << (*this);
    std::cout << "------------------------" << std::endl;
}

int rps_battle(int player_1, int player_2)
{
    return (3 + player_1 - player_2) % 3;
}

void rps_arena(const std::vector<Student> students, std::vector<int>& priority_list)
{
    size_t len = priority_list.size();
    std::vector<int> winner, tie, loser;
    for (size_t i = 0; i < len; i += 2) {
        if (i + 1 < len) {
            switch (rps_battle(students[priority_list[i]].rps, students[priority_list[i + 1]].rps)) {
            case 0:
                tie.push_back(priority_list[i]);
                tie.push_back(priority_list[i + 1]);
                break;
            case 1:
                winner.push_back(priority_list[i]);
                loser.push_back(priority_list[i + 1]);
                break;
            case 2:
                winner.push_back(priority_list[i + 1]);
                loser.push_back(priority_list[i]);
                break;
            }
        } else {
            tie.push_back(priority_list[i]);
        }
    }

    int pri_ind = 0;
    for (size_t j = 0; j < winner.size(); j++, pri_ind++) {
        priority_list[pri_ind] = winner[j];
    }
    for (size_t j = 0; j < tie.size(); j++, pri_ind++) {
        priority_list[pri_ind] = tie[j];
    }
    for (size_t j = 0; j < loser.size(); j++, pri_ind++) {
        priority_list[pri_ind] = loser[j];
    }
}

void load_student_priorities(std::vector<Student>& students, std::string filename)
{
    std::ifstream file(filename);
    CSVRow row;
    file >> row;
    int id = 0;
    while (file >> row) {
        students.emplace_back(std::string(row[1]), id, 8, 4, rps_map[row[10]]);
        for (size_t i = 0; i < students[id].priorities.size(); i++) {
            students[id].priorities[i] = lecturer_map[row[i + 2]];
        }
        ++id;
    }

    std::vector<int> tmp_priority = {0, 1, 2, 3, 4, 5, 6, 7};
    while (id < 120) {
        std::shuffle(tmp_priority.begin(), tmp_priority.end(), rng);
        std::string dummy_name = "dummy_" + std::to_string(id);
        students.emplace_back(dummy_name, id, 8, 4, 1);
        for (size_t i = 0; i < students[i].priorities.size(); i++) {
            students[id].priorities[i] = tmp_priority[i];
        }
        ++id;
    }
    file.close();
}

void strategy_2(std::vector<Student>& students, std::vector<Lecturer>& lecturers, std::vector<std::vector<int>>& unmatched, int section)
{
    DEBUG_MSG("[*] Using strategy 2");
    // std::queue<Student> student_queue;
    std::queue<std::reference_wrapper<Student>> student_queue;
    std::shuffle(students.begin(), students.end(), rng);

    for (Student& student : students) {
        student_queue.push(std::ref(student));
    }
    while (!student_queue.empty()) {
        Student& student = student_queue.front().get();
        for (size_t i = 0; i < student.priorities.size(); i++) {
            if (lecturers[student.priorities[i]].add(student, section)) {
                break;
            }
        }
        student_queue.pop();
    }
}

void strategy_1(std::vector<Student>& students, std::vector<Lecturer>& lecturers, std::vector<std::vector<int>>& unmatched, int section)
{
    DEBUG_MSG("[*] Using Strategy 1");
    std::map<int, int> matched_student_map;
    std::shuffle(students.begin(), students.end(), rng);

    for (Student student : students) {
        matched_student_map[student.id] = 0;
    }

    int priorities_size = students[0].priorities.size();

    // for each priorities
    for (int i = 0; i < priorities_size; i++) {
        std::vector<std::vector<int>> priority_list(lecturers.size());
        std::map<int, int>::iterator it;
        // get unmatched student
        for (it = matched_student_map.begin(); it != matched_student_map.end(); it++) {
            if (it->second != 1) {
                priority_list[students[it->first].priorities[i]].push_back(it->first);
            }
        }
        // for each lecturer
        for (size_t j = 0; j < lecturers.size(); j++) {
            int vacancies = lecturers[j].vacancy(section);
            if (vacancies <= 0) {
                continue;
            }
            // shuffle the list and include student under the lecturer util there are no capacities
            std::shuffle(priority_list[j].begin(), priority_list[j].end(), rng);
            rps_arena(students, priority_list[j]);
            for (size_t k = 0; k < priority_list[j].size(); k++) {
                if (!lecturers[j].add(students[priority_list[j][k]], section)) {
                    break;
                }
                matched_student_map[priority_list[j][k]] = 1;
            }
        }
    }

    for (std::map<int, int>::iterator it = matched_student_map.begin(); it != matched_student_map.end(); ++it) {
        if (!it->second) {
            unmatched[section].push_back(it->first);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [csv file]" << std::endl;
        exit(1);
    }

    if (!file_exists(argv[1])) {
        std::cerr << "[-] File " << argv[1] << " does not exist" << std::endl;
        exit(1);
    }

    std::vector<Lecturer> lecturers;
    std::vector<Student> students;
    std::vector<std::vector<int>> unmatched;
    while (1) {
        lecturers.reserve(8);
        students.reserve(120);
        unmatched.resize(4);


        std::map<std::string_view, int>::iterator it;
        for (it = lecturer_map.begin(); it != lecturer_map.end(); it++) {
            lecturers.emplace_back(std::string(it->first), it->second, 15, 4);
        }

        load_student_priorities(students, argv[1]);

        for (int i = 0; i < 4; i++) {
            if (argc == 3 && std::string(argv[2]) == "2") {
                strategy_2(students, lecturers, unmatched, i);
            } else {
                strategy_1(students, lecturers, unmatched, i);
            }
        }
        int tot = 0;
        for (Lecturer lecturer : lecturers) {
            for (int s = 0; s < 4; s++) {
                tot += lecturer.vacancy(s);
            }
        }
        if (tot == 0) {
            break;
        }
        lecturers.clear();
        students.clear();
        unmatched.clear();
    }

    std::string result_directory = "result/";
    std::filesystem::create_directory(result_directory);

    std::ofstream res_file;
    for (Lecturer lecturer : lecturers) {
        res_file.open(result_directory + lecturer.name + ".csv", std::ios_base::out);
        res_file << "梯次,學生ID,學生姓名" << std::endl;
        res_file << lecturer;
#ifdef DEBUG
        lecturer.list();
#endif
        res_file.close();
    }


    std::sort(students.begin(), students.end(), [](const auto& lhs, const auto& rhs) { return lhs.id < rhs.id; });
    res_file.open(result_directory + "student_results.csv", std::ios_base::out);
    res_file << "ID,學生姓名,講者一,講者二,講者三,講者四" << std::endl;
    for (Student student : students) {
        res_file << student << std::endl;
#ifdef DEBUG
        std::cout << student << std::endl;
#endif
    }
    res_file.close();


    return 0;
}
