#pragma once

unsigned int message(const std::string& text) const { return api::message(text.c_str()); }
unsigned int message(const char* text) const { return api::message(text); }