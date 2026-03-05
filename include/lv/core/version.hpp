#pragma once

/**
 * @file version.hpp
 * @brief LVGL version detection for conditional API support
 */

#include <lvgl.h>

// ==================== Compiler-Specific Attributes ====================

/**
 * @brief Empty Base Optimization attribute for MSVC
 *
 * MSVC requires __declspec(empty_bases) to enable EBO with multiple inheritance.
 * Without this, widget classes using CRTP mixins will be larger than sizeof(void*),
 * breaking the zero-overhead static_assert checks.
 */
#ifdef _MSC_VER
    #define LV_EMPTY_BASES __declspec(empty_bases)
#else
    #define LV_EMPTY_BASES
#endif

/// Check if the LVGL version is at least major.minor.patch
#ifndef LV_VERSION_AT_LEAST
#define LV_VERSION_AT_LEAST(major, minor, patch) \
    ((LVGL_VERSION_MAJOR > (major)) || \
     (LVGL_VERSION_MAJOR == (major) && LVGL_VERSION_MINOR > (minor)) || \
     (LVGL_VERSION_MAJOR == (major) && LVGL_VERSION_MINOR == (minor) && LVGL_VERSION_PATCH >= (patch)))
#endif

/**
 * @brief LVGL 8.x Compatibility Layer
 *
 * These macros provide backward compatibility when building with LVGL 8.x.
 * They map LVGL 9.x API names to their LVGL 8.x equivalents.
 */
#if !LV_VERSION_AT_LEAST(9, 0, 0)

/*=============================
 * Object API
 *=============================*/
#define lv_obj_delete                   lv_obj_del
#define lv_obj_delete_async             lv_obj_del_async

/*=============================
 * Display API
 *=============================*/
#define lv_display_t                    lv_disp_t
#define lv_display_rotation_t           lv_disp_rot_t
#define lv_display_get_horizontal_resolution lv_disp_get_hor_res
#define lv_display_get_vertical_resolution   lv_disp_get_ver_res
#define lv_display_get_screen_active    lv_disp_get_scr_act
#define lv_display_set_theme            lv_disp_set_theme
#define lv_display_get_theme            lv_disp_get_theme
#define lv_display_set_rotation         lv_disp_set_rotation
#define lv_display_get_rotation         lv_disp_get_rotation
#define lv_display_get_dpi              lv_disp_get_dpi
#define lv_display_get_layer_top        lv_disp_get_layer_top
#define lv_display_get_layer_sys        lv_disp_get_layer_sys
#define lv_display_get_layer_bottom     lv_disp_get_layer_bottom
#define lv_display_get_default          lv_disp_get_default

/*=============================
 * Color Format API
 *=============================*/
#define lv_color_format_t               lv_img_cf_t
#define LV_COLOR_FORMAT_UNKNOWN         0xFF
#define LV_COLOR_FORMAT_RAW             LV_IMG_CF_UNKNOWN
#define LV_COLOR_FORMAT_RAW_ALPHA       LV_IMG_CF_RAW_ALPHA
#define LV_COLOR_FORMAT_L8              LV_IMG_CF_ALPHA_8BIT
#define LV_COLOR_FORMAT_A8              LV_IMG_CF_ALPHA_8BIT
#define LV_COLOR_FORMAT_I1              LV_IMG_CF_INDEXED_1BIT
#define LV_COLOR_FORMAT_I2              LV_IMG_CF_INDEXED_2BIT
#define LV_COLOR_FORMAT_I4              LV_IMG_CF_INDEXED_4BIT
#define LV_COLOR_FORMAT_I8              LV_IMG_CF_INDEXED_8BIT
#define LV_COLOR_FORMAT_RGB565          LV_IMG_CF_TRUE_COLOR
#define LV_COLOR_FORMAT_RGB888          LV_IMG_CF_TRUE_COLOR_888
#define LV_COLOR_FORMAT_ARGB8888        LV_IMG_CF_TRUE_COLOR_ALPHA
/* XRGB8888: 32-bit format with ignored alpha byte. In LVGL 8.x, use TRUE_COLOR_888
 * (24-bit RGB) as the closest match. If only 16-bit TRUE_COLOR is available,
 * note that color depth reduction will occur. */
#ifdef LV_IMG_CF_TRUE_COLOR_888
    #define LV_COLOR_FORMAT_XRGB8888    LV_IMG_CF_TRUE_COLOR_888
