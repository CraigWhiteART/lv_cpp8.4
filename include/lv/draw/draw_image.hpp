#pragma once

/**
 * @file draw_image.hpp
 * @brief Wrapper for LVGL image drawing
 */

#include <lvgl.h>
#include "../core/version.hpp"
#include "layer.hpp"
#include "primitives.hpp"

namespace lv {

/**
 * @brief Fluent wrapper for lv_draw_image_dsc_t / lv_draw_img_dsc_t
 *
 * Example:
 * ```cpp
 * lv::ImageDsc dsc;
 * dsc.src(&my_image)
 *    .rotation(450)  // 45 degrees
 *    .scale(128)     // 50% size
 *    .recolor(lv::color::red(), LV_OPA_50);
 * lv::draw::image(layer, dsc, lv::area(10, 10, 110, 110));
 * ```
 */
class ImageDsc {
    lv_draw_image_dsc_t m_dsc{};
#if !LV_VERSION_AT_LEAST(9, 0, 0)
    const void* m_src = nullptr; ///< Stored separately for LVGL 8.x
#endif

public:
    ImageDsc() noexcept {
        lv_draw_image_dsc_init(&m_dsc);
    }

    /// Initialize from an object's styles
    ImageDsc(lv_obj_t* obj, lv_part_t part) noexcept {
        lv_draw_image_dsc_init(&m_dsc);
        lv_obj_init_draw_image_dsc(obj, part, &m_dsc);
    }

    /// Create from object's styles
    [[nodiscard]] static ImageDsc from_obj(lv_obj_t* obj, lv_part_t part = LV_PART_MAIN) noexcept {
        return ImageDsc(obj, part);
    }

    [[nodiscard]] lv_draw_image_dsc_t* get() noexcept { return &m_dsc; }
    [[nodiscard]] const lv_draw_image_dsc_t* get() const noexcept { return &m_dsc; }
    operator const lv_draw_image_dsc_t*() const noexcept { return &m_dsc; }

    // ==================== Source ====================

    /// Set image source (lv_image_dsc_t*, file path, or symbol)
    ImageDsc& src(const void* s) noexcept {
#if LV_VERSION_AT_LEAST(9, 0, 0)
        m_dsc.src = s;
#else
        m_src = s;
#endif
        return *this;
    }

#if !LV_VERSION_AT_LEAST(9, 0, 0)
    /// Get stored source (for LVGL 8.x)
    [[nodiscard]] const void* get_src() const noexcept { return m_src; }
#endif

    // ==================== Transform ====================

    /// Set rotation in 0.1 degree units (e.g., 450 = 45°)
    ImageDsc& rotation(int32_t r) noexcept {
#if LV_VERSION_AT_LEAST(9, 0, 0)
        m_dsc.rotation = r;
#else
        m_dsc.angle = static_cast<int16_t>(r);
#endif
        return *this;
    }

    /// Set scale (256 = 100%, 512 = 200%, 128 = 50%)
    /// @note LVGL 8.x only supports uniform zoom via scale(s)
    ImageDsc& scale(int32_t x, int32_t y) noexcept {
#if LV_VERSION_AT_LEAST(9, 0, 0)
        m_dsc.scale_x = x;
        m_dsc.scale_y = y;
#else
        m_dsc.zoom = static_cast<uint16_t>(x);
        (void)y; // Unused in 8.x
#endif
        return *this;
    }

    /// Set uniform scale
    ImageDsc& scale(int32_t s) noexcept {
#if LV_VERSION_AT_LEAST(9, 0, 0)
        m_dsc.scale_x = s;
        m_dsc.scale_y = s;
#else
        m_dsc.zoom = static_cast<uint16_t>(s);
#endif
        return *this;
    }

#if LV_VERSION_AT_LEAST(9, 0, 0)
    /// Set skew in 0.1 degree units (LVGL 9.x only)
    ImageDsc& skew(int32_t x, int32_t y) noexcept {
        m_dsc.skew_x = x;
        m_dsc.skew_y = y;
        return *this;
    }
#endif

    /// Set transform pivot point
    ImageDsc& pivot(int32_t x, int32_t y) noexcept {
        m_dsc.pivot.x = x;
        m_dsc.pivot.y = y;
        return *this;
    }

    // ==================== Appearance ====================

    /// Set opacity
    ImageDsc& opa(lv_opa_t o) noexcept {
        m_dsc.opa = o;
        return *this;
    }

    /// Set recolor
    ImageDsc& recolor(lv_color_t c, lv_opa_t o = LV_OPA_COVER) noexcept {
        m_dsc.recolor = c;
        m_dsc.recolor_opa = o;
        return *this;
    }

    /// Set blend mode
    ImageDsc& blend_mode(lv_blend_mode_t m) noexcept {
        m_dsc.blend_mode = m;
        return *this;
    }

#if LV_VERSION_AT_LEAST(9, 0, 0)
    /// Set clip radius (LVGL 9.x only)
    ImageDsc& clip_radius(int32_t r) noexcept {
        m_dsc.clip_radius = r;
        return *this;
    }
#endif

    // ==================== Options ====================

    /// Enable anti-aliasing for transforms
    ImageDsc& antialias(bool aa = true) noexcept {
#if LV_VERSION_AT_LEAST(9, 0, 0)
        m_dsc.antialias = aa ? 1 : 0;
#else
        m_dsc.antialias = aa;
#endif
        return *this;
    }

#if LV_VERSION_AT_LEAST(9, 0, 0)
    /// Enable tiling (LVGL 9.x only)
    ImageDsc& tile(bool t = true) noexcept {
        m_dsc.tile = t ? 1 : 0;
        return *this;
    }

    /// Set bitmap mask (LVGL 9.x only)
    ImageDsc& bitmap_mask(const lv_image_dsc_t* mask) noexcept {
        m_dsc.bitmap_mask_src = mask;
        return *this;
    }

    /// Set the image area (for tiling or partial rendering) (LVGL 9.x only)
    ImageDsc& image_area(const lv_area_t& area) noexcept {
        m_dsc.image_area = area;
        return *this;
    }
#endif
};

namespace draw {

/// Draw an image
inline void image(lv_layer_t* layer, const ImageDsc& dsc, const lv_area_t& coords) noexcept {
#if LV_VERSION_AT_LEAST(9, 0, 0)
    lv_draw_image(layer, dsc.get(), &coords);
#else
    lv_draw_img(layer, dsc.get(), &coords, dsc.get_src());
#endif
}

/// Convenience: draw a simple image
inline void image(lv_layer_t* layer, const void* src, const lv_area_t& coords) noexcept {
    ImageDsc dsc;
    dsc.src(src);
#if LV_VERSION_AT_LEAST(9, 0, 0)
    lv_draw_image(layer, dsc.get(), &coords);
#else
    lv_draw_img(layer, dsc.get(), &coords, src);
#endif
}

#if LV_VERSION_AT_LEAST(9, 0, 0)
/// Draw a layer as an image (LVGL 9.x only)
inline void layer_image(lv_layer_t* layer, const ImageDsc& dsc, const lv_area_t& coords) noexcept {
    lv_draw_layer(layer, dsc.get(), &coords);
}
#endif

/// Get the type of an image source
[[nodiscard]] inline lv_image_src_t image_src_type(const void* src) noexcept {
    return lv_image_src_get_type(src);
}

} // namespace draw

} // namespace lv
