#include <iostream>
#include <vector>

struct Node{
    int value;
    Node* next;
    Node(int val, Node* nxt = nullptr) : value(val), next(nxt){}
};
class Dequeue {
    private:
    Node* front_;
    Node* back_;
    size_t size_;

    public:

    Dequeue() : front_(nullptr), back_(nullptr), size_(0) {}

    ~Dequeue() {
        while(front_ != back_) {
            Node* tmp = front_;
            front_ = front_->next;
            delete tmp;
        }
    }

    void PushBack(int value) {
        Node* newnode = new Node(value);
        if (empty()) {
            front_ = back_ = newnode;
        } else {
            back_-> next = newnode;
            back_ = newnode;
        }
        ++size_;
    }
    void PushTop(int value) {
        Node* newnode = new Node(value);
        if (empty()) {
            front_ = back_ = newnode;
        } else {
            front_->next = newnode;
            front_ = newnode;
        }
        ++size;
    }
    void PopBack() {
        if (empty()){
            std::cerr << "Dequeue is empty" << '\n';
        } else {
            

        }
    }


}