#else
    #define LV_COLOR_FORMAT_XRGB8888    LV_IMG_CF_TRUE_COLOR
#endif

/*=============================
 * Result Type
 *=============================*/
#define lv_result_t                     lv_res_t
#define LV_RESULT_OK                    LV_RES_OK
#define LV_RESULT_INVALID               LV_RES_INV

/*=============================
 * Color Format Functions
 *=============================*/
#define lv_color_format_get_bpp(cf)     lv_img_cf_get_px_size(cf)
#define lv_color_format_has_alpha(cf)   lv_img_cf_has_alpha(cf)

/*=============================
 * Event API
 *=============================*/
#define lv_event_get_target_obj         lv_event_get_target
#define lv_event_get_current_target_obj lv_event_get_current_target

/*=============================
 * Animation API
 *=============================*/
#define lv_anim_set_duration            lv_anim_set_time
#define lv_anim_set_playback_duration   lv_anim_set_playback_time
#define lv_anim_completed_cb_t          lv_anim_ready_cb_t

/*=============================
 * Image API (Widget)
 *=============================*/
#define lv_image_create                 lv_img_create
#define lv_image_set_src                lv_img_set_src
#define lv_image_get_src                lv_img_get_src
#define lv_image_set_rotation           lv_img_set_angle
#define lv_image_get_rotation           lv_img_get_angle
#define lv_image_set_scale              lv_img_set_zoom
#define lv_image_get_scale              lv_img_get_zoom
#define lv_image_set_scale_x            lv_img_set_zoom_x
#define lv_image_set_scale_y            lv_img_set_zoom_y
#define lv_image_get_scale_x            lv_img_get_zoom_x
#define lv_image_get_scale_y            lv_img_get_zoom_y
#define lv_image_set_pivot              lv_img_set_pivot
#define lv_image_set_offset_x           lv_img_set_offset_x
#define lv_image_set_offset_y           lv_img_set_offset_y
#define lv_image_get_offset_x           lv_img_get_offset_x
#define lv_image_get_offset_y           lv_img_get_offset_y
#define lv_image_set_antialias          lv_img_set_antialias
#define lv_image_get_antialias          lv_img_get_antialias
#define lv_image_t                      lv_img_t
#define lv_image_header_t               lv_img_header_t
#define lv_image_dsc_t                  lv_img_dsc_t
#define lv_image_align_t                lv_img_align_t

/*=============================
 * Image Style Properties
 *=============================*/
#define LV_STYLE_IMAGE_RECOLOR          LV_STYLE_IMG_RECOLOR
#define LV_STYLE_IMAGE_RECOLOR_OPA      LV_STYLE_IMG_RECOLOR_OPA

/*=============================
 * Button API (Widget)
 *=============================*/
#define lv_button_create                lv_btn_create
#define lv_button_t                     lv_btn_t

/*=============================
 * Draw API
 *=============================*/
#define lv_draw_image_dsc_t             lv_draw_img_dsc_t
#define lv_draw_image_dsc_init          lv_draw_img_dsc_init
#define lv_obj_init_draw_image_dsc      lv_obj_init_draw_img_dsc
#define lv_draw_image                   lv_draw_img
#define lv_image_src_get_type           lv_img_src_get_type

/*=============================
 * Image Decoder API
 *=============================*/
#define lv_image_decoder_get_info       lv_img_decoder_get_info
#define lv_image_decoder_open           lv_img_decoder_open
#define lv_image_decoder_close          lv_img_decoder_close

/*=============================
 * Layer API
 *=============================*/
#define lv_layer_t                      lv_draw_ctx_t

/*=============================
 * Image Source Type
 *=============================*/
#define lv_image_src_t                  lv_img_src_t

/*=============================
 * Image Source Constants
 *=============================*/
#define LV_IMAGE_SRC_FILE               LV_IMG_SRC_FILE
#define LV_IMAGE_SRC_VARIABLE           LV_IMG_SRC_VARIABLE
#define LV_IMAGE_SRC_SYMBOL             LV_IMG_SRC_SYMBOL
#define LV_IMAGE_SRC_UNKNOWN            LV_IMG_SRC_UNKNOWN

/*=============================
 * Image Alignment Constants
 *=============================*/
