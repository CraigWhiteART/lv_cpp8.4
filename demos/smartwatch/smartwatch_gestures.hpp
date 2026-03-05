#pragma once
/**
 * @file smartwatch_gestures.hpp
 * @brief Deferred gesture handlers for smartwatch demo
 *
 * This file contains the on_gesture implementations that were deferred
 * from individual screen headers because they need access to DemoController
 * (which is defined in smartwatch_demo.hpp).
 *
 * Include this AFTER smartwatch_demo.hpp to have full type information.
 */

#include "smartwatch_demo.hpp"

namespace smartwatch {

// ControlScreen::on_gesture
inline void ControlScreen::on_gesture(lv::Event e) {
    (void)e;
    lv_dir_t dir = lv::Indev(lv::indev_active()).gesture_dir();

    if (dir == lv::kDir::left) {
        animate_y(m_screen, -SCREEN_SIZE, 1000, 0);
        animate_arc(m_controller->arc_cont(), ArcAnimation::EXPAND_UP, 700, 300);
        animate_opa(m_controller->main_arc(), 255, 500, 500);
    }
}

inline void ControlScreen::on_long_press(lv::Event e) {
    (void)e;
    animate_y(m_screen, -SCREEN_SIZE, 1000, 0);
    animate_arc(m_controller->arc_cont(), ArcAnimation::EXPAND_UP, 700, 300);
    animate_opa(m_controller->main_arc(), 255, 500, 500);
}

// ControlScreen::on_snake_click – open the snake game screen
inline void ControlScreen::on_snake_click(lv::Event /*e*/) {
    // Dismiss the settings screen upward
    animate_y(m_screen, -SCREEN_SIZE, 800, 0);
    // Start the game and slide the snake screen up from below
    m_controller->snake_screen().start_game();
    animate_y(m_controller->snake_screen().get(), 0, 800, 100);
}

// WeatherScreen::on_gesture
inline void WeatherScreen::on_gesture(lv::Event e) {
    (void)e;
    lv_dir_t dir = lv::Indev(lv::indev_active()).gesture_dir();

    if (dir == lv::kDir::right) {
        animate_x(m_screen, SCREEN_SIZE, 1000, 0);
        animate_arc(m_controller->arc_cont(), ArcAnimation::EXPAND_RIGHT, 700, 300);
        animate_opa(m_controller->main_arc(), 255, 500, 500);
        animate_opa(m_controller->overlay(), 0, 100, 0);
    }
    if (dir == lv::kDir::left) {
        animate_opa(m_controller->overlay(), 255, 100, 0);
        animate_x(m_screen, -SCREEN_SIZE - TRANSITION_GAP, 1000, 0);
        animate_x_from(m_controller->health_screen().get(), SCREEN_SIZE + TRANSITION_GAP, 0, 1000, 0);
    }
}

// HealthScreen::on_gesture
inline void HealthScreen::on_gesture(lv::Event e) {
    (void)e;
    lv_dir_t dir = lv::Indev(lv::indev_active()).gesture_dir();

    if (dir == lv::kDir::left) {
        animate_x(m_screen, -SCREEN_SIZE - TRANSITION_GAP, 1000, 0);
        animate_x_from(m_controller->sports_screen().get(), SCREEN_SIZE + TRANSITION_GAP, 0, 1000, 0);
    }
    if (dir == lv::kDir::right) {
        animate_x_from(m_controller->weather_screen().get(), -SCREEN_SIZE - TRANSITION_GAP, 0, 1000, 0);
        animate_x(m_screen, SCREEN_SIZE + TRANSITION_GAP, 1000, 0);
    }
}

// SportsScreen::on_gesture
inline void SportsScreen::on_gesture(lv::Event e) {
    (void)e;
    lv_dir_t dir = lv::Indev(lv::indev_active()).gesture_dir();

    if (dir == lv::kDir::left) {
        animate_x(m_screen, -SCREEN_SIZE - TRANSITION_GAP, 1000, 0);
        animate_x_from(m_controller->music_screen().get(), SCREEN_SIZE + TRANSITION_GAP, 0, 1000, 0);
    }
    if (dir == lv::kDir::right) {
        animate_x_from(m_controller->health_screen().get(), -SCREEN_SIZE - TRANSITION_GAP, 0, 1000, 0);
        animate_x(m_screen, SCREEN_SIZE + TRANSITION_GAP, 1000, 0);
    }
}

// MusicScreen::on_gesture
inline void MusicScreen::on_gesture(lv::Event e) {
    (void)e;
    lv_dir_t dir = lv::Indev(lv::indev_active()).gesture_dir();

    if (dir == lv::kDir::left) {
        animate_x(m_screen, -SCREEN_SIZE, 1000, 0);
        animate_arc(m_controller->arc_cont(), ArcAnimation::EXPAND_RIGHT, 700, 300);
        animate_opa(m_controller->main_arc(), 255, 500, 500);
        animate_opa(m_controller->overlay(), 0, 100, 0);
    }
    if (dir == lv::kDir::right) {
        animate_x_from(m_controller->sports_screen().get(), -SCREEN_SIZE - TRANSITION_GAP, 0, 1000, 0);
        animate_x(m_screen, SCREEN_SIZE + TRANSITION_GAP, 1000, 0);
    }
}

// SnakeScreen::on_gesture – steer the snake while the game is active
inline void SnakeScreen::on_gesture(lv::Event e) {
    (void)e;
    if (m_game_over) return; // Long press handles navigation when game is over

    lv_dir_t dir = lv::Indev(lv::indev_active()).gesture_dir();

    if      (dir == lv::kDir::top    && m_dir != Dir::DOWN)  m_next_dir = Dir::UP;
    else if (dir == lv::kDir::bottom && m_dir != Dir::UP)    m_next_dir = Dir::DOWN;
    else if (dir == lv::kDir::left   && m_dir != Dir::RIGHT) m_next_dir = Dir::LEFT;
    else if (dir == lv::kDir::right  && m_dir != Dir::LEFT)  m_next_dir = Dir::RIGHT;
}

// SnakeScreen::on_long_press – return to the settings/control screen
inline void SnakeScreen::on_long_press(lv::Event /*e*/) {
    stop_game();
    // Slide snake screen back down off-screen
    animate_y(m_screen, SCREEN_SIZE, 800, 0);
    // Bring the settings screen back into view
    animate_y(m_controller->control_screen().get(), 0, 800, 100);
}

} // namespace smartwatch
