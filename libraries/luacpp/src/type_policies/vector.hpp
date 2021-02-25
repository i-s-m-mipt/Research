#pragma once

#include <vector>

namespace lua {
  template <typename T>
  struct type_policy<vector_element<T>> {
    typedef T write_type;
    typedef T read_type;

    static inline bool type_matches(lua::State s, const int idx) {
      return s.is_table(idx); // Boxed type is not checked
    }
      
    static inline read_type get_unsafe(lua::State s, const int idx, const int i) {
      s.raw_get_field(idx, i);
      auto rslt = entity<type_policy<read_type>>(s, -1).get();
      s.pop();
      return rslt;
    }

    static inline void apply_unsafe(lua::State s, const int idx, std::function<void(const lua::State&, int)> f,
                                    const int i) {
      s.raw_get_field(idx, i);
      f(s, idx);
      s.pop();
    }

    static inline void set(lua::State s, const int idx, write_type value, const int i)  {
      if (type_matches(s, idx)) {
        s.push<>(i);
        s.push<>(value);
        s.set_table(idx - 2);
      } else {
        throw std::runtime_error("Luacpp: Can't create table field from non-table lua variable in stack");
      }
    }
  };

  template <typename T>
  struct type_policy<std::vector<T, std::allocator<T>>> {
    typedef std::vector<T, std::allocator<T>> write_type;
    typedef ::lua::vector<T> read_type;

    static inline bool type_matches(lua::State s, int idx) {
      return s.is_nil(idx) || s.is_table(idx); 
    }
      
    static inline read_type get_unsafe(lua::State s, int idx) {
      return read_type(s, idx).get();
    }

    static inline void apply_unsafe(lua::State s, int idx, std::function<void(const lua::State&, int)> f) {
      f(s, idx);
    }

    static inline void set(lua::State s, int idx, const write_type& value) {
      throw std::runtime_error("Luacpp: setting vectors is not implemented");
    }
  };

  template <typename T>
  struct type_policy<::lua::vector<T>> {
    typedef ::lua::vector<T> write_type;
    typedef ::lua::vector<T> read_type;

    static inline bool type_matches(lua::State s, int idx) {
      return s.is_nil(idx) || s.is_table(idx); 
    }
      
    static inline read_type get_unsafe(lua::State s, int idx) {
      return read_type(s, idx);
    }

    static inline void apply_unsafe(lua::State s, int idx, std::function<void(const lua::State&, int)> f) {
      f(s, idx);
    }

    static inline void set(lua::State s, int idx, write_type value)  {
      throw std::runtime_error("Luacpp: etting lua vectors is not implemented");
    }
  };
  
  
}
