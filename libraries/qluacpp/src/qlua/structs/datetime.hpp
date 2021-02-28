#pragma once

// date and datetime tables

#define QLUACPP_DATETIME_TABLE_FIELD( NAME )    \
  static constexpr const char _##NAME##_field_name[] = #NAME; \
  ::lua::Entity<detail::datetime_type_policy<_##NAME##_field_name>> NAME{m_state, m_index}; \


// --- Direct macro table generators ---

// date
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(date_)
    LUA_TABLE_FIELD(date, std::string)
    LUA_TABLE_FIELD(year, unsigned int)
    LUA_TABLE_FIELD(month, unsigned int)
    LUA_TABLE_FIELD(day, unsigned int)
    LUA_TABLE_END()
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::date_)

// datetime "Формат даты и времени, используемый таблицах"
namespace qlua {
  namespace table {
    LUA_TABLE_BEGIN(datetime)
    LUA_TABLE_FIELD(mcs, int)
    LUA_TABLE_FIELD(ms, int)
    LUA_TABLE_FIELD(sec, int)
    LUA_TABLE_FIELD(min, int)
    LUA_TABLE_FIELD(hour, int)
    LUA_TABLE_FIELD(day, int)
    LUA_TABLE_FIELD(week_day, int)
    LUA_TABLE_FIELD(month, int)
    LUA_TABLE_FIELD(year, int)
    LUA_TABLE_END()
  }
}
LUA_TABLE_TYPE_ADAPTER(::qlua::table::datetime)

// --- For nested tables ---

// datetime "Формат даты и времени, используемый таблицах"
namespace qlua {
  // Plain C structure to hold date_time
  struct c_date_time {
    int mcs;
    int ms;
    int sec;
    int min;
    int hour;
    int day;
    int week_day;
    int month;
    int year;
  };
  
  namespace table {
    namespace detail {

      template <const char* FieldName>
      struct datetime_type_policy {
        using write_type = const c_date_time&;
        using read_type = c_date_time;
        using type = c_date_time;

        static inline bool type_matches(lua::State s, int idx) {
          return s.is_table(idx); // Check that we're at a table
        }

        static inline read_type get_unsafe(lua::State s, int idx) {
          read_type rslt{0};
          int table_idx = idx;
          s.push<const char*>(FieldName);
          if (idx <= 0)
            s.get_table(idx - 1);
          else
            s.get_table(idx);
          if (s.is_table(-1)) {
            rslt.mcs = get_field<int>(s, "mcs");
            rslt.ms = get_field<int>(s, "ms");
            rslt.sec = get_field<int>(s, "sec");
            rslt.min = get_field<int>(s, "min");
            rslt.hour = get_field<int>(s, "hour");
            rslt.day = get_field<int>(s, "day");
            rslt.week_day = get_field<int>(s, "week_day");
            rslt.month = get_field<int>(s, "month");
            rslt.year = get_field<int>(s, "year"); 
          } else {
            s.pop();
            throw std::runtime_error("table does not have "+std::string(FieldName)+" table member");
          }
          s.pop();
          return rslt;
        }

        static inline void apply_unsafe(lua::State s, int idx, std::function<void(const lua::State&, int)> f) {
          throw std::runtime_error("apply_unsafe is not implemented for datetime");
        }

        static inline void set(lua::State s, int idx, c_date_time value) {
          throw std::runtime_error("set is not implemented for datetime");
        }
      
      private:
        template <typename T>
        static inline const T get_field(lua::State s, const char* name) {
          s.push<const char*>(name);
          s.raw_get(-2);
          auto rslt = lua::Entity<lua::Type_Adapter<T>>(s, -1).get();
          s.pop(1);
          return rslt;
        }
      
      };
      
    }
  }
}
