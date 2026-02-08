#ifndef SAKURAE_ATRI_CONFIG_HPP
#define SAKURAE_ATRI_CONFIG_HPP

#include <iostream>

namespace atri {
    struct DebugConfig {
        bool displayAST = false;
        bool displaySakIR = false;
        bool displayRawLLVMIR = false;
        bool displayOptimizedLLVMIR = false;
    };
}

#endif // !SAKURAE_ATRI_CONFIG_HPP