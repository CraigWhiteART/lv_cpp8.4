#pragma once
/**
 * @file smartwatch_snake.hpp
 * @brief Snake mini-game screen for smartwatch demo
 *
 * A 16×16 grid snake game accessible from the bottom of the settings/control
 * screen.  The player swipes to steer the snake; a long press returns to the
 * settings screen.
 */

#include "smartwatch_private.hpp"
#include <cstdio>
#include <cstring>

// Font declarations used by this screen
LV_FONT_DECLARE(font_inter_bold_24);
LV_FONT_DECLARE(font_inter_regular_24);

namespace smartwatch {

class SnakeScreen {
public:
    void create(DemoController& controller);
    [[nodiscard]] lv_obj_t* get() const { return m_screen; }

    /// Start (or restart) the game and reveal the screen.
    void start_game();
    /// Pause the game timer (called before hiding the screen).
    void stop_game();

private:
    // ── Grid / cell geometry ──────────────────────────────────────────────
    static constexpr int GRID_W     = 16;
    static constexpr int GRID_H     = 16;
    static constexpr int CELL_SIZE  = 16;   // px per cell
    static constexpr int CELL_GAP   = 2;    // px between cells
    static constexpr int CELL_STEP  = CELL_SIZE + CELL_GAP; // 18
    static constexpr int GAME_W     = GRID_W * CELL_STEP - CELL_GAP; // 286
    static constexpr int GAME_H     = GRID_H * CELL_STEP - CELL_GAP; // 286
    static constexpr int GAME_OFF_Y = 50;   // pixels from screen top

    // ── Game constants ────────────────────────────────────────────────────
    static constexpr uint32_t TICK_MS    = 200;   // ms per game step
    static constexpr int      MAX_LEN    = GRID_W * GRID_H; // 256
    static constexpr int      INITIAL_LEN = 3;

    // ── Cell colours ─────────────────────────────────────────────────────
    static constexpr uint32_t COL_EMPTY = 0x0f172a;
    static constexpr uint32_t COL_HEAD  = 0x4ade80; // bright green
    static constexpr uint32_t COL_BODY  = 0x166534; // dark green
    static constexpr uint32_t COL_FOOD  = 0xef4444; // red
    static constexpr uint32_t COL_DEAD  = 0xfbbf24; // amber on collision

    // ── Direction enum ───────────────────────────────────────────────────
    enum class Dir { UP, DOWN, LEFT, RIGHT };

    struct Point { int x = 0, y = 0; };

    // ── Event / callback handlers ─────────────────────────────────────────
    // on_gesture and on_long_press are deferred to smartwatch_gestures.hpp
    // because they require DemoController to be fully defined.
    void on_gesture(lv::Event e);
    void on_long_press(lv::Event e);
    void on_restart_click(lv::Event e);

    static void game_tick_cb(lv_timer_t* t);
    void do_tick();

    // ── Game-logic helpers ────────────────────────────────────────────────
    void reset_game();
    void spawn_food();
    [[nodiscard]] bool body_at(Point p, int segments_to_check) const;
    [[nodiscard]] Point next_head() const;

    // ── Display helpers ───────────────────────────────────────────────────
    void update_cells();   // Full redraw (called on reset)
    void set_cell_color(int x, int y, uint32_t color);
    void show_game_over();
    void hide_game_over();

    // ── Widgets ───────────────────────────────────────────────────────────
    lv_obj_t* m_screen          = nullptr;
    lv_obj_t* m_score_label     = nullptr;
    lv_obj_t* m_hi_score_label  = nullptr;
    lv_obj_t* m_game_over_cont  = nullptr;
    lv_obj_t* m_go_score_label  = nullptr;
    lv_obj_t* m_cells[MAX_LEN]{};

    DemoController* m_controller = nullptr;
    lv::Style m_main_style;
    bool m_inited = false;

    // Raw timer pointer (lives for the duration of the demo)
    lv_timer_t* m_timer = nullptr;

    // ── Snake state (circular buffer) ─────────────────────────────────────
    // m_snake[m_head_idx] is the head; earlier snake indices wrap backwards.
    // Body segment i from head: m_snake[(m_head_idx - i + MAX_LEN) % MAX_LEN]
    Point    m_snake[MAX_LEN]{};
    int      m_head_idx  = 0;
    int      m_len       = INITIAL_LEN;
    Dir      m_dir       = Dir::RIGHT;
    Dir      m_next_dir  = Dir::RIGHT;
    Point    m_food{};
    int      m_score     = 0;
    int      m_hi_score  = 0;
    bool     m_game_over = false;

