#pragma once

namespace RCVM {
    class EvalStack {
    public:
        // 2MB stack memory
        EvalStack() :
                _memory(std::make_unique<char[]>(1024 * 2048)),
                _stack_top(_memory.get()) {
        }

        EvalStack(const EvalStack&) = delete;
        EvalStack operator=(const EvalStack&) = delete;

        void push(int value) {
            top_value() = value;
            _stack_top += 4;
        }

        void pop([[maybe_unused]] int target) {
            // target addr set value top_value()
            _stack_top -= 4;
        }

        template<typename Callable>
        void exec(Callable &&f)
        {
            auto new_v = f(get_value(0), get_value(4));
            _stack_top += 8;
            push(new_v);
        }

        int& top_value() {
            return get_value(0);
        }

        int& get_value(int offset) {
            return *reinterpret_cast<int*>(_stack_top - offset);
        }

        std::unique_ptr<char[]> _memory;
        char *_stack_top;
    };
}