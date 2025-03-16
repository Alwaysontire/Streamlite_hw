#include <iostream>
#include <queue>

void AddId(std::queue<int>& que, int id) {
    que.push(id);
}

int NextClient(std::queue<int>& que) {
    if (que.empty()) {
        return 0;
    }
    int last = que.front();
    que.pop();
    return last;
}

int Count(std::queue<int>& que) {
    return static_cast<int>(que.size());
}

int main() {
    int n = 0;
    std::cin >> n;
    std::queue<int> que;
    for (int i = 0; i < n; ++i) {
        std::string command;
        std::cin >> command;
        if (command == "ADD") {
            int id = 0;
            std::cin >> id;
            AddId(que, id);
        } else if (command == "COUNT") {
            std::cout << Count(que) << '\n';
        } else {
            int tmp = NextClient(que);
            if (tmp == 0) {
                std::cout << "Queue is empty" << '\n';
            } else {
                std::cout << tmp << '\n';
            }
        }
    }
    return 0;
}