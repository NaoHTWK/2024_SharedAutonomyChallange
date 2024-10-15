#pragma once

#include <cstdint>

/**
 * We use these functions to handle signals and have a graceful shutdown.
 */
namespace SignalHandling {

/**
 * @brief This function initializes the signal handler and must be called before any thread was created. Threads inherit
 * the signal handler.
 */
void init();

/**
 * @brief Indicates that a signal was received and we should terminate.
 * @return true if it was requested that we terminate.
 */
bool shutdown();

/**
 * @brief Requests the software to be shut down.
 */
void requestShutdown();

/**
 * @brief Set how long do we wait until we kill everything hard. Default is 5s.
 * @param timeout_in_us The timeout in us (use e.g. 5_s)
 */
void setWatchdogTimeout(uint32_t timeout_in_us);

}  // namespace SignalHandling
