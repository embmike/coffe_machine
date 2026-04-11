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

#define APP_SHOW_BRINGUP_TEST_PATTERN     1U
#define APP_TEST_PATTERN_HOLD_MS          5000U

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
