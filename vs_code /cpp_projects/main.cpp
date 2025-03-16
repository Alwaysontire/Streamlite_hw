#include <iostream>
#include <vector>
#include <string>

class Employee {
    private:
    std::string name_;

    public:
    explicit Employee(const std::string& name) : name_(name)  {};

    virtual double CalculateSalary() const noexcept = 0;

    virtual ~Employee() noexcept = default;

    const std::string& GetName() const noexcept {
        return this->name_;
    }
};

class HourlyEmployee : public Employee {
    private:
    double hourlyrate_;
    double hoursworked_;

    public:
    HourlyEmployee (const std::string& name, double hourlyrate, double hoursworked) : Employee(name), hourlyrate_(hourlyrate), hoursworked_(hoursworked) {}

    double CalculateSalary() const noexcept override {
        return this-> hourlyrate_ * this->hoursworked_;
    }
};

class SalariedEmployee : public Employee {
    private:
    double monthlysalary_;

    public:
    SalariedEmployee(const std::string& name, double monthlysalary) : Employee(name), monthlysalary_(monthlysalary){};

    double CalculateSalary() const noexcept override {
        return this->monthlysalary_;
    }

    ~SalariedEmployee() noexcept override = default;

};

int main() {
    std::vector<Employee*> employees;
    employees.push_back(new HourlyEmployee("Иван Петров", 200.0, 160.0));
    employees.push_back(new HourlyEmployee("Мария Сидорова", 250.0, 140.0));
    employees.push_back(new SalariedEmployee("Алексей Иванов", 60000.0));
    employees.push_back(new SalariedEmployee("Елена Смирнова", 75000.0));

    for (const auto& employee : employees) {
        std::cout << employee->GetName() << ": " << employee->CalculateSalary();
    }

    return 0;
}


