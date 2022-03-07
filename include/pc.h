#pragma once

#include <string>

namespace RCVM {
    class PC {
    public:
        PC() = default;

        void absolute_jump(size_t new_pc);

        void relative_jump(int offset);

        void increase();

        size_t current() const;

        operator size_t() const
        {
            return current();
        }

        void force_need_incr();

    private:
        size_t _inst_addr = 0;

        bool _need_increase = true;
    };
}
