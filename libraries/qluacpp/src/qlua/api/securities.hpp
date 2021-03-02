#pragma once

#include "../structs/function_results.hpp"

// getClassSecurities - функция для получения списка кодов бумаг для списка классов, заданного списком кодов 
QLUACPP_DETAIL_API_FUNCTION4(const char*,
                             getClassSecurities,
                             const char*, class_name)

