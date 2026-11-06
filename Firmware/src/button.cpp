#include "button.h"
#include "config.h"

static bool last_stable_state = HIGH;
static bool last_reading = HIGH;
static uint32_t last_debounce_time = 0;
constexpr uint32_t DEBOUNCE_DELAY_MS = 35;

void button_init() {
    // Trạng thái bình thường kéo PULLUP là HIGH, bấm xuống là LOW
    pinMode(PIN_BUTTON_RESET, INPUT_PULLUP);
}

bool is_button_pressed() {
    bool current_reading = digitalRead(PIN_BUTTON_RESET);

    if (current_reading != last_reading) {
        last_debounce_time = millis();
    }
    last_reading = current_reading;

    if ((millis() - last_debounce_time) > DEBOUNCE_DELAY_MS) {
        if (current_reading != last_stable_state) {
            last_stable_state = current_reading;
        }
    }

    return (last_stable_state == LOW);
}
