#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class Stack {
   private:
    int size = 0;
    std::list<char> stack;

   public:
    void Push(char a) {
        this->stack.emplace_back(a);
        this->size++;
    }

    void Pop() {
        this->stack.pop_back();
        this->size--;
    }

    int Top() {
        return this->stack.back();
    }

    int Size() {
        return this->size;
    }
};

int main() {
    std::string stroka;
    std::getline(std::cin, stroka);
    std::vector<char> already;
    Stack stack;
    std::transform(stroka.begin(), stroka.end(), stroka.begin(), ::tolower);
    for (char letter : stroka) {
        stack.Push(letter);
    }
    bool balance = true;
    for (char letter : stroka) {
        if (stack.Top() != letter) {
            balance = false;
            break;
        }
        stack.Pop();
    }
    if (balance) {
        std::cout << "YES" << '\n';
    } else {
        std::cout << "NO" << '\n';
    }
    return 0;
}