#pragma once

// DataSource (раздел "Функции для работы с графиками" qlua.chm)

#include "tables.hpp"

namespace qlua {
  struct data_source {
    struct time {
      unsigned int year;
      unsigned int month;
      unsigned int day;
      unsigned int week_day;
      unsigned int hour;
      unsigned int min;
      unsigned int sec;
      unsigned int ms;
      unsigned int count;
    };
    
    data_source(const lua::State& l,
                const char* class_code,  // код класса
                const char* sec_code,    // код бумаги
                const unsigned int interval, // интервал запрашиваемого графика, INTERVAL_*
                const char* param = nullptr // Необязательный параметр (что запрашивать)
                ) : l_(l) {
      auto f = [this] (const lua::State& l) -> int {
        // Check if CreateDataSource returned a table in first rslt
        if (l_.is_table(-2)) {
          // Copy the table
          l_.push_value(-2);
          // Create reference
          if (l_.is_table(-4)) {
            ref_key_ = luaL_ref(l_.state(), -4);
            if (ref_key_ != LUA_NOREF) {
              l_.pop(2);
              return 0;
            } else {
              l_.pop(1); // push_value
              throw std::runtime_error("Failed to create reference to datasource");
            }
          } else {
          }
        } else  {
          auto err = l_.at<const char*>(-1);
          std::string msg("Failed to create datasource, qlua returned: ");
          msg += err.get();
          l_.pop(2);
          throw std::runtime_error(msg.c_str());
        }
        return 0;
      };
      
      // Take references descriptor table
      l_.get_global(data_source::desc_table_name());
      if (l_.is_nil(-1)) {  // Create table if doesn't exist
        l_.pop(1);
        l_.new_table();
        l_.set_global(data_source::desc_table_name());
        l_.get_global(data_source::desc_table_name());
      }
      if (l_.is_table(-1)) {
        // Call CreateDataSource and handle the results
        if (param != nullptr) {
          l_.call_and_apply(f, 2, "CreateDataSource", class_code, sec_code, interval, param);
        } else {
          l_.call_and_apply(f, 2, "CreateDataSource", class_code, sec_code, interval);
        }
      } else {
        l_.pop(1); // get_global desc table
        throw std::runtime_error("Failed to create lua table " + std::string(data_source::desc_table_name()) + " in globals for datasource descriptor");
      }
    }

    double O(const unsigned int candle_idx) {
      return candle_method_call<double>("O", candle_idx);
    }

    double C(const unsigned int candle_idx) {
      return candle_method_call<double>("C", candle_idx);
    }
    
    double H(const unsigned int candle_idx) {
      return candle_method_call<double>("H", candle_idx);
    }
    
    double L(const unsigned int candle_idx) {
      return candle_method_call<double>("L", candle_idx);
    }
    
    double V(const unsigned int candle_idx) {
      return candle_method_call<double>("V", candle_idx);
    }
    
    // Time
    time T(const unsigned int candle_idx) const {
      if (ref_key_ == LUA_NOREF)
        throw std::runtime_error("Can't call datasource method: no reference to the object");

      l_.get_global(data_source::desc_table_name());
      auto i = 1;
      if (l_.is_table(-1)) {
        l_.push_number(ref_key_);
        l_.raw_get(-2);
        ++i;
        l_.push_string("T");
        l_.raw_get(-2); // Push function from table to stack
        ++i;
        if (l_.is_function(-1)) {
          l_.push_value(-2); // Push 'this'
          l_.push_number(candle_idx);
          const int nargs = 2;
          const int nres = 1;
          l_.pcall(nargs, nres, 0);
          i = i - 1 + nres; // 1 for function name popped by pcall
          auto t = l_.at<table::chart_time>(-1).get();
          time rslt =  {t.year.get(), t.month.get(), t.day.get(), t.week_day.get(), t.hour.get(), t.min.get(), t.sec.get(), t.ms.get(), t.count.get() };
          l_.pop(i);
          return rslt;
        } else {
          l_.pop(i);
          throw std::runtime_error("Call to datasource method failed: T method is not a function in object table");
        }
      } else {
        l_.pop(i); 
        throw std::runtime_error("Call to datasource method failed: " + std::string(data_source::desc_table_name()) + " is not a table in lua globals");
      }
    }

