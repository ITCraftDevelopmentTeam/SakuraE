#ifndef SAKORAE_BLOCK_HPP
#define SAKORAE_BLOCK_HPP

#include "instruction.hpp"

namespace sakoraE::IR {
    // SakoraE IR Block
    // Rule: Every block id starts as '@'
    class Block {
        std::vector<Instruction> instructions;
        fzlib::String ID = "@empty-block";
    public:
        Block(fzlib::String id, std::vector<Instruction> ops): instructions(ops), ID(id) {}
        Block(fzlib::String id) {}

        const fzlib::String& getID() {
            return ID;
        }

        void insert(const Instruction& ins) {
            instructions.push_back(ins);
        }

        const Instruction& op(std::size_t pos) {
            return instructions.at(pos);
        }

        const Instruction& operator[] (std::size_t pos) {
            return instructions.at(pos);
        }
    };
}

#endif // !SAKORAE_BLOCK_HPP
