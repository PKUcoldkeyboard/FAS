#pragma once
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO //控制记录日志的级别
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

//以下宏spdlog已经定义， 日志输出到控制台，包括打印文件名和行号
//SPDLOG_DEBUG(...)
//SPDLOG_INFO(...)
//SPDLOG_WARN(...)
//SPDLOG_ERROR(...)
//SPDLOG_CRITICAL(...)

//以下宏新定义， 日志输出到旋转日志文件，包括打印文件名和行号
constexpr auto LOGGER_NAME = "rotating sys log";
constexpr auto LOGGER_SAVE_PATH = "logs/rotating.txt";
#define SPDLOG_INFO_FILE(...) \
auto logger0 = spdlog::get(LOGGER_NAME); \
if (nullptr == logger0) \
{ \
	auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
	SPDLOG_LOGGER_INFO(rotating_logger, __VA_ARGS__);\
} \
else \
{ \
	SPDLOG_LOGGER_INFO(logger0, __VA_ARGS__); \
}

#define SPDLOG_WARN_FILE(...) \
auto logger1 = spdlog::get(LOGGER_NAME); \
if (nullptr == logger1) \
{ \
	auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
	SPDLOG_LOGGER_WARN(rotating_logger, __VA_ARGS__);\
} \
else \
{ \
	SPDLOG_LOGGER_WARN(logger1, __VA_ARGS__); \
}

#define SPDLOG_ERROR_FILE(...) \
auto logger2 = spdlog::get(LOGGER_NAME); \
if (nullptr == logger2) \
{ \
	auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
	SPDLOG_LOGGER_ERROR(rotating_logger, __VA_ARGS__);\
} \
else \
{ \
	SPDLOG_LOGGER_ERROR(logger2, __VA_ARGS__); \
}

#define SPDLOG_CRITICAL_FILE(...) \
auto logger3 = spdlog::get(LOGGER_NAME); \
if (nullptr == logger3) \
{ \
	auto rotating_logger = spdlog::rotating_logger_mt(LOGGER_NAME, LOGGER_SAVE_PATH, 1048576 * 5, 3); \
	SPDLOG_LOGGER_CRITICAL(rotating_logger, __VA_ARGS__);\
} \
else \
{ \
	SPDLOG_LOGGER_CRITICAL(logger3, __VA_ARGS__); \
}