#define LV_IMAGE_ALIGN_TOP_LEFT         LV_IMG_ALIGN_TOP_LEFT
#define LV_IMAGE_ALIGN_TOP_MID          LV_IMG_ALIGN_TOP_MID
#define LV_IMAGE_ALIGN_TOP_RIGHT        LV_IMG_ALIGN_TOP_RIGHT
#define LV_IMAGE_ALIGN_BOTTOM_LEFT      LV_IMG_ALIGN_BOTTOM_LEFT
#define LV_IMAGE_ALIGN_BOTTOM_MID       LV_IMG_ALIGN_BOTTOM_MID
#define LV_IMAGE_ALIGN_BOTTOM_RIGHT     LV_IMG_ALIGN_BOTTOM_RIGHT
#define LV_IMAGE_ALIGN_LEFT_MID         LV_IMG_ALIGN_LEFT_MID
#define LV_IMAGE_ALIGN_RIGHT_MID        LV_IMG_ALIGN_RIGHT_MID
#define LV_IMAGE_ALIGN_CENTER           LV_IMG_ALIGN_CENTER
#define LV_IMAGE_ALIGN_STRETCH          LV_IMG_ALIGN_STRETCH
#define LV_IMAGE_ALIGN_TILE             LV_IMG_ALIGN_TILE
#ifndef LV_IMAGE_ALIGN_CONTAIN
#define LV_IMAGE_ALIGN_CONTAIN          11
#endif
#ifndef LV_IMAGE_ALIGN_COVER
#define LV_IMAGE_ALIGN_COVER            12
#endif

/*=============================
 * Scale Constants
 *=============================*/
#define LV_SCALE_NONE                   256

/*=============================
 * Events (LVGL 9.x only)
 *=============================*/
/* These events don't exist in LVGL 8.x - define to a placeholder value */
#ifndef LV_EVENT_SINGLE_CLICKED
#define LV_EVENT_SINGLE_CLICKED         LV_EVENT_LAST
#endif
#ifndef LV_EVENT_DOUBLE_CLICKED
#define LV_EVENT_DOUBLE_CLICKED         LV_EVENT_LAST
#endif
#ifndef LV_EVENT_TRIPLE_CLICKED
#define LV_EVENT_TRIPLE_CLICKED         LV_EVENT_LAST
#endif
#ifndef LV_EVENT_SCROLL_THROW_BEGIN
#define LV_EVENT_SCROLL_THROW_BEGIN     LV_EVENT_LAST
#endif
#ifndef LV_EVENT_ROTARY
#define LV_EVENT_ROTARY                 LV_EVENT_LAST
#endif
#ifndef LV_EVENT_INDEV_RESET
#define LV_EVENT_INDEV_RESET            LV_EVENT_LAST
#endif
#ifndef LV_EVENT_HOVER_OVER
#define LV_EVENT_HOVER_OVER             LV_EVENT_LAST
#endif
#ifndef LV_EVENT_HOVER_LEAVE
#define LV_EVENT_HOVER_LEAVE            LV_EVENT_LAST
#endif
#ifndef LV_EVENT_DRAW_TASK_ADDED
#define LV_EVENT_DRAW_TASK_ADDED        LV_EVENT_LAST
#endif
#ifndef LV_EVENT_CREATE
#define LV_EVENT_CREATE                 LV_EVENT_LAST
#endif
#ifndef LV_EVENT_INVALIDATE_AREA
#define LV_EVENT_INVALIDATE_AREA        LV_EVENT_LAST
#endif
#ifndef LV_EVENT_RESOLUTION_CHANGED
#define LV_EVENT_RESOLUTION_CHANGED     LV_EVENT_LAST
#endif
#ifndef LV_EVENT_COLOR_FORMAT_CHANGED
#define LV_EVENT_COLOR_FORMAT_CHANGED   LV_EVENT_LAST
#endif
#ifndef LV_EVENT_REFR_REQUEST
#define LV_EVENT_REFR_REQUEST           LV_EVENT_LAST
#endif
#ifndef LV_EVENT_REFR_START
#define LV_EVENT_REFR_START             LV_EVENT_LAST
#endif
#ifndef LV_EVENT_REFR_READY
#define LV_EVENT_REFR_READY             LV_EVENT_LAST
#endif
#ifndef LV_EVENT_RENDER_START
#define LV_EVENT_RENDER_START           LV_EVENT_LAST
#endif
#ifndef LV_EVENT_RENDER_READY
#define LV_EVENT_RENDER_READY           LV_EVENT_LAST
#endif
#ifndef LV_EVENT_FLUSH_START
#define LV_EVENT_FLUSH_START            LV_EVENT_LAST
#endif
#ifndef LV_EVENT_FLUSH_FINISH
#define LV_EVENT_FLUSH_FINISH           LV_EVENT_LAST
#endif
#ifndef LV_EVENT_FLUSH_WAIT_START
#define LV_EVENT_FLUSH_WAIT_START       LV_EVENT_LAST
#endif
#ifndef LV_EVENT_FLUSH_WAIT_FINISH
#define LV_EVENT_FLUSH_WAIT_FINISH      LV_EVENT_LAST
#endif
#ifndef LV_EVENT_VSYNC
#define LV_EVENT_VSYNC                  LV_EVENT_LAST
#endif
#ifndef LV_EVENT_VSYNC_REQUEST
#define LV_EVENT_VSYNC_REQUEST          LV_EVENT_LAST
#endif

