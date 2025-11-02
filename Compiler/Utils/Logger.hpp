#ifndef SAKORAE_LOGGER_HPP
#define SAKORAE_LOGGER_HPP

#include "Compiler/Error/error.hpp"

namespace sutils {
    inline void print(std::string content) {
        std::cout << "[SakoraUtils - Debug]: " <<  content;
    }

    inline void println(std::string content) {
        std::cout << "[SakoraUtils - Debug]: " <<  content << std::endl;
    }

    inline void reportError(sakoraE::OccurredTerm term, std::string content, sakoraE::PositionInfo pinfo) {
        sakoraE::SakoraError error(term, content, pinfo);
        throw std::runtime_error(error.toString());
    }
}

#endif