    bool SetUpdateCallback(const char* lua_function_name) {
      if (ref_key_ == LUA_NOREF)
        throw std::runtime_error("Can't call datasource method: no reference to the object");

      l_.get_global(data_source::desc_table_name());
      auto i = 1;
      if (l_.is_table(-1)) {
        l_.push_number(ref_key_);
        l_.raw_get(-2);
        ++i;
        l_.push_string("SetUpdateCallback");
        l_.raw_get(-2); // Push function from table to stack
        ++i;
        if (l_.is_function(-1)) {
          l_.push_value(-2); // Push 'this'
          ++i;
          l_.get_global(lua_function_name);
          ++i;
          if (l_.is_function(-1)) {
            l_.pcall(2, 1, 0);
            ++i;
            auto rslt = l_.at<bool>(-1).get();
            l_.pop(i - 3); // Function address and arguments
            return rslt;
          } else {
            throw std::runtime_error("Call to datasource method failed: " + std::string(lua_function_name) + " is not a function in lua globals");
          }
        } else {
          l_.pop(i);
          throw std::runtime_error("Call to datasource method failed: SetUpdateCallback method is not a function in object table");
        }
      } else {
        l_.pop(i); 
        throw std::runtime_error("Call to datasource method failed: " + std::string(data_source::desc_table_name()) + " is not a table in lua globals");
      }
    }

    bool SetEmptyCallback() {
      return ds_method_call<bool>("SetEmptyCallback");
    }

    unsigned int Size() {
      return ds_method_call<unsigned int>("Size");
    }
    
    bool Close() {
      return ds_method_call<bool>("Close");
    }

    static const char* desc_table_name() {
      static const char* s = "qluacpp_datasource_object_refs";
      return s;
    }

  private:
    lua::State l_;
    int ref_key_{LUA_NOREF};

    template <typename T>
    inline T candle_method_call(const char* name, const unsigned int candle_idx) const {
      if (ref_key_ == LUA_NOREF)
        throw std::runtime_error("Can't call datasource method: no reference to the object");
      l_.get_global(data_source::desc_table_name());
      auto i = 1; // Stack counter for items not cleaned by pcall; [desc_table]
      if (l_.is_table(-1)) {
        l_.push_number(ref_key_);
        l_.raw_get(-2);
        ++i; // [desc_table, ref]
        l_.push_string(name);
        l_.raw_get(-2); // Push function from table to stack
        ++i; // [desc_table, ref, func]
        if (l_.is_function(-1)) {
          l_.push_value(-2); // Push 'this'
          l_.push_number(candle_idx);
          const int nargs = 2;
          const int nres = 1;
          l_.pcall(nargs, nres, 0);
          i = i - 1 + nres; // 1 for function name popped by pcall
          auto rslt = l_.at<T>(-1).get();
          l_.pop(i);
          return rslt;
        } else {
          l_.pop(i);
          throw std::runtime_error("Call to datasource method failed: " + std::string(name) + " method is not a function in object table");
        }
      } else {
        l_.pop(i); 
        throw std::runtime_error("Call to datasource method failed: " + std::string(data_source::desc_table_name()) + " is not a table in lua globals");
      }
    }

    template <typename T>
    inline T ds_method_call(const char* name) const {
      if (ref_key_ == LUA_NOREF)
        throw std::runtime_error("Can't call datasource method: no reference to the object");
      l_.get_global(data_source::desc_table_name());
      auto i = 1; // Stack counter for items not cleaned by pcall; [desc_table]
      if (l_.is_table(-1)) {
        l_.push_number(ref_key_);
        l_.raw_get(-2);
        ++i; // [desc_table, ref]
        l_.push_string(name);
        l_.raw_get(-2); // Push function from table to stack
        ++i; // [desc_table, ref, func]
        if (l_.is_function(-1)) {
          l_.push_value(-2); // Push 'this'
          const int nargs = 1;
          const int nres = 1;
          l_.pcall(nargs, nres, 0);
          i = i - 1 + nres; // 1 for function name popped by pcall
          auto rslt = l_.at<T>(-1).get();
          l_.pop(i);
          return rslt;
        } else {
          l_.pop(i);
          throw std::runtime_error("Call to datasource method failed: " + std::string(name) + " method is not a function in object table");
        }
      } else {
        l_.pop(i); 
        throw std::runtime_error("Call to datasource method failed: " + std::string(data_source::desc_table_name()) + " is not a table in lua globals");
      }
    }

  };
  
}
