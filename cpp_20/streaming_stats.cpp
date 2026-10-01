#include <iostream>
#include <vector>
#include <queue>
#include <concepts>
#include <chrono>

template<typename T>
concept NumericStreamable = std::is_floating_point_v<T> || std::is_integral_v<T>;

template<NumericStreamable T = double>
class StreamingTwoHeapStats {
private:
    std::priority_queue<T, std::vector<T>, std::less<T>> max_heap_;
    std::priority_queue<T, std::vector<T>, std::greater<T>> min_heap_;

public:
    struct StepResult {
        T running_value;
        double elapsed_ms;
    };

    StepResult insert(T num) {
        auto start_time = std::chrono::high_resolution_clock::now();

        if (max_heap_.empty() || num <= max_heap_.top()) {
            max_heap_.push(num);
        } else {
            min_heap_.push(num);
        }

        if (max_heap_.size() > min_heap_.size() + 1) {
            T val = max_heap_.top(); max_heap_.pop();
            min_heap_.push(val);
        } else if (min_heap_.size() > max_heap_.size()) {
            T val = min_heap_.top(); min_heap_.pop();
            max_heap_.push(val);
        }

        T running_val = 0;
        if (max_heap_.size() == min_heap_.size()) {
            running_val = (max_heap_.top() + min_heap_.top()) / static_cast<T>(2.0);
        } else {
            running_val = max_heap_.top();
        }

        auto elapsed = std::chrono::high_resolution_clock::now() - start_time;
        double elapsed_ms = std::chrono::duration<double, std::milli>(elapsed).count();

        return StepResult{running_val, elapsed_ms};
    }
};

int main() {
    std::vector<double> stream = {5.0, 15.0, 1.0, 3.0, 8.0, 9.0, 10.0, 12.0, 14.0, 20.0, 2.0, 4.0, 6.0};
    StreamingTwoHeapStats<double> processor;

    for (double val : stream) {
        auto [res, t_ms] = processor.insert(val);
        std::cout << "Value: " << val << " | Median: " << res << " | Time: " << t_ms << " ms\n";
    }
    return 0;
}