#ifndef SAKURAE_PROGRAM_HPP
#define SAKURAE_PROGRAM_HPP

#include "module.hpp"

namespace sakuraE::IR {
    class Program {
        fzlib::String ID;

        std::vector<Module*> moduleList;
        // Indicates the current maximum index of moduleList
        int cur = -1;
    public:
        Program(fzlib::String id): ID(id) {}

        Program& buildModule(fzlib::String id, PositionInfo info) {
            Module* module = new Module(id, info);
            moduleList.emplace_back(module);
            cur ++;

            return *this;
        }

        Module* curMod () {
            return moduleList[cur];
        }

        Module* mod(std::size_t index) {
            return moduleList[index];
        }

        fzlib::String toString() {
            fzlib::String result = ID + " {";
            for (auto mod: moduleList) {
                result += mod->toString();
            }
            return result;
        }

        fzlib::String toFullString() {
            fzlib::String result = ID + " {";
            for (auto mod: moduleList) {
                result += mod->toFullString();
            }
            return result;
        }
    };
}

#endif // !SAKURAE_PROGRAM_HPP