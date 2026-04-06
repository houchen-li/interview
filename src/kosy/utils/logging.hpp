/**
 * @file logging.hpp
 * @author Houchen Li (houchen_li@hotmail.com)
 * @brief
 * @version 0.1
 * @date 2024-05-09
 *
 * @copyright Copyright (c) 2024 Kosy Development Team.
 *            All rights reserved.
 *
 */

#pragma once

#include <memory>
#include <string_view>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#define KOSY_LOG_LEVEL_TRACE SPDLOG_LEVEL_TRACE
#define KOSY_LOG_LEVEL_DEBUG SPDLOG_LEVEL_DEBUG
#define KOSY_LOG_LEVEL_INFO SPDLOG_LEVEL_INFO
#define KOSY_LOG_LEVEL_WARN SPDLOG_LEVEL_WARN
#define KOSY_LOG_LEVEL_ERROR SPDLOG_LEVEL_ERROR
#define KOSY_LOG_LEVEL_CRITICAL SPDLOG_LEVEL_CRITICAL
#define KOSY_LOG_LEVEL_OFF SPDLOG_LEVEL_OFF

#if !defined(KOSY_LOG_ACTIVE_LEVEL)
#define KOSY_LOG_ACTIVE_LEVEL KOSY_LOG_LEVEL_INFO
#endif

#ifndef KOSY_LOG_NO_SOURCE_LOC
#define KOSY_LOG_LOGGER_CALL(logger, level, ...) \
    (logger)->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_CALL(logger, level, ...) \
    (logger)->log(spdlog::source_loc{}, level, __VA_ARGS__)
#endif

#if KOSY_LOG_ACTIVE_LEVEL <= KOSY_LOG_LEVEL_TRACE
#define KOSY_LOG_LOGGER_TRACE(logger, ...) \
    KOSY_LOG_LOGGER_CALL(logger, spdlog::level::trace, __VA_ARGS__)
#define KOSY_LOG_TRACE(...) KOSY_LOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_TRACE(logger, ...) (void)0
#define KOSY_LOG_TRACE(...) (void)0
#endif

#if KOSY_LOG_ACTIVE_LEVEL <= KOSY_LOG_LEVEL_DEBUG
#define KOSY_LOG_LOGGER_DEBUG(logger, ...) \
    KOSY_LOG_LOGGER_CALL(logger, spdlog::level::debug, __VA_ARGS__)
#define KOSY_LOG_DEBUG(...) KOSY_LOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_DEBUG(logger, ...) (void)0
#define KOSY_LOG_DEBUG(...) (void)0
#endif

#if KOSY_LOG_ACTIVE_LEVEL <= KOSY_LOG_LEVEL_INFO
#define KOSY_LOG_LOGGER_INFO(logger, ...) \
    KOSY_LOG_LOGGER_CALL(logger, spdlog::level::info, __VA_ARGS__)
#define KOSY_LOG_INFO(...) KOSY_LOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_INFO(logger, ...) (void)0
#define KOSY_LOG_INFO(...) (void)0
#endif

#if KOSY_LOG_ACTIVE_LEVEL <= KOSY_LOG_LEVEL_WARN
#define KOSY_LOG_LOGGER_WARN(logger, ...) \
    KOSY_LOG_LOGGER_CALL(logger, spdlog::level::warn, __VA_ARGS__)
#define KOSY_LOG_WARN(...) KOSY_LOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_WARN(logger, ...) (void)0
#define KOSY_LOG_WARN(...) (void)0
#endif

#if KOSY_LOG_ACTIVE_LEVEL <= KOSY_LOG_LEVEL_ERROR
#define KOSY_LOG_LOGGER_ERROR(logger, ...) \
    KOSY_LOG_LOGGER_CALL(logger, spdlog::level::err, __VA_ARGS__)
#define KOSY_LOG_ERROR(...) KOSY_LOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_ERROR(logger, ...) (void)0
#define KOSY_LOG_ERROR(...) (void)0
#endif

#if KOSY_LOG_ACTIVE_LEVEL <= KOSY_LOG_LEVEL_CRITICAL
#define KOSY_LOG_LOGGER_CRITICAL(logger, ...) \
    KOSY_LOG_LOGGER_CALL(logger, spdlog::level::critical, __VA_ARGS__)
#define KOSY_LOG_CRITICAL(...) KOSY_LOG_LOGGER_CRITICAL(spdlog::default_logger_raw(), __VA_ARGS__)
#else
#define KOSY_LOG_LOGGER_CRITICAL(logger, ...) (void)0
#define KOSY_LOG_CRITICAL(...) (void)0
#endif

