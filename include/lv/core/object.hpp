#pragma once

/**
 * @file object.hpp
 * @brief Zero-cost RAII wrappers for LVGL objects
 *
 * Provides ObjectView (non-owning) and Object (owning) wrappers for lv_obj_t.
 * ObjectView is a thin wrapper with zero overhead - just a pointer.
 * Object adds RAII semantics for automatic cleanup.
 */

#include <lvgl.h>
#include <utility>
#include <cstdint>
#include "wrap.hpp"
#include "version.hpp"
#include "style.hpp"

namespace lv {

// ==================== Feature Detection ====================

/// Compile-time check for object naming support (for UI automation)
#ifdef LV_USE_OBJ_NAME
inline constexpr bool has_obj_name = LV_USE_OBJ_NAME;
#else
inline constexpr bool has_obj_name = false;
#endif

// ==================== Symbol Constants ====================

namespace symbol {
    constexpr const char* left = LV_SYMBOL_LEFT;
    constexpr const char* right = LV_SYMBOL_RIGHT;
    constexpr const char* up = LV_SYMBOL_UP;
    constexpr const char* down = LV_SYMBOL_DOWN;
    constexpr const char* ok = LV_SYMBOL_OK;
    constexpr const char* close = LV_SYMBOL_CLOSE;
    constexpr const char* plus = LV_SYMBOL_PLUS;
    constexpr const char* minus = LV_SYMBOL_MINUS;
    constexpr const char* home = LV_SYMBOL_HOME;
    constexpr const char* settings = LV_SYMBOL_SETTINGS;
    constexpr const char* wifi = LV_SYMBOL_WIFI;
    constexpr const char* bluetooth = LV_SYMBOL_BLUETOOTH;
    constexpr const char* volume_max = LV_SYMBOL_VOLUME_MAX;
    constexpr const char* volume_mid = LV_SYMBOL_VOLUME_MID;
    constexpr const char* mute = LV_SYMBOL_MUTE;
    constexpr const char* play = LV_SYMBOL_PLAY;
    constexpr const char* pause = LV_SYMBOL_PAUSE;
    constexpr const char* stop = LV_SYMBOL_STOP;
    constexpr const char* prev = LV_SYMBOL_PREV;
    constexpr const char* next = LV_SYMBOL_NEXT;
    constexpr const char* refresh = LV_SYMBOL_REFRESH;
    constexpr const char* edit = LV_SYMBOL_EDIT;
    constexpr const char* trash = LV_SYMBOL_TRASH;
    constexpr const char* save = LV_SYMBOL_SAVE;
    constexpr const char* file = LV_SYMBOL_FILE;
    constexpr const char* folder = LV_SYMBOL_DIRECTORY;
    constexpr const char* upload = LV_SYMBOL_UPLOAD;
    constexpr const char* download = LV_SYMBOL_DOWNLOAD;
    constexpr const char* copy = LV_SYMBOL_COPY;
    constexpr const char* cut = LV_SYMBOL_CUT;
    constexpr const char* paste = LV_SYMBOL_PASTE;
    constexpr const char* warning = LV_SYMBOL_WARNING;
    constexpr const char* list = LV_SYMBOL_LIST;
    constexpr const char* power = LV_SYMBOL_POWER;
    constexpr const char* eye_open = LV_SYMBOL_EYE_OPEN;
    constexpr const char* eye_close = LV_SYMBOL_EYE_CLOSE;
}

/**
 * @brief Non-owning view of an LVGL object
 *
 * This is the base class for all widget wrappers. It holds a raw pointer
 * to lv_obj_t without managing its lifetime. Use this when the object
 * lifetime is managed by LVGL's parent-child relationship.
 *
 * Size: sizeof(void*) - typically 4 or 8 bytes
 * Overhead: Zero - just a pointer wrapper
 */
class LV_EMPTY_BASES ObjectView {
protected:
    lv_obj_t* m_obj;

public:
    /// Construct from raw LVGL object pointer
    constexpr explicit ObjectView(lv_obj_t* obj) noexcept : m_obj(obj) {}

    /// Default constructor creates null view
    constexpr ObjectView() noexcept : m_obj(nullptr) {}

    /// Get the underlying LVGL object pointer
    [[nodiscard]] constexpr lv_obj_t* get() const noexcept { return m_obj; }

    /// Implicit conversion to lv_obj_t* for C API interop
    [[nodiscard]] constexpr operator lv_obj_t*() const noexcept { return m_obj; }

    /// Check if the view points to a valid object
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return m_obj != nullptr; }

    /// Check if two views point to the same object
    [[nodiscard]] constexpr bool operator==(const ObjectView& other) const noexcept {
        return m_obj == other.m_obj;
    }

    // ==================== Performance Getters ====================
    // Note: These do not return *this, but the values for comparison. 
    // They are used by Mixins' fluent _if methods.

    [[nodiscard]] lv_color_t get_bg_color(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_bg_color(m_obj, sel);
    }

    [[nodiscard]] lv_opa_t get_bg_opa(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_bg_opa(m_obj, sel);
    }

