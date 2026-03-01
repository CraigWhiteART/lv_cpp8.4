#pragma once

/**
 * @file meter.hpp
 * @brief Zero-cost wrapper for LVGL Meter widget
 */

#include "../core/version.hpp"

#if LV_USE_METER && !LV_VERSION_AT_LEAST(9, 0, 0)

#include <lvgl.h>
#include "../core/object.hpp"
#include "../core/event.hpp"
#include "../core/style.hpp"

namespace lv {

/**
 * @brief Meter widget wrapper
 *
 * A flexible meter widget that can display gauges, clocks, or any circular scale.
 * Supports multiple scales and indicators (needles, arcs, scale lines).
 *
 * Size: sizeof(void*) - 4 or 8 bytes
 */
class LV_EMPTY_BASES Meter : public ObjectView,
            public ObjectMixin<Meter>,
            public EventMixin<Meter>,
            public StyleMixin<Meter> {
public:
    Meter() noexcept : ObjectView(nullptr) {}
    Meter(wrap_t, lv_obj_t* obj) noexcept : ObjectView(obj) {}

    [[nodiscard]] static Meter create(lv_obj_t* parent) {
        return Meter(wrap, lv_meter_create(parent));
    }
    [[nodiscard]] static Meter create(ObjectView parent) {
        return create(parent.get());
    }

    // ==================== Scale ====================

    /// Add a new scale to the meter
    [[nodiscard]] lv_meter_scale_t* add_scale() noexcept {
        return lv_meter_add_scale(m_obj);
    }

    /// Set the properties of the ticks of a scale
    Meter& set_scale_ticks(lv_meter_scale_t* scale, uint16_t cnt, uint16_t width, uint16_t len,
                           lv_color_t color) noexcept {
        lv_meter_set_scale_ticks(m_obj, scale, cnt, width, len, color);
        return *this;
    }

    /// Make some ticks major ticks and set their attributes
    Meter& set_scale_major_ticks(lv_meter_scale_t* scale, uint16_t nth, uint16_t width,
                                  uint16_t len, lv_color_t color, int16_t label_gap) noexcept {
        lv_meter_set_scale_major_ticks(m_obj, scale, nth, width, len, color, label_gap);
        return *this;
    }

    /// Set the value and angular range of a scale
    Meter& set_scale_range(lv_meter_scale_t* scale, int32_t min, int32_t max,
                           uint32_t angle_range, uint32_t rotation) noexcept {
        lv_meter_set_scale_range(m_obj, scale, min, max, angle_range, rotation);
        return *this;
    }

    // ==================== Indicators ====================

    /// Add a needle line indicator to a scale
    [[nodiscard]] lv_meter_indicator_t* add_needle_line(lv_meter_scale_t* scale, uint16_t width,
                                                         lv_color_t color, int16_t r_mod) noexcept {
        return lv_meter_add_needle_line(m_obj, scale, width, color, r_mod);
    }

    /// Add a needle image indicator to a scale
    [[nodiscard]] lv_meter_indicator_t* add_needle_img(lv_meter_scale_t* scale, const void* src,
                                                        lv_coord_t pivot_x, lv_coord_t pivot_y) noexcept {
        return lv_meter_add_needle_img(m_obj, scale, src, pivot_x, pivot_y);
    }

    /// Add an arc indicator to a scale
    [[nodiscard]] lv_meter_indicator_t* add_arc(lv_meter_scale_t* scale, uint16_t width,
                                                 lv_color_t color, int16_t r_mod) noexcept {
        return lv_meter_add_arc(m_obj, scale, width, color, r_mod);
    }

    /// Add a scale lines indicator to a scale
    [[nodiscard]] lv_meter_indicator_t* add_scale_lines(lv_meter_scale_t* scale,
                                                         lv_color_t color_start,
                                                         lv_color_t color_end,
                                                         bool local, int16_t width_mod) noexcept {
        return lv_meter_add_scale_lines(m_obj, scale, color_start, color_end, local, width_mod);
    }

    // ==================== Indicator Values ====================

    /// Set the value of an indicator (sets both start and end to same value)
    Meter& set_indicator_value(lv_meter_indicator_t* indic, int32_t value) noexcept {
        lv_meter_set_indicator_value(m_obj, indic, value);
        return *this;
    }

    /// Set the start value of an indicator
    Meter& set_indicator_start_value(lv_meter_indicator_t* indic, int32_t value) noexcept {
        lv_meter_set_indicator_start_value(m_obj, indic, value);
        return *this;
    }

    /// Set the end value of an indicator
    Meter& set_indicator_end_value(lv_meter_indicator_t* indic, int32_t value) noexcept {
        lv_meter_set_indicator_end_value(m_obj, indic, value);
        return *this;
    }

    // ==================== Convenience Methods ====================

    /// Create a simple gauge with default scale and needle
    /// Returns the indicator for further customization
    [[nodiscard]] lv_meter_indicator_t* add_simple_needle(uint16_t width, lv_color_t color, int16_t r_mod) {
        lv_meter_scale_t* scale = add_scale();
        return add_needle_line(scale, width, color, r_mod);
    }
};

} // namespace lv

#endif // LV_USE_METER && !LV_VERSION_AT_LEAST(9, 0, 0)