#define KOSY_LOG_LOGGER_CALL_IF(logger, level, condition, ...) \
    (condition) ? KOSY_LOG_LOGGER_CALL(logger, level, __VA_ARGS__) : (void)0

#define KOSY_LOG_LOGGER_TRACE_IF(logger, condition, ...) \
    KOSY_LOG_LOGGER_CALL_IF(logger, spdlog::level::trace, condition, __VA_ARGS__)

#define KOSY_LOG_LOGGER_DEBUG_IF(logger, condition, ...) \
    KOSY_LOG_LOGGER_CALL_IF(logger, spdlog::level::debug, condition, __VA_ARGS__)

#define KOSY_LOG_LOGGER_INFO_IF(logger, condition, ...) \
    KOSY_LOG_LOGGER_CALL_IF(logger, spdlog::level::info, condition, __VA_ARGS__)

#define KOSY_LOG_LOGGER_WARN_IF(logger, condition, ...) \
    KOSY_LOG_LOGGER_CALL_IF(logger, spdlog::level::warn, condition, __VA_ARGS__)

#define KOSY_LOG_LOGGER_ERROR_IF(logger, condition, ...) \
    KOSY_LOG_LOGGER_CALL_IF(logger, spdlog::level::err, condition, __VA_ARGS__)

#define KOSY_LOG_LOGGER_CRITICAL_IF(logger, condition, ...) \
    KOSY_LOG_LOGGER_CALL_IF(logger, spdlog::level::critical, condition, __VA_ARGS__)

#define KOSY_LOG_TRACE_IF(condition, ...) \
    KOSY_LOG_LOGGER_TRACE_IF(spdlog::default_logger_raw(), condition, __VA_ARGS__)

#define KOSY_LOG_DEBUG_IF(condition, ...) \
    KOSY_LOG_LOGGER_DEBUG_IF(spdlog::default_logger_raw(), condition, __VA_ARGS__)

#define KOSY_LOG_INFO_IF(condition, ...) \
    KOSY_LOG_LOGGER_INFO_IF(spdlog::default_logger_raw(), condition, __VA_ARGS__)

#define KOSY_LOG_WARN_IF(condition, ...) \
    KOSY_LOG_LOGGER_WARN_IF(spdlog::default_logger_raw(), condition, __VA_ARGS__)

#define KOSY_LOG_ERROR_IF(condition, ...) \
    KOSY_LOG_LOGGER_ERROR_IF(spdlog::default_logger_raw(), condition, __VA_ARGS__)

#define KOSY_LOG_CRITICAL_IF(condition, ...) \
    KOSY_LOG_LOGGER_CRITICAL_IF(spdlog::default_logger_raw(), condition, __VA_ARGS__)

namespace kosy {

using Logger = spdlog::logger;
using LogLevel = spdlog::level::level_enum;

[[using gnu: pure]]
inline auto makeLogger(std::string_view name, std::string_view log_file = "")
    -> std::shared_ptr<Logger> {
    spdlog::init_thread_pool(8192, 1);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] %@ %v");

    if (log_file.empty()) {
        return std::make_shared<spdlog::async_logger>(
            name.data(), console_sink, spdlog::thread_pool()
        );
    }

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file.data(), true);
    file_sink->set_level(spdlog::level::trace);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [thread %t] %@ %v");

    return std::make_shared<spdlog::async_logger>(
        name.data(), spdlog::sinks_init_list{console_sink, file_sink}, spdlog::thread_pool()
    );
}

[[using gnu: pure, always_inline]] [[nodiscard]]
inline auto getLogger(std::string_view name) -> std::shared_ptr<Logger> {
    return spdlog::get(name.data());
}

[[using gnu: always_inline]]
inline auto dropLogger(std::string_view name) -> void {
    spdlog::drop(name.data());
}

[[using gnu: always_inline]]
inline auto dropAllLogger() -> void {
    spdlog::drop_all();
}

[[using gnu: always_inline]]
inline auto registerLogger(std::shared_ptr<Logger> logger) -> void {
    spdlog::register_logger(std::move(logger));
}

[[using gnu: always_inline]]
inline auto setDefaultLogger(std::shared_ptr<Logger> logger) -> void {
    spdlog::set_default_logger(std::move(logger));
}

[[using gnu: always_inline]] [[nodiscard]]
inline auto getDefaultLogger() -> std::shared_ptr<Logger> {
    return spdlog::default_logger();
}

} // namespace kosy