    [[nodiscard]] int32_t get_border_width(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_border_width(m_obj, sel);
    }

    [[nodiscard]] int32_t get_radius(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_radius(m_obj, sel);
    }

    [[nodiscard]] lv_color_t get_text_color(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_text_color(m_obj, sel);
    }

    [[nodiscard]] const lv_font_t* get_text_font(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_text_font(m_obj, sel);
    }

    [[nodiscard]] lv_opa_t get_text_opa(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_text_opa(m_obj, sel);
    }

    [[nodiscard]] lv_text_align_t get_text_align(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_text_align(m_obj, sel);
    }

    [[nodiscard]] int16_t get_transform_angle(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_transform_angle(m_obj, sel);
    }

    [[nodiscard]] int16_t get_transform_zoom(lv_style_selector_t sel = 0) const noexcept {
        return lv_obj_get_style_transform_zoom(m_obj, sel);
    }

    // ==================== Performance Guards (Internal Helpers) ====================
    // These are named set_*_if to avoid ambiguity with fluent Mixin methods.
    
    ObjectView& set_bg_color_if(lv_color_t color, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_bg_color(m_obj, sel).full != color.full) {
            lv_obj_set_style_bg_color(m_obj, color, sel);
        }
        return *this;
    }

    ObjectView& set_bg_opa_if(lv_opa_t opa, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_bg_opa(m_obj, sel) != opa) {
            lv_obj_set_style_bg_opa(m_obj, opa, sel);
        }
        return *this;
    }

    ObjectView& set_border_width_if(int32_t width, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_border_width(m_obj, sel) != width) {
            lv_obj_set_style_border_width(m_obj, width, sel);
        }
        return *this;
    }

    ObjectView& set_shadow_opa_if(lv_opa_t opa, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_shadow_opa(m_obj, sel) != opa) {
            lv_obj_set_style_shadow_opa(m_obj, opa, sel);
        }
        return *this;
    }

    ObjectView& set_shadow_color_if(lv_color_t color, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_shadow_color(m_obj, sel).full != color.full) {
            lv_obj_set_style_shadow_color(m_obj, color, sel);
        }
        return *this;
    }

    ObjectView& set_radius_if(int32_t radius, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_radius(m_obj, sel) != radius) {
            lv_obj_set_style_radius(m_obj, radius, sel);
        }
        return *this;
    }

    ObjectView& set_pad_all_if(int32_t pad, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_pad_top(m_obj, sel) != pad ||
            lv_obj_get_style_pad_bottom(m_obj, sel) != pad ||
            lv_obj_get_style_pad_left(m_obj, sel) != pad ||
            lv_obj_get_style_pad_right(m_obj, sel) != pad) {
            lv_obj_set_style_pad_all(m_obj, pad, sel);
        }
        return *this;
    }

    ObjectView& set_text_color_if(lv_color_t color, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_text_color(m_obj, sel).full != color.full) {
            lv_obj_set_style_text_color(m_obj, color, sel);
        }
        return *this;
    }

    ObjectView& set_text_font_if(const lv_font_t* font, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_text_font(m_obj, sel) != font) {
            lv_obj_set_style_text_font(m_obj, font, sel);
        }
        return *this;
    }

    ObjectView& set_text_opa_if(lv_opa_t opa, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_text_opa(m_obj, sel) != opa) {
            lv_obj_set_style_text_opa(m_obj, opa, sel);
        }
        return *this;
    }

    ObjectView& set_text_align_if(lv_text_align_t align, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_text_align(m_obj, sel) != align) {
            lv_obj_set_style_text_align(m_obj, align, sel);
        }
        return *this;
    }

    ObjectView& set_transform_angle_if(int16_t angle, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_transform_angle(m_obj, sel) != angle) {
            lv_obj_set_style_transform_angle(m_obj, angle, sel);
        }
        return *this;
    }

    ObjectView& set_transform_zoom_if(int16_t zoom, lv_style_selector_t sel = 0) noexcept {
        if (lv_obj_get_style_transform_zoom(m_obj, sel) != zoom) {
            lv_obj_set_style_transform_zoom(m_obj, zoom, sel);
        }
        return *this;
    }

    ObjectView& set_long_mode_if(lv_label_long_mode_t mode) noexcept {
        if (lv_label_get_long_mode(m_obj) != mode) {
            lv_label_set_long_mode(m_obj, mode);
        }
        return *this;
    }

    ObjectView& set_size_if(int32_t w, int32_t h) noexcept {
        if (lv_obj_get_width(m_obj) != w || lv_obj_get_height(m_obj) != h) {
            lv_obj_set_size(m_obj, w, h);
        }
        return *this;
    }

    ObjectView& set_width_if(int32_t w) noexcept {
        if (lv_obj_get_width(m_obj) != w) {
            lv_obj_set_width(m_obj, w);
        }
        return *this;
    }

    ObjectView& set_height_if(int32_t h) noexcept {
        if (lv_obj_get_height(m_obj) != h) {
            lv_obj_set_height(m_obj, h);
        }
        return *this;
    }

    ObjectView& set_pos_if(int32_t x, int32_t y) noexcept {
        if (lv_obj_get_x(m_obj) != x || lv_obj_get_y(m_obj) != y) {
            lv_obj_set_pos(m_obj, x, y);
        }
        return *this;
    }

    ObjectView& set_x_if(int32_t x) noexcept {
        if (lv_obj_get_x(m_obj) != x) {
            lv_obj_set_x(m_obj, x);
        }
        return *this;
    }

    ObjectView& set_y_if(int32_t y) noexcept {
        if (lv_obj_get_y(m_obj) != y) {
            lv_obj_set_y(m_obj, y);
        }
        return *this;
    }

    ObjectView& set_align_if(lv_align_t alignment, int32_t x_ofs = 0, int32_t y_ofs = 0) noexcept {
        if (lv_obj_get_style_align(m_obj, 0) != alignment ||
            lv_obj_get_style_x(m_obj, 0) != x_ofs ||
            lv_obj_get_style_y(m_obj, 0) != y_ofs) {
            lv_obj_align(m_obj, alignment, x_ofs, y_ofs);
        }
        return *this;
    }

    ObjectView& set_visible_if(bool v) noexcept {
        bool cur = lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        if (v && cur) lv_obj_remove_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        else if (!v && !cur) lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        return *this;
    }

    ObjectView& set_show_if() noexcept { return set_visible_if(true); }
    ObjectView& set_hide_if() noexcept { return set_visible_if(false); }

};

