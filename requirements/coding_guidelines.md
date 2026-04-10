Klar — hier ist eine **direkt nutzbare Markdown-Vorlage** für deine `coding_guidelines.md`, inklusive:

- allgemeine C/C++-Vorgaben
- Doxygen-Regeln
- Templates für `.hpp/.h`
- Templates für `.cpp/.c`

Du kannst den Inhalt einfach kopieren und als `coding_guidelines.md` speichern.

------

# Coding Guidelines for C/C++

## 1. General

These coding guidelines define the required style and documentation rules for all C and C++ source files of this project.

Goals:

- readable and maintainable code
- consistent naming and formatting
- clear API contracts
- audit-friendly documentation
- low-risk embedded software development

------

## 2. Formatting Rules

### 2.1 Brace Style

Use **Allman-Brace-Style**.

Example:

```cpp
if (is_valid == true)
{
    do_something();
}
else
{
    do_something_else();
}
```

------

### 2.2 Initialization

Use **uniform / brace initialization** wherever possible.

Example:

```cpp
uint32_t frequency_hz { 48'000'000 };
float temperature_celsius { 23.5F };
BoardController board { };
```

------

### 2.3 Indentation

- use 4 spaces per indentation level
- do not use tabs

------

### 2.4 Line Length

Recommended maximum line length: **120 characters**

------

### 2.5 One Statement per Line

Write only one statement per line.

Allowed:

```cpp
uint32_t frequency_hz { 0U };
frequency_hz = BOARD_FREQUENCY;
```

Not allowed:

```cpp
uint32_t frequency_hz { 0U }; frequency_hz = BOARD_FREQUENCY;
```

------

## 3. Naming Conventions

### 3.1 Constants

Use **SCREAMING_SNAKE_CASE** for constants.

Example:

```cpp
constexpr uint32_t BOARD_FREQUENCY { 48'000'000 };
constexpr uint32_t MAX_SENSOR_COUNT { 4U };
```

------

### 3.2 Variables and Functions

Use **snake_case** for:

- variables
- function names
- method names
- parameters
- local variables
- member variables

Example:

```cpp
uint32_t frequency_hz { 0U };

void set_frequency(uint32_t frequency);
float read_temperature() const;
```

------

### 3.3 Type Names

Use **PascalCase** for:

- classes
- structs
- enums
- typedef-like abstractions

Example:

```cpp
class BoardController;
struct SensorData;
enum class SensorStatus;
```

------

### 3.4 Member Variables

Use **snake_case with trailing underscore** for member variables.

Example:

```cpp
uint32_t frequency_hz_ { 0U };
float temperature_celsius_ { 0.0F };
```

------

### 3.5 Enum Values

Use **snake_case** for scoped enum values.

Example:

```cpp
enum class SensorStatus
{
    ok,
    error,
    not_available
};
```

------

## 4. Function and Method Design

### 4.1 Method Naming

Methods should follow the form:

```cpp
object.verb_object(...)
```

Examples:

```cpp
board.set_frequency(BOARD_FREQUENCY);
board.set_max_frequency(MAX_FREQUENCY);
board.set_min_frequency(MIN_FREQUENCY);

sensor.read_temperature();
sensor.set_status(SensorStatus::error);
logger.write_message("System started");
```

------

### 4.2 Function Design Principles

Functions should:

- have a single clear responsibility
- be short and readable
- use descriptive parameter names
- validate input when required
- avoid hidden side effects

------

## 5. Documentation Rules

Documentation shall be written in **Doxygen style**.

### 5.1 Use `@brief`

Every public function, method, class, struct, enum, and module shall provide a short summary using `@brief`.

------

### 5.2 Use `@details`

Use `@details` when additional explanation is needed, for example:

- algorithm description
- constraints
- assumptions
- limitations
- safety-relevant behavior
- preconditions and postconditions

Example:

```cpp
/**
 * @brief Sets the operating frequency.
 * @details Validates the requested frequency against the configured
 *          minimum and maximum frequency limits.
 *
 * @param[in] frequency Desired frequency in Hz.
 */
void set_frequency(uint32_t frequency);
```

------

### 5.3 Use `@param[in]`, `@param[out]`, `@param[in,out]`

Document parameters explicitly.

Use:

- `@param[in]` for input-only parameters
- `@param[out]` for output-only parameters
- `@param[in,out]` for modified input/output parameters

This is especially important for:

- pointers
- references
- buffers
- non-const parameters

Example:

```cpp
/**
 * @brief Reads data into a destination buffer.
 * @details Copies the current payload into the user-provided buffer.
 *
 * @param[out] destination Pointer to destination buffer.
 * @param[in]  destination_size Size of destination buffer in bytes.
 * @return Number of bytes written.
 */
size_t read_data(uint8_t* destination, size_t destination_size);
```

Example with in/out parameter:

```cpp
/**
 * @brief Advances the parser position.
 * @details Updates the index to the next unread byte.
 *
 * @param[in,out] index Current buffer index, updated by the function.
 */
void advance_index(size_t& index);
```

------

### 5.4 Use `@return`

Document all non-void return values.

Example:

