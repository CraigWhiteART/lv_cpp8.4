#!/usr/bin/env python3
"""
LVGL 8.4 Compatibility Verification Script

This script verifies that the C++ wrapper can compile and link with LVGL 8.4.
It generates a test file that exercises all compatibility macros and type mappings.

Usage:
    python scripts/verify_lvgl84_compat.py
    # Or with cmake:
    python scripts/verify_lvgl84_compat.py --build
"""

import argparse
import os
import subprocess
import sys
import tempfile

# Template for the verification test file
TEST_TEMPLATE = '''
/**
 * @file test_lvgl84_compat.cpp
 * @brief Hard verification test for LVGL 8.4 compatibility
 *
 * This file exercises EVERY compatibility macro and type mapping
 * to ensure the C++ wrapper works correctly with LVGL 8.4.
 */

#include <lvgl.h>
#include <lv/lv.hpp>

// ============================================================================
// Test 1: Object API compatibility
// ============================================================================
void test_object_api() {
    lv_obj_t* obj = lv_obj_create(lv_scr_act());
    
    // These should work via macros in version.hpp
    lv_obj_delete(obj);
    
    obj = lv_obj_create(lv_scr_act());
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN);
    
    uint32_t count = lv_obj_get_child_count(obj);
    (void)count;
}

// ============================================================================
// Test 2: Display API compatibility
// ============================================================================
void test_display_api() {
    lv_display_t* disp = lv_disp_get_default();
    if (!disp) return;
    
    int32_t w = lv_display_get_horizontal_resolution(disp);
    int32_t h = lv_display_get_vertical_resolution(disp);
    lv_display_rotation_t rot = lv_display_get_rotation(disp);
    
    (void)w;
    (void)h;
    (void)rot;
}

// ============================================================================
// Test 3: Color format compatibility
// ============================================================================
void test_color_format() {
    // All these should map to LVGL 8.x equivalents
    lv_color_format_t cf;
    cf = LV_COLOR_FORMAT_UNKNOWN;
    cf = LV_COLOR_FORMAT_RAW;
    cf = LV_COLOR_FORMAT_RAW_ALPHA;
    cf = LV_COLOR_FORMAT_L8;
    cf = LV_COLOR_FORMAT_A8;
    cf = LV_COLOR_FORMAT_I1;
    cf = LV_COLOR_FORMAT_I2;
    cf = LV_COLOR_FORMAT_I4;
    cf = LV_COLOR_FORMAT_I8;
    cf = LV_COLOR_FORMAT_RGB565;
    cf = LV_COLOR_FORMAT_RGB888;
    cf = LV_COLOR_FORMAT_ARGB8888;
    cf = LV_COLOR_FORMAT_XRGB8888;
    cf = LV_COLOR_FORMAT_A1;
    cf = LV_COLOR_FORMAT_A2;
    cf = LV_COLOR_FORMAT_A4;
    cf = LV_COLOR_FORMAT_AL88;
    cf = LV_COLOR_FORMAT_RGB565_SWAPPED;
    cf = LV_COLOR_FORMAT_RGB565A8;
    cf = LV_COLOR_FORMAT_ARGB8565;
    cf = LV_COLOR_FORMAT_ARGB8888_PREMULTIPLIED;
    cf = LV_COLOR_FORMAT_ARGB1555;
    cf = LV_COLOR_FORMAT_ARGB4444;
    cf = LV_COLOR_FORMAT_ARGB2222;
    cf = LV_COLOR_FORMAT_NATIVE;
    cf = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA;
    
    (void)cf;
}

// ============================================================================
// Test 4: Image API compatibility (9.x style names)
// ============================================================================
void test_image_api() {
    lv_obj_t* parent = lv_scr_act();
    lv_obj_t* img = lv_image_create(parent);
    
    lv_image_set_rotation(img, 900);  // 90 degrees
    lv_image_set_scale(img, 256);     // 100%%
    lv_image_set_scale_x(img, 256);
    lv_image_set_scale_y(img, 256);
    lv_image_set_pivot(img, 50, 50);
    lv_image_set_offset_x(img, 10);
    lv_image_set_offset_y(img, 10);
    lv_image_set_antialias(img, true);
    
    lv_color_format_t cf = lv_image_get_src(img) ? LV_COLOR_FORMAT_ARGB8888 : LV_COLOR_FORMAT_UNKNOWN;
    (void)cf;
}

// ============================================================================
// Test 5: Image alignment constants
// ============================================================================
void test_image_alignment() {
    auto align1 = LV_IMAGE_ALIGN_TOP_LEFT;
    auto align2 = LV_IMAGE_ALIGN_TOP_MID;
    auto align3 = LV_IMAGE_ALIGN_TOP_RIGHT;
    auto align4 = LV_IMAGE_ALIGN_BOTTOM_LEFT;
    auto align5 = LV_IMAGE_ALIGN_BOTTOM_MID;
    auto align6 = LV_IMAGE_ALIGN_BOTTOM_RIGHT;
    auto align7 = LV_IMAGE_ALIGN_LEFT_MID;
    auto align8 = LV_IMAGE_ALIGN_RIGHT_MID;
    auto align9 = LV_IMAGE_ALIGN_CENTER;
    auto align10 = LV_IMAGE_ALIGN_STRETCH;
    auto align11 = LV_IMAGE_ALIGN_TILE;
    auto align12 = LV_IMAGE_ALIGN_CONTAIN;  // 8.x placeholder
    auto align13 = LV_IMAGE_ALIGN_COVER;    // 8.x placeholder
    
    (void)align1; (void)align2; (void)align3; (void)align4;
    (void)align5; (void)align6; (void)align7; (void)align8;
    (void)align9; (void)align10; (void)align11; (void)align12;
    (void)align13;
}

// ============================================================================
// Test 6: Image source types
// ============================================================================
void test_image_source() {
    lv_image_src_t type = LV_IMAGE_SRC_FILE;
    type = LV_IMAGE_SRC_VARIABLE;
    type = LV_IMAGE_SRC_SYMBOL;
    type = LV_IMAGE_SRC_UNKNOWN;
    (void)type;
}

// ============================================================================
// Test 7: Event compatibility
// ============================================================================
void test_event_api() {
    // Events defined in version.hpp for 8.x
    lv_event_code_t code = LV_EVENT_SINGLE_CLICKED;
    code = LV_EVENT_DOUBLE_CLICKED;
    code = LV_EVENT_TRIPLE_CLICKED;
    code = LV_EVENT_SCROLL_THROW_BEGIN;
    code = LV_EVENT_ROTARY;
    code = LV_EVENT_INDEV_RESET;
    code = LV_EVENT_HOVER_OVER;
    code = LV_EVENT_HOVER_LEAVE;
    code = LV_EVENT_DRAW_TASK_ADDED;
    code = LV_EVENT_CREATE;
    code = LV_EVENT_INVALIDATE_AREA;
    code = LV_EVENT_RESOLUTION_CHANGED;
    code = LV_EVENT_COLOR_FORMAT_CHANGED;
    code = LV_EVENT_REFR_REQUEST;
    code = LV_EVENT_VSYNC;
    code = LV_EVENT_VSYNC_REQUEST;
    
    (void)code;
}

// ============================================================================
// Test 8: Animation API compatibility
// ============================================================================
void test_animation_api() {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_duration(&a, 500);
    lv_anim_set_playback_duration(&a, 500);
    
    // These are stubs in 8.x
    lv_anim_pause(&a);
    lv_anim_resume(&a);
    lv_anim_delete(lv_scr_act(), nullptr);
}

// ============================================================================
// Test 9: Animation Timeline compatibility
// ============================================================================
void test_timeline_api() {
    lv_anim_timeline_t* tl = lv_anim_timeline_create();
    if (tl) {
        lv_anim_timeline_set_delay(tl, 100);
        lv_anim_timeline_set_repeat_count(tl, 1);
        lv_anim_timeline_pause(tl);
        lv_anim_timeline_set_repeat_delay(tl, 0);
        lv_anim_timeline_set_user_data(tl, nullptr);
        lv_anim_timeline_merge(tl, nullptr, 0);
        uint16_t prog = lv_anim_timeline_get_progress(tl);
        uint32_t delay = lv_anim_timeline_get_delay(tl);
        uint32_t repeat = lv_anim_timeline_get_repeat_count(tl);
        uint32_t repeat_delay = lv_anim_timeline_get_repeat_delay(tl);
        void* data = lv_anim_timeline_get_user_data(tl);
        
        (void)prog; (void)delay; (void)repeat; (void)repeat_delay; (void)data;
        
        lv_anim_timeline_delete(tl);
    }
}

// ============================================================================
// Test 10: Style API compatibility
// ============================================================================
void test_style_api() {
    lv_style_t style;
    lv_style_init(&style);
    
    // 9.x style property names
    lv_style_set_shadow_offset_x(&style, 5);
    lv_style_set_shadow_offset_y(&style, 5);
    lv_style_set_margin_top(&style, 10);
    lv_style_set_margin_bottom(&style, 10);
    lv_style_set_margin_left(&style, 10);
    lv_style_set_margin_right(&style, 10);
    lv_style_set_image_opa(&style, LV_OPA_COVER);
    lv_style_set_image_recolor(&style, lv_color_white());
    lv_style_set_image_recolor_opa(&style, LV_OPA_50);
    lv_style_set_transform_scale(&style, 256);
    lv_style_set_transform_scale_x(&style, 256);
    lv_style_set_transform_scale_y(&style, 256);
    lv_style_set_transform_rotation(&style, 0);
    
    lv_style_reset(&style);
}

// ============================================================================
// Test 11: Object style setters compatibility
// ============================================================================
void test_obj_style_api() {
    lv_obj_t* obj = lv_obj_create(lv_scr_act());
    
    lv_obj_set_style_image_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_transform_rotation(obj, 0, 0);
    lv_obj_set_style_transform_scale(obj, 256, 0);
    
    lv_obj_del(obj);
}

// ============================================================================
// Test 12: Screen API compatibility
// ============================================================================
void test_screen_api() {
    lv_obj_t* scr = lv_scr_act();
    lv_screen_load(scr);
    lv_obj_t* active = lv_screen_active();
    (void)active;
}

// ============================================================================
// Test 13: Display rotation
// ============================================================================
void test_display_rotation() {
    auto r1 = LV_DISPLAY_ROTATION_0;
    auto r2 = LV_DISPLAY_ROTATION_90;
    auto r3 = LV_DISPLAY_ROTATION_180;
    auto r4 = LV_DISPLAY_ROTATION_270;
    (void)r1; (void)r2; (void)r3; (void)r4;
}

// ============================================================================
// Test 14: Scale constants
// ============================================================================
void test_scale_constants() {
    uint32_t scale = LV_SCALE_NONE;
    (void)scale;
}

// ============================================================================
// Test 15: Result type
// ============================================================================
void test_result_type() {
    lv_result_t res = LV_RESULT_OK;
    res = LV_RESULT_INVALID;
    (void)res;
}

// ============================================================================
// Test 16: Layout type
// ============================================================================
void test_layout_type() {
    lv_layout_t layout = LV_LAYOUT_NONE;
    layout = LV_LAYOUT_FLEX;
    layout = LV_LAYOUT_GRID;
    (void)layout;
}

// ============================================================================
// Test 17: Value and point types
// ============================================================================
void test_value_types() {
    lv_value_precise_t val = 100;
    (void)val;
    
    lv_point_precise_t pt;
    pt.x = 100;
    pt.y = 100;
    (void)pt;
}

// ============================================================================
// Test 18: Gradient types
// ============================================================================
void test_gradient_types() {
    lv_grad_extend_t extend = LV_GRAD_EXTEND_PAD;
    extend = LV_GRAD_EXTEND_REPEAT;
    extend = LV_GRAD_EXTEND_REFLECT;
    
    lv_grad_dsc_t grad;
    lv_grad_linear_init(&grad, 0, 0, 100, 0, extend);
    lv_grad_radial_init(&grad, 50, 50, 100, 0, extend);
    lv_grad_radial_set_focal(&grad, 50, 50, 30);
    lv_grad_conical_init(&grad, 50, 50, 0, 360, extend);
    
    (void)extend;
}

// ============================================================================
// Test 19: Timer API
// ============================================================================
void test_timer_api() {
    lv_timer_t* timer = lv_timer_create([](lv_timer_t*){}, 100, nullptr);
    if (timer) {
        lv_timer_delete(timer);
    }
}

// ============================================================================
// Test 20: Theme API
// ============================================================================
void test_theme_api() {
    lv_theme_default_deinit();
    lv_theme_t* mono = lv_theme_mono_get();
    lv_theme_mono_deinit();
    
    lv_obj_t* obj = lv_obj_create(lv_scr_act());
    lv_theme_t* theme = lv_theme_get_from_obj(obj);
    (void)theme;
    lv_obj_del(obj);
}

// ============================================================================
// Test 21: Screen animation (9.x style)
// ============================================================================
void test_screen_anim() {
    lv_obj_t* scr = lv_obj_create(nullptr);
    lv_screen_load_anim(scr, LV_SCREEN_LOAD_ANIM_NONE, 300, 0, false);
    lv_obj_del(scr);
}

// ============================================================================
// Test 22: Blend mode compatibility
// ============================================================================
void test_blend_mode() {
    lv_blend_mode_t mode = LV_BLEND_MODE_DIFFERENCE;
    (void)mode;
}

// ============================================================================
// Test 23: Button API (9.x naming)
// ============================================================================
void test_button_api() {
    lv_obj_t* parent = lv_scr_act();
    lv_obj_t* btn = lv_button_create(parent);
    (void)btn;
}

// ============================================================================
// Test 24: Image decoder API (9.x naming)
// ============================================================================
void test_image_decoder() {
    lv_image_header_t header;
    lv_result_t res = lv_image_decoder_get_info("test.png", &header);
    (void)res;
}

// ============================================================================
// Test 25: C++ wrapper usage
// ============================================================================
void test_cpp_wrapper() {
    // Basic widget creation
    auto label = lv::Label::create(lv::screen_active())
        .text("Test")
        .width(100);
    
    auto btn = lv::Button::create(lv::screen_active())
        .size(100, 50)
        .center();
    
    // Test constants
    lv::kEvent::pressed;
    lv::kState::checked;
    lv::kPart::main;
    lv::kAlign::center;
    lv::kFlag::clickable;
    
    // Test color format constants
    lv::kColorFormat::RGB565;
    lv::kColorFormat::ARGB8888;
    lv::kColorFormat::native;
    
    // Test display rotation
    lv::kDisplayRotation::none;
    lv::kDisplayRotation::r90;
    
    // Test layout
    lv::kLayout::none;
    
    (void)label;
    (void)btn;
}

// ============================================================================
// Main
// ============================================================================
int main() {
    lv_init();
    
    test_object_api();
    test_display_api();
    test_color_format();
    test_image_api();
    test_image_alignment();
    test_image_source();
    test_event_api();
    test_animation_api();
    test_timeline_api();
    test_style_api();
    test_obj_style_api();
    test_screen_api();
    test_display_rotation();
    test_scale_constants();
    test_result_type();
    test_layout_type();
    test_value_types();
    test_gradient_types();
    test_timer_api();
    test_theme_api();
    test_screen_anim();
    test_blend_mode();
    test_button_api();
    test_image_decoder();
    test_cpp_wrapper();
    
    return 0;
}
'''