    // Simple LCG PRNG (seeded in start_game)
    uint32_t m_rng = 42;
    uint32_t rand_next() noexcept {
        m_rng = m_rng * 1664525u + 1013904223u;
        return m_rng;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// create()
// ─────────────────────────────────────────────────────────────────────────────

inline void SnakeScreen::create(DemoController& controller) {
    m_controller = &controller;

    if (!m_inited) {
        m_main_style
            .text_color(lv::colors::white())
            .bg_color(lv::rgb(0x0a0a1a))
            .bg_opa(lv::opa::_100)
            .radius(lv::kRadius::circle)
            .translate_y(SCREEN_SIZE); // start off-screen below
        m_inited = true;
    }

    auto screen = lv::Box::create(lv::screen_active());
    screen.remove_all_styles()
        .add_style(m_main_style.get())
        .fill()
        .scrollbar_mode(lv::kScrollbarMode::off)
        .remove_flag(lv::kFlag::gesture_bubble)
        .remove_flag(lv::kFlag::event_bubble)
        .remove_flag(lv::kFlag::scrollable);
    screen.on<&SnakeScreen::on_gesture>(lv::kEvent::gesture, this)
          .on<&SnakeScreen::on_long_press>(lv::kEvent::long_pressed, this);
    m_screen = screen.get();

    // ── Header row ────────────────────────────────────────────────────────
    lv::Label::create(m_screen)
        .text("SNAKE")
        .pos(0, 10)
        .align(lv::kAlign::top_mid)
        .text_font(&font_inter_bold_24)
        .letter_space(3);

    m_score_label = lv::Label::create(m_screen)
        .text("Score: 0")
        .pos(-5, 12)
        .align(lv::kAlign::top_left)
        .get();

    m_hi_score_label = lv::Label::create(m_screen)
        .text("Best: 0")
        .pos(-5, 12)
        .align(lv::kAlign::top_right)
        .get();

    // ── Game container ────────────────────────────────────────────────────
    auto game_cont = lv::Box::create(m_screen);
    game_cont.remove_all_styles()
        .size(GAME_W, GAME_H)
        .pos(0, GAME_OFF_Y)
        .align(lv::kAlign::top_mid)
        .bg_color(lv::rgb(COL_EMPTY))
        .bg_opa(lv::opa::cover)
        .radius(6)
        .remove_flag(lv::kFlag::clickable)
        .remove_flag(lv::kFlag::scrollable);

    // ── Cell widgets ──────────────────────────────────────────────────────
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            auto cell = lv::Box::create(game_cont);
            cell.remove_all_styles()
                .size(CELL_SIZE, CELL_SIZE)
                .pos(x * CELL_STEP, y * CELL_STEP)
                .bg_color(lv::rgb(COL_EMPTY))
                .bg_opa(lv::opa::cover)
                .radius(3)
                .remove_flag(lv::kFlag::clickable)
                .remove_flag(lv::kFlag::scrollable);
            m_cells[x + y * GRID_W] = cell.get();
        }
    }

    // ── Controls hint ─────────────────────────────────────────────────────
    lv::Label::create(m_screen)
        .text("Swipe to steer  |  Hold to exit")
        .pos(0, -8)
        .align(lv::kAlign::bottom_mid)
        .opa(lv::opa::_60);

    // ── Game-over overlay (hidden initially) ──────────────────────────────
    auto go_cont = lv::Box::create(m_screen);
    go_cont.remove_all_styles()
        .fill()
        .bg_color(lv::rgb(0x000000))
        .bg_opa(lv::opa::_80)
        .remove_flag(lv::kFlag::scrollable)
        .add_flag(lv::kFlag::hidden);
    m_game_over_cont = go_cont.get();

    lv::Label::create(m_game_over_cont)
        .text("GAME OVER")
        .pos(0, -60)
        .align(lv::kAlign::center)
        .text_font(&font_inter_bold_24)
        .letter_space(2);

    m_go_score_label = lv::Label::create(m_game_over_cont)
        .text("Score: 0")
        .pos(0, -20)
        .align(lv::kAlign::center)
        .get();