/*=============================
 * Blend Modes
 *=============================*/
#ifndef LV_BLEND_MODE_DIFFERENCE
#define LV_BLEND_MODE_DIFFERENCE        LV_BLEND_MODE_NORMAL
#endif

/*=============================
 * Layout Types (LVGL 8.x doesn't have lv_layout_t enum)
 *=============================*/
/* LVGL 8.x uses bool for layout enable, not layout type enum */
#define lv_layout_t                     uint8_t
#ifndef LV_LAYOUT_NONE
#define LV_LAYOUT_NONE                  0
#endif
#ifndef LV_LAYOUT_FLEX
#define LV_LAYOUT_FLEX                  1
#endif
#ifndef LV_LAYOUT_GRID
#define LV_LAYOUT_GRID                  2
#endif

/*=============================
 * Color Formats (missing in 8.x)
 *=============================*/
#ifndef LV_COLOR_FORMAT_A1
#define LV_COLOR_FORMAT_A1              LV_IMG_CF_ALPHA_1BIT
#endif
#ifndef LV_COLOR_FORMAT_A2
#define LV_COLOR_FORMAT_A2              LV_IMG_CF_ALPHA_2BIT
#endif
#ifndef LV_COLOR_FORMAT_A4
#define LV_COLOR_FORMAT_A4              LV_IMG_CF_ALPHA_4BIT
#endif
#ifndef LV_COLOR_FORMAT_AL88
#define LV_COLOR_FORMAT_AL88            LV_IMG_CF_ALPHA_8BIT
#endif
#ifndef LV_COLOR_FORMAT_RGB565_SWAPPED
#define LV_COLOR_FORMAT_RGB565_SWAPPED  LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_RGB565A8
#define LV_COLOR_FORMAT_RGB565A8        LV_IMG_CF_TRUE_COLOR_ALPHA
#endif
#ifndef LV_IMG_CF_TRUE_COLOR_888
#define LV_IMG_CF_TRUE_COLOR_888        LV_IMG_CF_TRUE_COLOR_8888
#endif
#ifndef LV_COLOR_FORMAT_ARGB8565
#define LV_COLOR_FORMAT_ARGB8565        LV_IMG_CF_TRUE_COLOR_ALPHA
#endif
#ifndef LV_COLOR_FORMAT_ARGB8888_PREMULTIPLIED
#define LV_COLOR_FORMAT_ARGB8888_PREMULTIPLIED LV_IMG_CF_TRUE_COLOR_ALPHA
#endif
#ifndef LV_COLOR_FORMAT_ARGB1555
#define LV_COLOR_FORMAT_ARGB1555        LV_IMG_CF_TRUE_COLOR_ALPHA
#endif
#ifndef LV_COLOR_FORMAT_ARGB4444
#define LV_COLOR_FORMAT_ARGB4444        LV_IMG_CF_TRUE_COLOR_ALPHA
#endif
#ifndef LV_COLOR_FORMAT_ARGB2222
#define LV_COLOR_FORMAT_ARGB2222        LV_IMG_CF_TRUE_COLOR_ALPHA
#endif
#ifndef LV_COLOR_FORMAT_I420
#define LV_COLOR_FORMAT_I420            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_I422
#define LV_COLOR_FORMAT_I422            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_I444
#define LV_COLOR_FORMAT_I444            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_I400
#define LV_COLOR_FORMAT_I400            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_NV21
#define LV_COLOR_FORMAT_NV21            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_NV12
#define LV_COLOR_FORMAT_NV12            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_YUY2
#define LV_COLOR_FORMAT_YUY2            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_UYVY
#define LV_COLOR_FORMAT_UYVY            LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_NATIVE
#define LV_COLOR_FORMAT_NATIVE          LV_IMG_CF_TRUE_COLOR
#endif
#ifndef LV_COLOR_FORMAT_NATIVE_WITH_ALPHA
#define LV_COLOR_FORMAT_NATIVE_WITH_ALPHA LV_IMG_CF_TRUE_COLOR_ALPHA
#endif

