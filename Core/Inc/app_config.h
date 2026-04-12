/**
 * @file app_config.h
 * @brief Central application-level constants for display, touch, and UI timing.
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
   
#define APP_DEBUG_ENABLED                 1U             ///< Enables debug output.

#define APP_LCD_WIDTH                     480U           ///< Active display width in pixels.
#define APP_LCD_HEIGHT                    272U           ///< Active display height in pixels.
#define APP_LCD_FRAMEBUFFER_ADDR          0xD0000000U    ///< SDRAM framebuffer base address.

#define APP_TOUCH_MAX_X                   APP_LCD_WIDTH  ///< Touch coordinate maximum on the X axis.
#define APP_TOUCH_MAX_Y                   APP_LCD_HEIGHT ///< Touch coordinate maximum on the Y axis.
#define APP_TOUCH_ACCURACY                5U             ///< Touch filter accuracy passed to the BSP touch driver.

#define APP_SHOW_BRINGUP_TEST_PATTERN     0U             ///< Enables the power-on color-bar screen before TouchGFX starts.
#define APP_TEST_PATTERN_HOLD_MS          5000U          ///< Duration of the bring-up test pattern in milliseconds.
#define APP_BREWING_DONE_HOLD_MS          2000U          ///< Hold time on the completed brewing screen in milliseconds.

#define APP_STEAM_TICK_INTERVAL_LOW       65U            ///< Steam animation tick interval for low intensity.
#define APP_STEAM_TICK_INTERVAL_NORMAL    43U            ///< Steam animation tick interval for normal intensity.
#define APP_STEAM_TICK_INTERVAL_STRONG    26U            ///< Steam animation tick interval for strong intensity.

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