    // Restart button
    auto restart_btn = lv::Box::create(m_game_over_cont);
    restart_btn.remove_all_styles()
        .size(160, 52)
        .pos(0, 30)
        .align(lv::kAlign::center)
        .bg_color(lv::rgb(0x4ade80))
        .bg_opa(lv::opa::cover)
        .radius(26)
        .add_flag(lv::kFlag::clickable)
        .remove_flag(lv::kFlag::scrollable);
    restart_btn.on_click<&SnakeScreen::on_restart_click>(this);

    lv::Label::create(restart_btn)
        .text("PLAY AGAIN")
        .text_color(lv::rgb(0x052e16))
        .align(lv::kAlign::center)
        .text_font(&font_inter_bold_24);

    // Exit hint inside overlay
    lv::Label::create(m_game_over_cont)
        .text("or hold to exit")
        .pos(0, 90)
        .align(lv::kAlign::center)
        .opa(lv::opa::_60);

    // ── Game timer (paused until start_game() is called) ──────────────────
    m_timer = lv_timer_create(game_tick_cb, TICK_MS, this);
    lv_timer_pause(m_timer);

    // Initialise game state (draws initial cells)
    reset_game();
}

// ─────────────────────────────────────────────────────────────────────────────
// Public control
// ─────────────────────────────────────────────────────────────────────────────

inline void SnakeScreen::start_game() {
    // Mix tick count with both halves of this pointer for a varied seed
    const uintptr_t p = reinterpret_cast<uintptr_t>(this);
    m_rng = lv_tick_get()
          ^ static_cast<uint32_t>(p)
          ^ static_cast<uint32_t>(p >> 32);
    hide_game_over();
    reset_game();
    lv_timer_reset(m_timer);
    lv_timer_resume(m_timer);
}

inline void SnakeScreen::stop_game() {
    lv_timer_pause(m_timer);
}

// ─────────────────────────────────────────────────────────────────────────────
// Timer callback
// ─────────────────────────────────────────────────────────────────────────────

/*static*/ inline void SnakeScreen::game_tick_cb(lv_timer_t* t) {
    static_cast<SnakeScreen*>(lv_timer_get_user_data(t))->do_tick();
}

// ─────────────────────────────────────────────────────────────────────────────
// Game logic
// ─────────────────────────────────────────────────────────────────────────────

inline void SnakeScreen::reset_game() {
    m_dir       = Dir::RIGHT;
    m_next_dir  = Dir::RIGHT;
    m_score     = 0;
    m_len       = INITIAL_LEN;
    m_head_idx  = INITIAL_LEN - 1; // = 2
    m_game_over = false;

    // Place initial snake in the upper-left quarter, moving right
    const int sx = GRID_W / 4;
    const int sy = GRID_H / 2;
    for (int i = 0; i < INITIAL_LEN; i++) {
        m_snake[i] = {sx + i, sy};
        // m_snake[0]=tail, m_snake[1]=body, m_snake[2]=head
    }

    // Refresh score labels
    lv::Label(lv::wrap, m_score_label).text("Score: 0");

    spawn_food();
    update_cells(); // Full redraw of the grid
}

inline void SnakeScreen::spawn_food() {
    Point p;
    do {
        p = {static_cast<int>(rand_next() % static_cast<uint32_t>(GRID_W)),
             static_cast<int>(rand_next() % static_cast<uint32_t>(GRID_H))};
    } while (body_at(p, m_len));
    m_food = p;
}

inline bool SnakeScreen::body_at(Point p, int segments_to_check) const {
    for (int i = 0; i < segments_to_check; i++) {
        int idx = (m_head_idx - i + MAX_LEN) % MAX_LEN;
        if (m_snake[idx].x == p.x && m_snake[idx].y == p.y) return true;
    }
    return false;
}

inline SnakeScreen::Point SnakeScreen::next_head() const {
    Point h = m_snake[m_head_idx];
    switch (m_dir) {
        case Dir::UP:    return {h.x,     h.y - 1};
        case Dir::DOWN:  return {h.x,     h.y + 1};
        case Dir::LEFT:  return {h.x - 1, h.y    };
        case Dir::RIGHT: return {h.x + 1, h.y    };
    }
    LV_ASSERT_MSG(false, "unreachable: unknown snake direction");
    return h;
}

inline void SnakeScreen::do_tick() {
    if (m_game_over) return;

    m_dir = m_next_dir;
    const Point nh = next_head();

    // Wall collision
    if (nh.x < 0 || nh.x >= GRID_W || nh.y < 0 || nh.y >= GRID_H) {
        m_game_over = true;
        set_cell_color(m_snake[m_head_idx].x, m_snake[m_head_idx].y, COL_DEAD);
        show_game_over();
        lv_timer_pause(m_timer);
        return;
    }

    const bool eating = (nh.x == m_food.x && nh.y == m_food.y);

    // Self-collision (exclude tail when not eating – it moves away this tick)
    if (body_at(nh, eating ? m_len : m_len - 1)) {
        m_game_over = true;
        set_cell_color(m_snake[m_head_idx].x, m_snake[m_head_idx].y, COL_DEAD);
        show_game_over();
        lv_timer_pause(m_timer);
        return;
    }

    if (!eating) {
        // Erase old tail cell
        int tail_idx = (m_head_idx - m_len + 1 + MAX_LEN) % MAX_LEN;
        set_cell_color(m_snake[tail_idx].x, m_snake[tail_idx].y, COL_EMPTY);
    } else {
        m_len++;
        m_score += 10;

        char buf[32];
        snprintf(buf, sizeof(buf), "Score: %d", m_score);
        lv::Label(lv::wrap, m_score_label).text(buf);

        if (m_score > m_hi_score) {
            m_hi_score = m_score;
            snprintf(buf, sizeof(buf), "Best: %d", m_hi_score);
            lv::Label(lv::wrap, m_hi_score_label).text(buf);
        }

        if (m_len < MAX_LEN) {
            spawn_food();
            set_cell_color(m_food.x, m_food.y, COL_FOOD);
        }
    }

    // Old head → body colour
    set_cell_color(m_snake[m_head_idx].x, m_snake[m_head_idx].y, COL_BODY);

    // Advance head in circular buffer
    m_head_idx = (m_head_idx + 1) % MAX_LEN;
    m_snake[m_head_idx] = nh;
    set_cell_color(nh.x, nh.y, COL_HEAD);
}

// ─────────────────────────────────────────────────────────────────────────────
// Display helpers
// ─────────────────────────────────────────────────────────────────────────────

inline void SnakeScreen::set_cell_color(int x, int y, uint32_t color) {
    if (x < 0 || x >= GRID_W || y < 0 || y >= GRID_H) return;
    lv::Box(lv::wrap, m_cells[x + y * GRID_W]).bg_color(lv::rgb(color));
}

inline void SnakeScreen::update_cells() {
    // Reset all cells to empty
    for (int i = 0; i < GRID_W * GRID_H; i++) {
        lv::Box(lv::wrap, m_cells[i]).bg_color(lv::rgb(COL_EMPTY));
    }

    // Draw snake (tail→head so head colour is painted last)
    for (int seg = m_len - 1; seg >= 0; seg--) {
        int idx = (m_head_idx - seg + MAX_LEN) % MAX_LEN;
        set_cell_color(m_snake[idx].x, m_snake[idx].y,
                       (seg == 0) ? COL_HEAD : COL_BODY);
    }

    // Draw food
    set_cell_color(m_food.x, m_food.y, COL_FOOD);
}

inline void SnakeScreen::show_game_over() {
    char buf[32];
    snprintf(buf, sizeof(buf), "Score: %d", m_score);
    lv::Label(lv::wrap, m_go_score_label).text(buf);
    lv::Box(lv::wrap, m_game_over_cont).remove_flag(lv::kFlag::hidden);
}

inline void SnakeScreen::hide_game_over() {
    lv::Box(lv::wrap, m_game_over_cont).add_flag(lv::kFlag::hidden);
}

// ─────────────────────────────────────────────────────────────────────────────
// Button handler (doesn't need DemoController)
// ─────────────────────────────────────────────────────────────────────────────

inline void SnakeScreen::on_restart_click(lv::Event /*e*/) {
    hide_game_over();
    m_rng = lv_tick_get();
    reset_game();
    lv_timer_reset(m_timer);
    lv_timer_resume(m_timer);
}

// on_gesture and on_long_press are defined in smartwatch_gestures.hpp
// (they require DemoController to be fully defined).

} // namespace smartwatch