/**
 * @brief Owning wrapper for LVGL object with RAII semantics
 *
 * Object owns the lv_obj_t and will delete it in destructor.
 * Move-only to prevent double deletion.
 *
 * Note: In LVGL, parent owns children. When you create a widget with a parent,
 * the parent will delete children automatically. Use release() to transfer
 * ownership to LVGL's parent-child system.
 *
 * Size: sizeof(void*) - typically 4 or 8 bytes
 * Overhead: Just destructor call (inlined)
 */
class Object : public ObjectView {
public:
    /// Create a basic object with parent
    explicit Object(lv_obj_t* parent)
        : ObjectView(lv_obj_create(parent)) {}

    /// Take ownership of existing object
    struct adopt_t {};
    static constexpr adopt_t adopt{};

    Object(adopt_t, lv_obj_t* obj) noexcept
        : ObjectView(obj) {}

    /// Destructor deletes the LVGL object
    ~Object() noexcept {
        if (m_obj) {
            lv_obj_delete(m_obj);
        }
    }

    // Non-copyable
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

    // Move-only
    Object(Object&& other) noexcept : ObjectView(other.m_obj) {
        other.m_obj = nullptr;
    }

    Object& operator=(Object&& other) noexcept {
        if (this != &other) {
            if (m_obj) {
                lv_obj_delete(m_obj);
            }
            m_obj = other.m_obj;
            other.m_obj = nullptr;
        }
        return *this;
    }

    /// Release ownership and return raw pointer
    /// After this call, the object is no longer managed
    [[nodiscard]] lv_obj_t* release() noexcept {
        auto* p = m_obj;
        m_obj = nullptr;
        return p;
    }

    /// Reset to manage a different object (deletes current if any)
    void reset(lv_obj_t* obj = nullptr) noexcept {
        if (m_obj) {
            lv_obj_delete(m_obj);
        }
        m_obj = obj;
    }

    // ==================== Performance Guards (_if variants) ====================
    // Note: These are moved here to be accessible via ObjectView (non-owning views)
    // while maintaining their fluent return types when called on derived widgets.
    
    /// Set size ONLY if it differs from current size
    ObjectView& size_if(int32_t w, int32_t h) noexcept {
        if (lv_obj_get_width(m_obj) != w || lv_obj_get_height(m_obj) != h) {
            lv_obj_set_size(m_obj, w, h);
        }
        return *this;
    }

    /// Set width ONLY if it differs from current width
    ObjectView& width_if(int32_t w) noexcept {
        if (lv_obj_get_width(m_obj) != w) {
            lv_obj_set_width(m_obj, w);
        }
        return *this;
    }

    /// Set height ONLY if it differs from current height
    ObjectView& height_if(int32_t h) noexcept {
        if (lv_obj_get_height(m_obj) != h) {
            lv_obj_set_height(m_obj, h);
        }
        return *this;
    }

    /// Set position ONLY if it differs from current position
    ObjectView& pos_if(int32_t x, int32_t y) noexcept {
        if (lv_obj_get_x(m_obj) != x || lv_obj_get_y(m_obj) != y) {
            lv_obj_set_pos(m_obj, x, y);
        }
        return *this;
    }

    /// Set X ONLY if changed
    ObjectView& x_if(int32_t x) noexcept {
        if (lv_obj_get_x(m_obj) != x) {
            lv_obj_set_x(m_obj, x);
        }
        return *this;
    }

