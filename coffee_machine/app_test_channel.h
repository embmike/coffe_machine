/**
 * @file app_test_channel.h
 * @brief C bridge for the optional USB-CDC system-test channel.
 */

#ifndef APP_TEST_CHANNEL_H
#define APP_TEST_CHANNEL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Feeds received USB-CDC bytes into the system-test channel.
 * @param data Received byte buffer.
 * @param length Number of valid bytes in @p data.
 */
void AppTest_ChannelOnRxData(const uint8_t* data, size_t length);

/**
 * @brief Processes one pending system-test command if available.
 */
void AppTest_ChannelProcess(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_TEST_CHANNEL_H */
