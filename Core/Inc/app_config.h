#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define APP_LCD_WIDTH                     480U
#define APP_LCD_HEIGHT                    272U
#define APP_LCD_FRAMEBUFFER_ADDR          0xD0000000U

#define APP_TOUCH_MAX_X                   APP_LCD_WIDTH
#define APP_TOUCH_MAX_Y                   APP_LCD_HEIGHT
#define APP_TOUCH_ACCURACY                5U

#define APP_SHOW_BRINGUP_TEST_PATTERN     0U
#define APP_TEST_PATTERN_HOLD_MS          5000U
#define APP_BREWING_DONE_HOLD_MS          2000U

#define APP_STEAM_TICK_INTERVAL_LOW       65U
#define APP_STEAM_TICK_INTERVAL_NORMAL    43U
#define APP_STEAM_TICK_INTERVAL_STRONG    26U

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