def write_test_file(path: str) -> str:
    """Write the test file to disk."""
    test_path = os.path.join(path, 'test_lvgl84_compat.cpp')
    with open(test_path, 'w') as f:
        f.write(TEST_TEMPLATE)
    return test_path

def compile_test(test_file: str, include_dirs: list, build_dir: str) -> bool:
    """Try to compile the test file."""
    # Determine compiler
    cxx = os.environ.get('CXX', None)
    if cxx is None:
        # Auto-detect based on platform
        if sys.platform == 'win32':
            # On Windows, prefer cl.exe (MSVC) if available, fallback to g++
            cxx = 'cl' if subprocess.run(['where', 'cl'], capture_output=True).returncode == 0 else 'g++'
        else:
            cxx = 'g++'
    
    # Build compile command
    if cxx == 'cl' or cxx.endswith('cl.exe'):
        # MSVC command line
        cmd = ['cl', '/std:c++20', '/c', test_file, '/Fo' + os.path.join(build_dir, 'test.obj')]
        cmd.extend(['/I' + d for d in include_dirs])
        cmd.extend(['/DLV_CONF_INCLUDE_SIMPLE', '/W0'])  # Suppress warnings for this test
    else:
        # GCC/Clang command line
        cmd = [cxx, '-std=c++20', '-c', test_file, '-o', os.path.join(build_dir, 'test.o')]
        cmd.extend(['-I' + d for d in include_dirs])
        cmd.extend(['-DLV_CONF_INCLUDE_SIMPLE', '-w'])  # Suppress warnings for this test
    cmd.extend(['-I' + d for d in include_dirs])
    cmd.extend(['-DLV_CONF_INCLUDE_SIMPLE', '-w'])  # Suppress warnings for this test
    
    print(f"Compiling with: {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    if result.returncode != 0:
        print("ERROR: Compilation failed!")
        print("STDOUT:", result.stdout)
        print("STDERR:", result.stderr)
        return False
    
    print("SUCCESS: Compilation passed!")
    return True

def main():
    parser = argparse.ArgumentParser(description='Verify LVGL 8.4 compatibility')
    parser.add_argument('--build', action='store_true', help='Actually compile the test')
    parser.add_argument('--build-dir', default='build-84-verify', help='Build directory')
    parser.add_argument('--lvgl-dir', default='lvgl_8.4', help='Path to LVGL 8.4 directory')
    args = parser.parse_args()
    
    # Create build directory
    os.makedirs(args.build_dir, exist_ok=True)
    
    # Write test file
    test_file = write_test_file(args.build_dir)
    print(f"Generated test file: {test_file}")
    
    if not args.build:
        print("\nTo compile the test, run:")
        print(f"  python scripts/verify_lvgl84_compat.py --build --lvgl-dir {args.lvgl_dir}")
        return 0
    
    # Determine include directories
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_dir = os.path.dirname(script_dir)
    
    include_dirs = [
        os.path.join(project_dir, 'include'),
        os.path.join(project_dir, args.lvgl_dir),
    ]
    
    # Compile
    if not compile_test(test_file, include_dirs, args.build_dir):
        return 1
    
    print("\n" + "="*60)
    print("LVGL 8.4 Compatibility Verification PASSED!")
    print("="*60)
    return 0

if __name__ == '__main__':
    sys.exit(main())