/*=============================
 * Display Rotation Types
 *=============================*/
#ifndef LV_DISPLAY_ROTATION_0
#define LV_DISPLAY_ROTATION_0           LV_DISP_ROT_NONE
#endif
#ifndef LV_DISPLAY_ROTATION_90
#define LV_DISPLAY_ROTATION_90          LV_DISP_ROT_90
#endif
#ifndef LV_DISPLAY_ROTATION_180
#define LV_DISPLAY_ROTATION_180         LV_DISP_ROT_180
#endif
#ifndef LV_DISPLAY_ROTATION_270
#define LV_DISPLAY_ROTATION_270         LV_DISP_ROT_270
#endif

/*=============================
 * Event Descriptor Type
 *=============================*/
#define lv_event_dsc_t                  lv_event_cb_t

/*=============================
 * Object API
 *=============================*/
#define lv_obj_get_child_count(obj)     lv_obj_get_child_cnt(obj)
#define lv_obj_remove_flag              lv_obj_clear_flag
#define lv_obj_remove_state             lv_obj_clear_state

/*=============================
 * Deinit API (LVGL 9.x only)
 *=============================*/
static inline void lv_deinit(void) { }

/*=============================
 * Event API (LVGL 9.x only)
 *=============================*/
static inline uint32_t lv_obj_get_event_count(const lv_obj_t* obj) { (void)obj; return 0; }
static inline lv_event_dsc_t* lv_obj_get_event_dsc(const lv_obj_t* obj, uint32_t i) { (void)obj; (void)i; return nullptr; }
static inline lv_event_cb_t lv_event_dsc_get_cb(lv_event_dsc_t* dsc) { (void)dsc; return nullptr; }
static inline void* lv_event_dsc_get_user_data(lv_event_dsc_t* dsc) { (void)dsc; return nullptr; }

/*=============================
 * Value Types (LVGL 9.x only)
 *=============================*/
#define lv_value_precise_t              int32_t

/*=============================
 * Point Types (LVGL 9.x only)
 *=============================*/
typedef struct {
    int32_t x;
    int32_t y;
} lv_point_precise_t;

/*=============================
 * Animation API (LVGL 9.x only)
 *=============================*/
static inline void lv_anim_delete(lv_obj_t* var, lv_anim_exec_xcb_t exec_cb) { (void)var; (void)exec_cb; }
static inline void lv_anim_pause(lv_anim_t* a) { (void)a; }
static inline void lv_anim_resume(lv_anim_t* a) { (void)a; }
#define lv_anim_set_completed_cb        lv_anim_set_ready_cb

/*=============================
 * Animation Timeline API (LVGL 9.x only)
 *=============================*/
#define lv_anim_timeline_delete         lv_anim_timeline_del
static inline void lv_anim_timeline_set_delay(lv_anim_timeline_t* tl, uint32_t delay) { (void)tl; (void)delay; }
static inline void lv_anim_timeline_set_repeat_count(lv_anim_timeline_t* tl, uint32_t count) { (void)tl; (void)count; }
static inline void lv_anim_timeline_pause(lv_anim_timeline_t* tl) { (void)tl; }

/*=============================
 * Screen Animation (LVGL 9.x only)
 *=============================*/
typedef uint8_t lv_screen_load_anim_t;
#define LV_SCREEN_LOAD_ANIM_NONE        0
#define LV_SCREEN_LOAD_ANIM_FADE        1
#define LV_SCREEN_LOAD_ANIM_MOVE_LEFT   2
#define LV_SCREEN_LOAD_ANIM_MOVE_RIGHT  3
#define LV_SCREEN_LOAD_ANIM_MOVE_TOP    4
#define LV_SCREEN_LOAD_ANIM_MOVE_BOTTOM 5
static inline void lv_screen_load_anim(lv_obj_t* scr, lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool auto_del) {
    (void)anim_type; (void)time; (void)delay; (void)auto_del;
    lv_scr_load(scr);
}