```cpp
/**
 * @brief Returns the current operating frequency.
 * @return Current frequency in Hz.
 */
uint32_t get_frequency() const;
```

------

### 5.5 Use `///<` for Variables and Constants

Use trailing Doxygen comments for:

- constants
- member variables
- global/static variables where appropriate
- enum values
- optionally important local variables

Example:

```cpp
constexpr uint32_t MAX_FREQUENCY { 120'000'000U }; ///< Maximum frequency based on data sheet
uint32_t frequency_hz_ { 0U };                     ///< Current operating frequency in Hz
```

------

## 6. Header File Rules

Header files shall:

- contain declarations only
- avoid implementation unless explicitly intended
- use include guards or `#pragma once`
- include only what is required
- prefer forward declarations where appropriate

------

## 7. Source File Rules

Source files shall:

- contain the implementation
- include the matching header first
- keep internal symbols `static` or in anonymous namespaces where applicable
- document non-trivial internal functions as needed

------

## 8. C++ Template for `.hpp` Files

```cpp
#pragma once

#include <cstdint>
#include <cstddef>

/**
 * @brief Describes the status of a sensor.
 */
enum class SensorStatus
{
    ok,             ///< Sensor operates correctly
    error,          ///< Sensor error detected
    not_available   ///< Sensor is not available
};

/**
 * @brief Holds one sensor measurement.
 */
struct SensorData
{
    float value { 0.0F };              ///< Measured sensor value
    uint32_t timestamp_ms { 0U };      ///< Timestamp in milliseconds
    SensorStatus status { SensorStatus::ok }; ///< Current sensor status
};

/**
 * @brief Provides access to board-related configuration.
 */
class BoardController
{
public:
    /**
     * @brief Constructs the board controller.
     */
    BoardController() = default;

    /**
     * @brief Sets the operating frequency.
     * @details The frequency is applied only if it is within the allowed
     *          minimum and maximum limits.
     *
     * @param[in] frequency Desired operating frequency in Hz.
     */
    void set_frequency(uint32_t frequency);

    /**
     * @brief Sets the maximum allowed frequency.
     *
     * @param[in] max_frequency Maximum frequency in Hz.
     */
    void set_max_frequency(uint32_t max_frequency);

    /**
     * @brief Sets the minimum allowed frequency.
     *
     * @param[in] min_frequency Minimum frequency in Hz.
     */
    void set_min_frequency(uint32_t min_frequency);

    /**
     * @brief Returns the currently configured frequency.
     *
     * @return Current frequency in Hz.
     */
    uint32_t get_frequency() const;

private:
    uint32_t frequency_hz_ { 48'000'000U };   ///< Current operating frequency in Hz
    uint32_t max_frequency_hz_ { 120'000'000U }; ///< Maximum allowed frequency in Hz
    uint32_t min_frequency_hz_ { 1'000'000U };   ///< Minimum allowed frequency in Hz
};
```

------

## 9. C++ Template for `.cpp` Files

```cpp
#include "board_controller.hpp"

/**
 * @brief Sets the operating frequency.
 * @details The function updates the board frequency only if the given
 *          value is within the configured frequency range.
 *
 * @param[in] frequency Desired operating frequency in Hz.
 */
void BoardController::set_frequency(uint32_t frequency)
{
    if ((frequency >= min_frequency_hz_) && (frequency <= max_frequency_hz_))
    {
        frequency_hz_ = frequency;
    }
}

/**
 * @brief Sets the maximum allowed frequency.
 *
 * @param[in] max_frequency Maximum allowed frequency in Hz.
 */
void BoardController::set_max_frequency(uint32_t max_frequency)
{
    max_frequency_hz_ = max_frequency;
}

/**
 * @brief Sets the minimum allowed frequency.
 *
 * @param[in] min_frequency Minimum allowed frequency in Hz.
 */
void BoardController::set_min_frequency(uint32_t min_frequency)
{
    min_frequency_hz_ = min_frequency;
}

/**
 * @brief Returns the currently configured frequency.
 *
 * @return Current frequency in Hz.
 */
uint32_t BoardController::get_frequency() const
{
    return frequency_hz_;
}
```

------

## 10. C Template for `.h` Files

```c
#ifndef BOARD_DRIVER_H
#define BOARD_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Maximum operating frequency based on data sheet.
 */
#define MAX_FREQUENCY_HZ    (120000000U)

/**
 * @brief Minimum operating frequency based on data sheet.
 */
#define MIN_FREQUENCY_HZ    (1000000U)

/**
 * @brief Default board frequency.
 */
#define BOARD_FREQUENCY_HZ  (48000000U)

/**
 * @brief Describes the result of a board operation.
 */
typedef enum
{
    BOARD_STATUS_OK = 0,            /**< Operation completed successfully */
    BOARD_STATUS_ERROR,             /**< General error */
    BOARD_STATUS_INVALID_PARAMETER  /**< Invalid parameter detected */
} Board_Status;

/**
 * @brief Initializes the board driver.
 * @details This function initializes the internal board state and
 *          prepares the driver for later use.
 *
 * @return Operation result.
 */
Board_Status board_init(void);

/**
 * @brief Sets the board frequency.
 * @details The frequency value is validated against the allowed range.
 *
 * @param[in] frequency_hz Desired board frequency in Hz.
 * @return Operation result.
 */
Board_Status board_set_frequency(uint32_t frequency_hz);

/**
 * @brief Returns the current board frequency.
 *
 * @param[out] frequency_hz Pointer to destination variable.
 * @return Operation result.
 */
Board_Status board_get_frequency(uint32_t* frequency_hz);

#endif
```