    /// Set Y ONLY if changed
    ObjectView& y_if(int32_t y) noexcept {
        if (lv_obj_get_y(m_obj) != y) {
            lv_obj_set_y(m_obj, y);
        }
        return *this;
    }

    /// Set alignment ONLY if target resolved coordinates differ
    ObjectView& align_if(lv_align_t alignment, int32_t x_ofs, int32_t y_ofs) noexcept {
        if (alignment == LV_ALIGN_CENTER) {
            lv_obj_t* p = lv_obj_get_parent(m_obj);
            if (p) {
                int32_t pw = lv_obj_get_width(p);
                int32_t ph = lv_obj_get_height(p);
                int32_t ow = lv_obj_get_width(m_obj);
                int32_t oh = lv_obj_get_height(m_obj);
                int32_t tx = (pw - ow) / 2 + x_ofs;
                int32_t ty = (ph - oh) / 2 + y_ofs;
                if (lv_obj_get_x(m_obj) == tx && lv_obj_get_y(m_obj) == ty) {
                    return *this;
                }
            }
        }
        lv_obj_align(m_obj, alignment, x_ofs, y_ofs);
        return *this;
    }

    /// Show ONLY if hidden
    ObjectView& show_if() noexcept {
        if (lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_remove_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        }
        return *this;
    }

    /// Hide ONLY if visible
    ObjectView& hide_if() noexcept {
        if (!lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        }
        return *this;
    }

    /// Set visibility ONLY if changed
    ObjectView& visible_if(bool v) noexcept {
        bool cur = lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        if (v && cur) lv_obj_remove_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        else if (!v && !cur) lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        return *this;
    }

    // ==================== Performance Guards (Style _if variants) ====================

    /// Set text color ONLY if it differs
    ObjectView& text_color_if(lv_color_t color, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_text_color(m_obj, selector).full != color.full) {
            lv_obj_set_style_text_color(m_obj, color, selector);
        }
        return *this;
    }

    /// Set text font ONLY if it differs
    ObjectView& text_font_if(const lv_font_t* font, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_text_font(m_obj, selector) != font) {
            lv_obj_set_style_text_font(m_obj, font, selector);
        }
        return *this;
    }

    /// Set background color ONLY if it differs
    ObjectView& bg_color_if(lv_color_t color, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_bg_color(m_obj, selector).full != color.full) {
            lv_obj_set_style_bg_color(m_obj, color, selector);
        }
        return *this;
    }

    /// Set background opacity ONLY if it differs
    ObjectView& bg_opa_if(lv_opa_t opa, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_bg_opa(m_obj, selector) != opa) {
            lv_obj_set_style_bg_opa(m_obj, opa, selector);
        }
        return *this;
    }

    /// Set border width ONLY if it differs
    ObjectView& border_width_if(lv_coord_t width, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_border_width(m_obj, selector) != width) {
            lv_obj_set_style_border_width(m_obj, width, selector);
        }
        return *this;
    }

    /// Set transform angle ONLY if it differs
    ObjectView& transform_angle_if(int16_t angle, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_transform_angle(m_obj, selector) != angle) {
            lv_obj_set_style_transform_angle(m_obj, angle, selector);
        }
        return *this;
    }

    /// Set transform zoom ONLY if it differs
    ObjectView& transform_zoom_if(int16_t zoom, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_transform_zoom(m_obj, selector) != zoom) {
            lv_obj_set_style_transform_zoom(m_obj, zoom, selector);
        }
        return *this;
    }

    /// Set arc line color ONLY if it differs
    ObjectView& arc_color_if(lv_color_t color, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_arc_color(m_obj, selector).full != color.full) {
            lv_obj_set_style_arc_color(m_obj, color, selector);
        }
        return *this;
    }

    /// Set opa ONLY if it differs
    ObjectView& opa_if(lv_opa_t opa, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_opa(m_obj, selector) != opa) {
            lv_obj_set_style_opa(m_obj, opa, selector);
        }
        return *this;
    }

    /// Set style property by color ONLY if it differs
    ObjectView& part_color_if(lv_color_t color, lv_style_selector_t selector = 0) noexcept {
        // This is a generic helper used in some parts of the code for arc/line parts
        if (lv_obj_get_style_arc_color(m_obj, selector).full != color.full) {
            lv_obj_set_style_arc_color(m_obj, color, selector);
        }
        return *this;
    }

    /// Set style opa by part ONLY if it differs
    ObjectView& part_opa_if(lv_opa_t opa, lv_style_selector_t selector = 0) noexcept {
        if (lv_obj_get_style_opa(m_obj, selector) != opa) {
            lv_obj_set_style_opa(m_obj, opa, selector);
        }
        return *this;
    }
};

// ==================== Zero-Cost Verification ====================

// Static assertions to verify zero-overhead abstraction at compile time
// These ensure ObjectView is exactly the size of a pointer
static_assert(sizeof(ObjectView) == sizeof(void*),
    "ObjectView must be exactly pointer-sized for zero overhead");
static_assert(sizeof(Object) == sizeof(void*),
    "Object must be exactly pointer-sized for zero overhead");

