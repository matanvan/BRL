#pragma once

#ifndef UMDF_USING_NTSTATUS
#define UMDF_USING_NTSTATUS
#endif //UMDF_USING_NTSTATUS

#include <Windows.h>
#include <ntstatus.h>
#include <cstdint>
#include <chrono>
#include <string>
#include <memory>
#include <vector>
#include <memory>
#include <functional>
#include <optional>
#include <variant>

using Buffer = std::vector<uint8_t>;
