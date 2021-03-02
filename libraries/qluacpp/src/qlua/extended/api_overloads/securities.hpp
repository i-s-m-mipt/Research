#pragma once

const char* getClassSecurities(const std::string& class_name) const {
  return api::getClassSecurities(class_name.c_str());
}

const char* getClassSecurities(const char* class_name) const {
  return api::getClassSecurities(class_name);
}
