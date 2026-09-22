#ifndef UI_STYLES_HPP
#define UI_STYLES_HPP

#include <string>

namespace Futura {

struct ColorRGBA {
    float r, g, b, a;
};

class UIStyles {
public:
    static constexpr ColorRGBA BG_DARK = {0.05f, 0.07f, 0.12f, 1.0f};
    static constexpr ColorRGBA PANEL_BG = {0.09f, 0.11f, 0.18f, 0.95f};
    static constexpr ColorRGBA NEON_CYAN = {0.0f, 0.88f, 1.0f, 1.0f};
    static constexpr ColorRGBA NEON_PURPLE = {0.65f, 0.2f, 1.0f, 1.0f};
    static constexpr ColorRGBA HAZARD_FIRE = {1.0f, 0.3f, 0.1f, 0.9f};
    static constexpr ColorRGBA HAZARD_SMOKE = {0.6f, 0.6f, 0.65f, 0.7f};
    static constexpr ColorRGBA HAZARD_GAS = {0.2f, 0.9f, 0.3f, 0.8f};
    static constexpr ColorRGBA TEXT_PRIMARY = {0.92f, 0.95f, 1.0f, 1.0f};

    static std::string getThemeCSS();
};

} // namespace Futura

#endif // UI_STYLES_HPP