------

## 11. C Template for `.c` Files

```c
#include "board_driver.h"

/** @brief Current board frequency in Hz. */
static uint32_t current_frequency_hz = BOARD_FREQUENCY_HZ;

/**
 * @brief Initializes the board driver.
 * @details Resets the internal driver state to its default values.
 *
 * @return Operation result.
 */
Board_Status board_init(void)
{
    current_frequency_hz = BOARD_FREQUENCY_HZ;
    return BOARD_STATUS_OK;
}

/**
 * @brief Sets the board frequency.
 * @details The frequency is accepted only if it lies within the valid
 *          minimum and maximum frequency limits.
 *
 * @param[in] frequency_hz Desired board frequency in Hz.
 * @return Operation result.
 */
Board_Status board_set_frequency(uint32_t frequency_hz)
{
    if ((frequency_hz < MIN_FREQUENCY_HZ) || (frequency_hz > MAX_FREQUENCY_HZ))
    {
        return BOARD_STATUS_INVALID_PARAMETER;
    }

    current_frequency_hz = frequency_hz;
    return BOARD_STATUS_OK;
}

/**
 * @brief Returns the current board frequency.
 *
 * @param[out] frequency_hz Pointer to destination variable.
 * @return Operation result.
 */
Board_Status board_get_frequency(uint32_t* frequency_hz)
{
    if (frequency_hz == NULL)
    {
        return BOARD_STATUS_INVALID_PARAMETER;
    }

    *frequency_hz = current_frequency_hz;
    return BOARD_STATUS_OK;
}
```

------

## 12. Recommended Doxygen Patterns

### 12.1 Function with input parameter

```cpp
/**
 * @brief Sets the operating mode.
 *
 * @param[in] mode Desired operating mode.
 */
void set_mode(OperatingMode mode);
```

### 12.2 Function with output pointer

```cpp
/**
 * @brief Reads the current value.
 *
 * @param[out] value Pointer to destination variable.
 * @return true if reading was successful, otherwise false.
 */
bool read_value(uint32_t* value);
```

### 12.3 Function with in/out parameter

```cpp
/**
 * @brief Updates the buffer index.
 *
 * @param[in,out] index Current index, updated by this function.
 */
void update_index(size_t& index);
```

### 12.4 Function with detailed description

```cpp
/**
 * @brief Calculates the factorial of a number.
 * @details This function uses a recursive algorithm to compute
 *          the factorial of a non-negative integer.
 *          If the input is negative, the behavior is undefined.
 *
 * @param[in] n The number whose factorial is to be computed.
 * @return The factorial of the number.
 */
int factorial(int n);
```

------

## 13. Recommended Header Comment Template

```cpp
/**
 * @file board_controller.hpp
 * @brief Declaration of the BoardController class.
 * @details This file contains the public interface for board frequency
 *          management and related configuration functions.
 */
```

For C:

```c
/**
 * @file board_driver.h
 * @brief Public API of the board driver.
 * @details This file contains the declarations for the board driver
 *          used by the application layer.
 */
```

------

## 14. Recommended Source File Comment Template

```cpp
/**
 * @file board_controller.cpp
 * @brief Implementation of the BoardController class.
 */
```

For C:

```c
/**
 * @file board_driver.c
 * @brief Implementation of the board driver.
 */
```

------

## 15. Practical Recommendations

### Prefer this

```cpp
board.set_frequency(BOARD_FREQUENCY);
sensor.read_temperature();
controller.update_state();
```

### Avoid this

```cpp
board.Set_Frequency(BOARD_FREQUENCY);
sensor.Read_Temperature();
controller.DoStuff();
```

Reason:

- inconsistent naming
- unclear semantics
- less idiomatic in modern C/C++

------

## 16. Summary

Required rules:

- Doxygen-style comments
- `@brief` for public APIs
- `@details` for non-trivial behavior
- `@param[in]`, `@param[out]`, `@param[in,out]` for parameters
- `@return` for non-void functions
- `///<` for constants, variables, members, enum values
- uniform / brace initialization
- Allman brace style
- `SCREAMING_SNAKE_CASE` for constants
- `snake_case` for variables and functions
- `PascalCase` for types
- member variables with trailing underscore

------

Wenn du möchtest, formatiere ich dir das im nächsten Schritt noch in einer **etwas strengeren Embedded-/Automotive-Version**, also mit Abschnitten wie:

- include order
- `constexpr` statt `#define`
- `enum class` bevorzugen
- heap avoidance
- error handling rules
- testability rules

Dann wirkt die Datei noch mehr wie ein echtes Projekt-Standarddokument.