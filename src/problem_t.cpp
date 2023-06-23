//
// Created by matie on 23.06.2023.
//

#include "problem_t.h"

namespace mhe {
    std::ostream &operator<<(std::ostream &o, const problem_t v) {
        o << "{";
        for (auto e: v)
            o << e << ", ";
        o << "}";
        return o;
    }
} // mhe