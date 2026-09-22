#include "../../include/UI/UIStyles.hpp"

namespace Futura {

std::string UIStyles::getThemeCSS() {
    return R"(
        /* FUTURA Cyberpunk Theme Style Specs */
        body { background-color: #0b0f19; color: #e6f0ff; font-family: 'Segoe UI', Roboto, monospace; }
        .tab-panel { background: rgba(15, 22, 36, 0.95); border: 1px solid #00e5ff; box-shadow: 0 0 15px rgba(0,229,255,0.2); }
        .btn-futura { background: linear-gradient(135deg, #00e5ff, #a100ff); color: white; border: none; border-radius: 4px; font-weight: bold; }
        .btn-futura:hover { box-shadow: 0 0 12px #00e5ff; cursor: pointer; }
    )";
}

} // namespace Futura
