/**
  ******************************************************************************
  * @file    ft5336_conf.h
  * @brief   Local configuration for the FT5336 touch controller driver.
  ******************************************************************************
  */
#ifndef FT5336_CONF_H
#define FT5336_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app_config.h"

#define FT5336_AUTO_CALIBRATION_ENABLED      0U
#define FT5336_MAX_X_LENGTH                  APP_TOUCH_MAX_X
#define FT5336_MAX_Y_LENGTH                  APP_TOUCH_MAX_Y

#ifdef __cplusplus
}
#endif

#endif /* FT5336_CONF_H */