/*=============================
 * Style API (LVGL 9.x only / 8.x mapping)
 * Map 9.x style property names to 8.x equivalents where possible
 * Use no-op stubs only for truly new features
 *=============================*/
// Shadow offset - mapped to 8.x equivalents
#define lv_style_set_shadow_offset_x    lv_style_set_shadow_ofs_x
#define lv_style_set_shadow_offset_y    lv_style_set_shadow_ofs_y

// Margin - no-op stubs (margin not available in LVGL 8.x style API)
static inline void lv_style_set_margin_top(lv_style_t* style, lv_coord_t v) { (void)style; (void)v; }
static inline void lv_style_set_margin_bottom(lv_style_t* style, lv_coord_t v) { (void)style; (void)v; }
static inline void lv_style_set_margin_left(lv_style_t* style, lv_coord_t v) { (void)style; (void)v; }
static inline void lv_style_set_margin_right(lv_style_t* style, lv_coord_t v) { (void)style; (void)v; }

// Image opa/recolor - mapped to 8.x equivalents
#define lv_style_set_image_opa          lv_style_set_img_opa
#define lv_style_set_image_recolor      lv_style_set_img_recolor
#define lv_style_set_image_recolor_opa  lv_style_set_img_recolor_opa

// Transform - no-op stubs (transform properties in 8.x work differently)
static inline void lv_style_set_transform_scale(lv_style_t* style, int32_t v) { (void)style; (void)v; }
static inline void lv_style_set_transform_scale_x(lv_style_t* style, int32_t v) { (void)style; (void)v; }
static inline void lv_style_set_transform_scale_y(lv_style_t* style, int32_t v) { (void)style; (void)v; }
static inline void lv_style_set_transform_rotation(lv_style_t* style, int32_t v) { (void)style; (void)v; }

/*=============================
 * Missing Constants
 *=============================*/
#ifndef LV_EVENT_LAST
#define LV_EVENT_LAST                   _LV_EVENT_LAST
#endif
#ifndef LV_IMG_CF_TRUE_COLOR_8888
#define LV_IMG_CF_TRUE_COLOR_8888       LV_IMG_CF_TRUE_COLOR
#endif

/*=============================
 * Style API Stubs (LVGL 9.x only features)
 *=============================*/
static inline void lv_obj_set_style_image_opa(lv_obj_t* obj, lv_opa_t v, lv_style_selector_t s) { (void)obj; (void)v; (void)s; }
static inline void lv_obj_set_style_transform_rotation(lv_obj_t* obj, int32_t v, lv_style_selector_t s) { (void)obj; (void)v; (void)s; }
static inline void lv_obj_set_style_transform_scale(lv_obj_t* obj, int32_t v, lv_style_selector_t s) { (void)obj; (void)v; (void)s; }

/*=============================
 * Animation Timeline Stubs (LVGL 9.x only)
 *=============================*/
static inline void lv_anim_timeline_set_repeat_delay(lv_anim_timeline_t* tl, uint32_t delay) { (void)tl; (void)delay; }
static inline void lv_anim_timeline_set_user_data(lv_anim_timeline_t* tl, void* data) { (void)tl; (void)data; }
static inline void lv_anim_timeline_merge(lv_anim_timeline_t* tl, lv_anim_timeline_t* other, int32_t delay_ms) { (void)tl; (void)other; (void)delay_ms; }
static inline uint16_t lv_anim_timeline_get_progress(lv_anim_timeline_t* tl) { (void)tl; return 0; }
static inline uint32_t lv_anim_timeline_get_delay(lv_anim_timeline_t* tl) { (void)tl; return 0; }
static inline uint32_t lv_anim_timeline_get_repeat_count(lv_anim_timeline_t* tl) { (void)tl; return 0; }
static inline uint32_t lv_anim_timeline_get_repeat_delay(lv_anim_timeline_t* tl) { (void)tl; return 0; }
static inline void* lv_anim_timeline_get_user_data(lv_anim_timeline_t* tl) { (void)tl; return nullptr; }

/*=============================
 * Theme API (LVGL 9.x only)
 *=============================*/
