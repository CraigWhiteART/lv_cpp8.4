#pragma once

/**
 * @file colorwheel.hpp
 * @brief Zero-cost wrapper for LVGL ColorWheel widget
 */

#include <lvgl.h>
#include "../core/object.hpp"
#include "../core/event.hpp"
#include "../core/style.hpp"

#if LV_USE_COLORWHEEL

namespace lv {

/**
 * @brief Color mode constants for ColorWheel
 */
struct ColorWheelMode {
    static constexpr auto hue = LV_COLORWHEEL_MODE_HUE;
    static constexpr auto saturation = LV_COLORWHEEL_MODE_SATURATION;
    static constexpr auto value = LV_COLORWHEEL_MODE_VALUE;
};

/**
 * @brief ColorWheel widget wrapper
 *
 * A circular color picker that allows selecting HSV colors.
 * Supports hue, saturation, and value modes.
 *
 * Size: sizeof(void*) - 4 or 8 bytes
 */
class LV_EMPTY_BASES ColorWheel : public ObjectView,
            public ObjectMixin<ColorWheel>,
            public EventMixin<ColorWheel>,
            public StyleMixin<ColorWheel> {
public:
    ColorWheel() noexcept : ObjectView(nullptr) {}
    ColorWheel(wrap_t, lv_obj_t* obj) noexcept : ObjectView(obj) {}

    [[nodiscard]] static ColorWheel create(lv_obj_t* parent, bool knob_recolor = true) {
        return ColorWheel(wrap, lv_colorwheel_create(parent, knob_recolor));
    }
    [[nodiscard]] static ColorWheel create(ObjectView parent, bool knob_recolor = true) {
        return create(parent.get(), knob_recolor);
    }

    // ==================== Color Setters ====================

    /// Set the current HSV color
    bool set_hsv(lv_color_hsv_t hsv) noexcept {
        return lv_colorwheel_set_hsv(m_obj, hsv);
    }

    /// Set the current RGB color
    bool set_rgb(lv_color_t color) noexcept {
        return lv_colorwheel_set_rgb(m_obj, color);
    }

    // ==================== Mode ====================

    /// Set the color mode (hue/saturation/value)
    ColorWheel& mode(lv_colorwheel_mode_t m) noexcept {
        lv_colorwheel_set_mode(m_obj, m);
        return *this;
    }

    /// Set to hue mode
    ColorWheel& mode_hue() noexcept {
        return mode(LV_COLORWHEEL_MODE_HUE);
    }

    /// Set to saturation mode
    ColorWheel& mode_saturation() noexcept {
        return mode(LV_COLORWHEEL_MODE_SATURATION);
    }

    /// Set to value mode
    ColorWheel& mode_value() noexcept {
        return mode(LV_COLORWHEEL_MODE_VALUE);
    }

    /// Fix the color mode (disable long-press to change)
    ColorWheel& mode_fixed(bool fixed) noexcept {
        lv_colorwheel_set_mode_fixed(m_obj, fixed);
        return *this;
    }

    // ==================== Color Getters ====================

    /// Get the current HSV color
    [[nodiscard]] lv_color_hsv_t get_hsv() const noexcept {
        return lv_colorwheel_get_hsv(m_obj);
    }

    /// Get the current RGB color
    [[nodiscard]] lv_color_t get_rgb() const noexcept {
        return lv_colorwheel_get_rgb(m_obj);
    }

    /// Get the current color mode
    [[nodiscard]] lv_colorwheel_mode_t get_mode() const noexcept {
        return lv_colorwheel_get_color_mode(m_obj);
    }

    /// Check if mode is fixed
    [[nodiscard]] bool is_mode_fixed() const noexcept {
        return lv_colorwheel_get_color_mode_fixed(m_obj);
    }
};

} // namespace lv

#endif // LV_USE_COLORWHEEL