// ==================== Object Mixin for Widget Fluent API ====================

/**
 * @brief CRTP mixin providing common fluent object methods
 *
 * This mixin eliminates duplication of size(), width(), height(), pos(),
 * align(), center(), grow(), etc. across all widget wrappers.
 *
 * Widgets inherit from this and get proper fluent return types automatically.
 *
 * Example:
 * @code
 * class MyWidget : public ObjectView,
 *                  public ObjectMixin<MyWidget>,
 *                  public EventMixin<MyWidget> {
 *     // size(), width(), etc. return MyWidget& automatically
 * };
 * @endcode
 */
template<typename Derived>
class LV_EMPTY_BASES ObjectMixin {
private:
    [[nodiscard]] lv_obj_t* obj() noexcept {
        lv_obj_t* p = static_cast<Derived*>(this)->get();
        LV_ASSERT_NULL(p);
        return p;
    }

    [[nodiscard]] lv_obj_t* obj() const noexcept {
        lv_obj_t* p = static_cast<const Derived*>(this)->get();
        LV_ASSERT_NULL(p);
        return p;
    }

public:
    // ==================== Size ====================

    /// Set size in pixels
    Derived& size(int32_t w, int32_t h) noexcept {
        lv_obj_set_size(obj(), w, h);
        return *static_cast<Derived*>(this);
    }

    /// Set width in pixels
    Derived& width(int32_t w) noexcept {
        lv_obj_set_width(obj(), w);
        return *static_cast<Derived*>(this);
    }

    /// Set height in pixels
    Derived& height(int32_t h) noexcept {
        lv_obj_set_height(obj(), h);
        return *static_cast<Derived*>(this);
    }

