#include "driver/ledc.h"
#include "esp_rom_sys.h"

#define SERVO_MIN_PULSEWIDTH_US 500
#define SERVO_MAX_PULSEWIDTH_US 2500
#define SERVO_MAX_DEGREE        180

const int servo_gpio[4] = {13, 12, 14, 27};

void set_servo_angle(int channel, int angle) {
    int duty_us = SERVO_MIN_PULSEWIDTH_US +
                  (angle * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US)) / SERVO_MAX_DEGREE;

    uint32_t duty = ((uint64_t)duty_us * (1 << 16)) / 20000;
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}

void app_main(void) {
    for (int i = 0; i < 4; i++) {
        ledc_timer_config_t timer = {
            .duty_resolution = LEDC_TIMER_16_BIT,
            .freq_hz = 50,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = i,
            .clk_cfg = LEDC_AUTO_CLK
        };
        ledc_timer_config(&timer);

        ledc_channel_config_t channel = {
            .gpio_num = servo_gpio[i],
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel = i,
            .timer_sel = i,
            .duty = 0,
            .hpoint = 0
        };
        ledc_channel_config(&channel);
    }

    while (1) {
        for (int angle = 0; angle <= 180; angle += 45) {
            for (int i = 0; i < 4; i++) {
                set_servo_angle(i, angle);
            }
            esp_rom_delay_us(500000); // 0.5 seconds
        }
        for (int angle = 180; angle >= 0; angle -= 45) {
            for (int i = 0; i < 4; i++) {
                set_servo_angle(i, angle);
            }
            esp_rom_delay_us(500000);
        }
    }
}

