#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
struct Task {
  int id;
  std::string title;
  std::string description;
  std::string deadline;
  int priority;
};
std::vector<Task> tasks;
int taskIdCounter = 1;

void addTask() {
  Task newTask;
  newTask.id = taskIdCounter++;

  std::cout << "Enter task title: ";
  std::getline(std::cin, newTask.title);
  if (newTask.title.empty()) {
    std::cout << "Title cannot be empty\n";
    return;
  }
  std::cout << "Enter task description: ";
  std::getline(std::cin, newTask.description);

  std::cout << "Enter task deadline(YYYY-MM-DD): ";
  std::getline(std::cin, newTask.deadline);

  std::cout << "Enter task priority(1-5): ";
  std::cin >> newTask.priority;
  std::cin.ignore();

  tasks.push_back(newTask);
  std::cout << "Task added successfully" << '\n';
}

void listTasks() {
  if (tasks.empty()) {
    std::cout << "No tasks avaliable" << '\n';
    return;
  }
  std::cout << std::setw(5) << "ID" << std::setw(20) << "Title" << std::setw(30)
            << "Deadline" << std::setw(10) << "Priority" << '\n';
  for (const auto &task : tasks) {
    std::cout << std::setw(5) << task.id << std::setw(20) << task.title
              << std::setw(30) << task.deadline << std::setw(10)
              << task.priority << '\n';
  }
}
void deleteTask() {
  int id;
  std::cout << "Enter task ID to delete";
  std::cin >> id;
  std::cin.ignore();

  auto it = std::remove_if(tasks.begin(), tasks.end(),
                           [id](const Task &task) { return task.id == id; });

  if (it != tasks.end()) {
    tasks.erase(it, tasks.end());
    std::cout << "Task delete successfully" << '\n';
  } else {
    std::cout << "Task not found" << '\n';
  }
}

void saveTasksToFile() {
  std::ofstream file("tasks.txt");
  if (!file) {
    std::cerr << "Error opening file for writing.\n";
    return;
  }
  for (const auto &task : tasks) {
    file << task.id << "," << task.title << "," << task.description << ","
         << task.deadline << "," << task.priority << '\n';
  }
  file.close();
  std::cout << "Tasks saved successfuly" << '\n';
}

void loadTasksFromFile() {
  std::ifstream file("tasks.txt");
  if (!file) {
    std::cout << "No previous tasks found.\n";
    return;
  }

  tasks.clear();
  std::string line;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    Task task;
    std::string id, priority;

    std::getline(ss, id, ',');
    std::getline(ss, task.title, ',');
    std::getline(ss, task.description, ',');
    std::getline(ss, task.deadline, ',');
    std::getline(ss, priority, ',');

    if (id.empty() || priority.empty())
      continue;

    task.id = std::stoi(id);
    task.priority = std::stoi(priority);

    tasks.push_back(task);
  }
  file.close();
  std::cout << "Tasks loaded successfully.\n";
}

int main() {
  loadTasksFromFile();
  while (true) {
    std::cout << "\nTask Manager\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. List Tasks\n";
    std::cout << "3. Delete Task\n";
    std::cout << "4. Save and Exit\n";
    std::cout << "Choose an option\n";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
    case 1:
      addTask();
      break;
    case 2:
      listTasks();
      break;
    case 3:
      deleteTask();
      break;
    case 4:
      saveTasksToFile();
      break;
    default:
      std::cout << "Invalid choice.Try again.\n";
    }
  }
  return 0;
}