static inline void lv_theme_default_deinit() { }
static inline lv_theme_t* lv_theme_mono_get() { return nullptr; }
static inline void lv_theme_mono_deinit() { }

/*=============================
 * Screen API
 *=============================*/
#define lv_screen_load                  lv_scr_load
#define lv_screen_active                lv_scr_act

/*=============================
 * Input Device API (LVGL 9.x only)
 *=============================*/
typedef void (*lv_indev_read_cb_t)(lv_indev_t*, lv_indev_data_t*);
static inline lv_indev_t* lv_indev_create() { return nullptr; }
static inline void lv_indev_set_type(lv_indev_t* indev, lv_indev_type_t type) { (void)indev; (void)type; }
static inline void lv_indev_set_read_cb(lv_indev_t* indev, lv_indev_read_cb_t cb) { (void)indev; (void)cb; }
static inline void lv_indev_set_user_data(lv_indev_t* indev, void* data) { (void)indev; (void)data; }
static inline void* lv_indev_get_user_data(lv_indev_t* indev) { (void)indev; return nullptr; }
static inline void lv_indev_set_display(lv_indev_t* indev, lv_disp_t* disp) { (void)indev; (void)disp; }
static inline lv_indev_t* lv_indev_active() { return nullptr; }

/*=============================
 * Object Type Checking
 *=============================*/
#define lv_obj_check_type(obj, class_p) (lv_obj_get_class(obj) == (class_p))

/*=============================
 * Event Removal API
 *=============================*/
/* lv_obj_remove_event_cb_with_user_data exists in LVGL 8.4+ */

/*=============================
 * Theme API
 *=============================*/
// Note: lv_theme_get_from_obj is the 8.x name, 9.x uses lv_theme_get_theme_from_obj
// Since we use lv_theme_get_from_obj in the code, we don't need a mapping for 8.x

/*=============================
 * Group API
 *=============================*/
#define lv_group_delete                 lv_group_del

/*=============================
 * Text API
 *=============================*/
#define lv_text_get_size                lv_txt_get_size

/*=============================
 * Style API - Background Image
 *=============================*/
#define lv_obj_set_style_bg_image_src   lv_obj_set_style_bg_img_src
#define lv_style_set_bg_image_src       lv_style_set_bg_img_src

/*=============================
 * Buttonmatrix API
 *=============================*/
#define lv_buttonmatrix_ctrl_t          lv_btnmatrix_ctrl_t

/*=============================
 * Timer API
 *=============================*/
#define lv_timer_delete                 lv_timer_del
static inline void* lv_timer_get_user_data(lv_timer_t* timer) { (void)timer; return nullptr; }
static inline void lv_timer_set_user_data(lv_timer_t* timer, void* data) { (void)timer; (void)data; }
static inline bool lv_timer_get_paused(const lv_timer_t* timer) { (void)timer; return false; }

/*=============================
 * Gradient API (LVGL 9.x only)
 *=============================*/
typedef uint8_t lv_grad_extend_t;
#define LV_GRAD_EXTEND_PAD              0
#define LV_GRAD_EXTEND_REPEAT           1
#define LV_GRAD_EXTEND_REFLECT          2
static inline void lv_grad_linear_init(lv_grad_dsc_t* dsc, int32_t from_x, int32_t from_y, int32_t to_x, int32_t to_y, lv_grad_extend_t extend) {
    (void)dsc; (void)from_x; (void)from_y; (void)to_x; (void)to_y; (void)extend;
}
static inline void lv_grad_radial_init(lv_grad_dsc_t* dsc, int32_t center_x, int32_t center_y, int32_t to_x, int32_t to_y, lv_grad_extend_t extend) {
    (void)dsc; (void)center_x; (void)center_y; (void)to_x; (void)to_y; (void)extend;
}
static inline void lv_grad_radial_set_focal(lv_grad_dsc_t* dsc, int32_t center_x, int32_t center_y, int32_t radius) {
    (void)dsc; (void)center_x; (void)center_y; (void)radius;
}
static inline void lv_grad_conical_init(lv_grad_dsc_t* dsc, int32_t center_x, int32_t center_y, int32_t start_angle, int32_t end_angle, lv_grad_extend_t extend) {
    (void)dsc; (void)center_x; (void)center_y; (void)start_angle; (void)end_angle; (void)extend;
}

#endif /* !LV_VERSION_AT_LEAST(9, 0, 0) */