    /// Set size to content
    Derived& size_content() noexcept {
        lv_obj_set_size(obj(), LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        return *static_cast<Derived*>(this);
    }

    /// Set width to percentage of parent
    Derived& width_pct(int32_t pct) noexcept {
        lv_obj_set_width(obj(), LV_PCT(pct));
        return *static_cast<Derived*>(this);
    }

    /// Set height to percentage of parent
    Derived& height_pct(int32_t pct) noexcept {
        lv_obj_set_height(obj(), LV_PCT(pct));
        return *static_cast<Derived*>(this);
    }

    /// Fill parent width (100%)
    Derived& fill_width() noexcept {
        lv_obj_set_width(obj(), LV_PCT(100));
        return *static_cast<Derived*>(this);
    }

    /// Fill parent height (100%)
    Derived& fill_height() noexcept {
        lv_obj_set_height(obj(), LV_PCT(100));
        return *static_cast<Derived*>(this);
    }

    /// Fill parent (100% width and height)
    Derived& fill() noexcept {
        lv_obj_set_size(obj(), LV_PCT(100), LV_PCT(100));
        return *static_cast<Derived*>(this);
    }

    /// Set size ONLY if it differs from current size (Performance guard)
    Derived& size_if(int32_t w, int32_t h) noexcept {
        lv_obj_t* p = obj();
        if (lv_obj_get_width(p) != w || lv_obj_get_height(p) != h) {
            lv_obj_set_size(p, w, h);
        }
        return *static_cast<Derived*>(this);
    }

    /// Set width ONLY if it differs from current width
    Derived& width_if(int32_t w) noexcept {
        lv_obj_t* p = obj();
        if (lv_obj_get_width(p) != w) {
            lv_obj_set_width(p, w);
        }
        return *static_cast<Derived*>(this);
    }

    /// Set height ONLY if it differs from current height
    Derived& height_if(int32_t h) noexcept {
        lv_obj_t* p = obj();
        if (lv_obj_get_height(p) != h) {
            lv_obj_set_height(p, h);
        }
        return *static_cast<Derived*>(this);
    }

    // ==================== Position ====================

    /// Set position relative to parent
    Derived& pos(int32_t x, int32_t y) noexcept {
        lv_obj_set_pos(obj(), x, y);
        return *static_cast<Derived*>(this);
    }

    /// Set X position
    Derived& x(int32_t x) noexcept {
        lv_obj_set_x(obj(), x);
        return *static_cast<Derived*>(this);
    }

    /// Set Y position
    Derived& y(int32_t y) noexcept {
        lv_obj_set_y(obj(), y);
        return *static_cast<Derived*>(this);
    }

    /// Set position ONLY if it differs from current position
    Derived& pos_if(int32_t x, int32_t y) noexcept {
        lv_obj_t* p = obj();
        if (lv_obj_get_x(p) != x || lv_obj_get_y(p) != y) {
            lv_obj_set_pos(p, x, y);
        }
        return *static_cast<Derived*>(this);
    }
    
    /// Set X ONLY if changed
    Derived& x_if(int32_t x) noexcept {
        lv_obj_t* p = obj();
        if (lv_obj_get_x(p) != x) {
            lv_obj_set_x(p, x);
        }
        return *static_cast<Derived*>(this);
    }

    /// Set Y ONLY if changed
    Derived& y_if(int32_t y) noexcept {
        lv_obj_t* p = obj();
        if (lv_obj_get_y(p) != y) {
            lv_obj_set_y(p, y);
        }
        return *static_cast<Derived*>(this);
    }

    // ==================== Alignment ====================

    /// Align relative to parent with offset
    Derived& align(lv_align_t alignment, int32_t x_ofs, int32_t y_ofs) noexcept {
        lv_obj_align(obj(), alignment, x_ofs, y_ofs);
        return *static_cast<Derived*>(this);
    }

    /// Set alignment mode (without changing position)
    Derived& align(lv_align_t alignment) noexcept {
        lv_obj_set_align(obj(), alignment);
        return *static_cast<Derived*>(this);
    }

    /// Set alignment ONLY if target resolved coordinates differ (Performance guard)
    /// Note: Currently only manual implementation for LV_ALIGN_CENTER (most common).
    /// Fallbacks to lv_obj_align if alignment is not CENTER.
    Derived& align_if(lv_align_t alignment, int32_t x_ofs = 0, int32_t y_ofs = 0) noexcept {
        lv_obj_t* m_obj = obj();
        if (alignment == LV_ALIGN_CENTER) {
            lv_obj_t* p = lv_obj_get_parent(m_obj);
            if (p) {
                int32_t pw = lv_obj_get_width(p);
                int32_t ph = lv_obj_get_height(p);
                int32_t ow = lv_obj_get_width(m_obj);
                int32_t oh = lv_obj_get_height(m_obj);
                int32_t tx = (pw - ow) / 2 + x_ofs;
                int32_t ty = (ph - oh) / 2 + y_ofs;
                if (lv_obj_get_x(m_obj) == tx && lv_obj_get_y(m_obj) == ty) {
                    return *static_cast<Derived*>(this);
                }
            }
        }
        lv_obj_align(m_obj, alignment, x_ofs, y_ofs);
        return *static_cast<Derived*>(this);
    }

    /// Center in parent
    Derived& center() noexcept {
        lv_obj_center(obj());
        return *static_cast<Derived*>(this);
    }

    // ==================== Flex Layout ====================

#if LV_USE_FLEX
    /// Set flex grow factor (for children of flex containers)
    Derived& grow(uint8_t factor = 1) noexcept {
        lv_obj_set_flex_grow(obj(), factor);
        return *static_cast<Derived*>(this);
    }
#endif

    // ==================== Visibility ====================

    /// Hide the object
    Derived& hide() noexcept {
        lv_obj_add_flag(obj(), LV_OBJ_FLAG_HIDDEN);
        return *static_cast<Derived*>(this);
    }

    /// Show the object
    Derived& show() noexcept {
        lv_obj_remove_flag(obj(), LV_OBJ_FLAG_HIDDEN);
        return *static_cast<Derived*>(this);
    }

    /// Set visibility
    Derived& visible(bool v) noexcept {
        if (v) lv_obj_remove_flag(obj(), LV_OBJ_FLAG_HIDDEN);
        else lv_obj_add_flag(obj(), LV_OBJ_FLAG_HIDDEN);
        return *static_cast<Derived*>(this);
    }
    /// Set visibility ONLY if changed
    Derived& visible_if(bool v) noexcept {
        lv_obj_t* m_obj = obj();
        bool cur = lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        if (v && cur) lv_obj_remove_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        else if (!v && !cur) lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        return *static_cast<Derived*>(this);
    }

    /// Show ONLY if hidden
    Derived& show_if() noexcept {
        lv_obj_t* m_obj = obj();
        if (lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_remove_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        }
        return *static_cast<Derived*>(this);
    }

    /// Hide ONLY if visible
    Derived& hide_if() noexcept {
        lv_obj_t* m_obj = obj();
        if (!lv_obj_has_flag(m_obj, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_add_flag(m_obj, LV_OBJ_FLAG_HIDDEN);
        }
        return *static_cast<Derived*>(this);
    }
    // ==================== Flags ====================

    /// Set clickable flag
    Derived& clickable(bool v = true) noexcept {
        if (v) lv_obj_add_flag(obj(), LV_OBJ_FLAG_CLICKABLE);
        else lv_obj_remove_flag(obj(), LV_OBJ_FLAG_CLICKABLE);
        return *static_cast<Derived*>(this);
    }

    /// Add object flags
    Derived& add_flag(lv_obj_flag_t flag) noexcept {
        lv_obj_add_flag(obj(), flag);
        return *static_cast<Derived*>(this);
    }

    /// Remove object flags
    Derived& remove_flag(lv_obj_flag_t flag) noexcept {
        lv_obj_remove_flag(obj(), flag);
        return *static_cast<Derived*>(this);
    }

    /// Compatibility alias for remove_flag
    Derived& clear_flag(lv_obj_flag_t flag) noexcept {
        return remove_flag(flag);
    }

    /// Query object flags
    [[nodiscard]] bool has_flag(lv_obj_flag_t flag) const noexcept {
        return lv_obj_has_flag(obj(), flag);
    }

    // ==================== State ====================

    /// Add state flags
    Derived& add_state(lv_state_t state) noexcept {
        lv_obj_add_state(obj(), state);
        return *static_cast<Derived*>(this);
    }

    /// Remove state flags
    Derived& remove_state(lv_state_t state) noexcept {
        lv_obj_remove_state(obj(), state);
        return *static_cast<Derived*>(this);
    }

    /// Compatibility alias for clear state
    Derived& clear_state(lv_state_t state) noexcept {
        lv_obj_clear_state(obj(), state);
        return *static_cast<Derived*>(this);
    }

    /// Set state ONLY if it differs (Performance guard)
    Derived& add_state_if(lv_state_t state) noexcept {
        lv_obj_t* m_obj = obj();
        if (!lv_obj_has_state(m_obj, state)) {
            lv_obj_add_state(m_obj, state);
        }
        return *static_cast<Derived*>(this);
    }

    /// Clear state ONLY if it exists (Performance guard)
    Derived& clear_state_if(lv_state_t state) noexcept {
        lv_obj_t* m_obj = obj();
        if (lv_obj_has_state(m_obj, state)) {
            lv_obj_clear_state(m_obj, state);
        }
        return *static_cast<Derived*>(this);
    }

    // ==================== User Data ====================

    /**
     * @brief Set user data pointer
     *
     * @note LVGL 9.x: Safe to use on any object, including component roots.
     *       Components use event-descriptor scanning for ownership lookup
     *       and do not occupy user_data.
     * @note LVGL 8.x: Components use user_data for ownership lookup.
     *       Do NOT use this on component root objects when targeting 8.x.
     */
    Derived& user_data(void* data) noexcept {
        lv_obj_set_user_data(obj(), data);
        return *static_cast<Derived*>(this);
    }

    /// Get user data pointer
    [[nodiscard]] void* user_data() const noexcept {
        return lv_obj_get_user_data(obj());
    }

    /// Get user data as typed pointer
    template<typename T>
    [[nodiscard]] T* user_data_as() const noexcept {
        return static_cast<T*>(lv_obj_get_user_data(obj()));
    }

    /// Get object coordinates
    void get_coords(lv_area_t* area) const noexcept {
        lv_obj_get_coords(obj(), area);
    }

    // ==================== Scrolling ====================

    /// Enable/disable scrolling
    Derived& scrollable(bool v = true) noexcept {
        if (v) lv_obj_add_flag(obj(), LV_OBJ_FLAG_SCROLLABLE);
        else lv_obj_remove_flag(obj(), LV_OBJ_FLAG_SCROLLABLE);
        return *static_cast<Derived*>(this);
    }

    /// Set scroll direction
    Derived& scroll_dir(lv_dir_t dir) noexcept {
        lv_obj_set_scroll_dir(obj(), dir);
        return *static_cast<Derived*>(this);
    }

    /// Set scrollbar mode
    Derived& scrollbar_mode(lv_scrollbar_mode_t mode) noexcept {
        lv_obj_set_scrollbar_mode(obj(), mode);
        return *static_cast<Derived*>(this);
    }

    /// Set horizontal scroll snap
    Derived& scroll_snap_x(lv_scroll_snap_t snap) noexcept {
        lv_obj_set_scroll_snap_x(obj(), snap);
        return *static_cast<Derived*>(this);
    }

    /// Set vertical scroll snap
    Derived& scroll_snap_y(lv_scroll_snap_t snap) noexcept {
        lv_obj_set_scroll_snap_y(obj(), snap);
        return *static_cast<Derived*>(this);
    }

    // ==================== Extended Click Area ====================

    /// Set extended click area (pixels outside object bounds that respond to clicks)
    Derived& ext_click_area(int32_t size) noexcept {
        lv_obj_set_ext_click_area(obj(), size);
        return *static_cast<Derived*>(this);
    }

    // ==================== Flex Layout ====================

#if LV_USE_FLEX
    /// Set flex flow direction
    Derived& flex_flow(lv_flex_flow_t flow) noexcept {
        lv_obj_set_flex_flow(obj(), flow);
        return *static_cast<Derived*>(this);
    }

    /// Set flex alignment (main, cross, and track alignment)
    Derived& flex_align(lv_flex_align_t main, lv_flex_align_t cross, lv_flex_align_t track) noexcept {
        lv_obj_set_flex_align(obj(), main, cross, track);
        return *static_cast<Derived*>(this);
    }
#endif

    // ==================== Layout ====================

    /// Invalidate (request redraw)
    Derived& invalidate() noexcept {
        lv_obj_invalidate(obj());
        return *static_cast<Derived*>(this);
    }

    /// Update layout now
    Derived& update_layout() noexcept {
        lv_obj_update_layout(obj());
        return *static_cast<Derived*>(this);
    }

    /// Set layout type (use kLayout::none, kLayout::flex, kLayout::grid)
    Derived& layout(lv_layout_t l) noexcept {
        lv_obj_set_layout(obj(), l);
        return *static_cast<Derived*>(this);
    }

    /// Disable layout (children use absolute positioning)
    Derived& layout_none() noexcept {
        lv_obj_set_layout(obj(), LV_LAYOUT_NONE);
        return *static_cast<Derived*>(this);
    }

    // ==================== Parent/Child ====================

    /// Move to new parent
    Derived& set_parent(ObjectView new_parent) noexcept {
        lv_obj_set_parent(obj(), new_parent);
        return *static_cast<Derived*>(this);
    }

    // ==================== Style ====================

    /// Add a style to the object (LVGL 8.x compatible - non-const style pointer)
    Derived& add_style(lv_style_t* style, lv_style_selector_t selector = 0) noexcept {
        lv_obj_add_style(obj(), style, selector);
        return *static_cast<Derived*>(this);
    }

    /// Remove a style from the object
    Derived& remove_style(lv_style_t* style, lv_style_selector_t selector = 0) noexcept {
        lv_obj_remove_style(obj(), style, selector);
        return *static_cast<Derived*>(this);
    }

    /// Remove all styles
    Derived& remove_all_styles() noexcept {
        lv_obj_remove_style_all(obj());
        return *static_cast<Derived*>(this);
    }

    /// Compatibility alias for remove_all_styles
    Derived& remove_style_all() noexcept {
        return remove_all_styles();
    }

#if LV_VERSION_AT_LEAST(9, 5, 0)
    /// Remove theme styles from this object (LVGL 9.5+)
    Derived& remove_theme(lv_style_selector_t selector = 0) noexcept {
        lv_obj_remove_theme(obj(), selector);
        return *static_cast<Derived*>(this);
    }
#endif

    // ==================== Radio Button ====================

#if LV_VERSION_AT_LEAST(9, 5, 0)
    /// Enable/disable radio button behavior (LVGL 9.5+)
    /// When enabled, checking this object unchecks siblings in the same parent
    Derived& radio_button(bool en = true) noexcept {
        lv_obj_set_radio_button(obj(), en);
        return *static_cast<Derived*>(this);
    }

    /// Check if radio button behavior is enabled (LVGL 9.5+)
    [[nodiscard]] bool is_radio_button() const noexcept {
        return lv_obj_is_radio_button(obj());
    }
#endif

    // ==================== Style Binding ====================

#if LV_VERSION_AT_LEAST(9, 5, 0) && LV_USE_OBSERVER
    /// Bind a style property to a reactive subject (LVGL 9.5+)
    /// The property updates automatically when the subject value changes
    lv_observer_t* bind_style_prop(lv_style_prop_t prop, lv_style_selector_t selector,
                                   lv_subject_t* subject) noexcept {
        return lv_obj_bind_style_prop(obj(), prop, selector, subject);
    }
#endif

    // ==================== Scroll Operations ====================

    /// Scroll by given offset
    Derived& scroll_by(int32_t x, int32_t y, lv_anim_enable_t anim_en) noexcept {
        lv_obj_scroll_by(obj(), x, y, anim_en);
        return *static_cast<Derived*>(this);
    }

    /// Scroll to given coordinates
    Derived& scroll_to(int32_t x, int32_t y, lv_anim_enable_t anim_en) noexcept {
        lv_obj_scroll_to(obj(), x, y, anim_en);
        return *static_cast<Derived*>(this);
    }

    /// Update scroll snap
    Derived& update_snap(lv_anim_enable_t anim_en) noexcept {
        lv_obj_update_snap(obj(), anim_en);
        return *static_cast<Derived*>(this);
    }

    // ==================== Z-Order ====================

    /// Move object to foreground (on top of siblings)
    Derived& move_foreground() noexcept {
        lv_obj_move_foreground(obj());
        return *static_cast<Derived*>(this);
    }

    /// Move object to background (behind siblings)
    Derived& move_background() noexcept {
        lv_obj_move_background(obj());
        return *static_cast<Derived*>(this);
    }

    // ==================== Alignment ====================

    /// Align to another object
    Derived& align_to(ObjectView base, lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0) noexcept {
        lv_obj_align_to(obj(), base.get(), align, x_ofs, y_ofs);
        return *static_cast<Derived*>(this);
    }

    // ==================== Extended Draw Size ====================

    /// Trigger a refresh of the extended draw size
    Derived& refresh_ext_draw_size() noexcept {
        lv_obj_refresh_ext_draw_size(obj());
        return *static_cast<Derived*>(this);
    }

    // ==================== Object Naming ====================

    /// Set object name (requires LV_USE_OBJ_NAME in lv_conf.h)
    /// Useful for widget identification in UI automation and debugging
    Derived& name([[maybe_unused]] const char* name) noexcept {
        if constexpr (has_obj_name) {
            lv_obj_set_name(obj(), name);
        }
        return *static_cast<Derived*>(this);
    }

    /// Get object name (returns nullptr if not set or LV_USE_OBJ_NAME is disabled)
    [[nodiscard]] const char* get_name() const noexcept {
        if constexpr (has_obj_name) {
            return lv_obj_get_name(obj());
        } else {
            return nullptr;
        }
    }
};

} // namespace lv
