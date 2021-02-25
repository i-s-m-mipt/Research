#pragma once

// level2_quotes_record
namespace qlua {
  namespace table {
    LUACPP_STATIC_TABLE_BEGIN(level2_quotes_lua_record)
    LUACPP_TABLE_FIELD(price, std::string)
    LUACPP_TABLE_FIELD(quantity, std::string)
    LUACPP_STATIC_TABLE_END()
  }
}
LUACPP_STATIC_TABLE_TYPE_POLICY(::qlua::table::level2_quotes_lua_record)

namespace qlua {
  namespace table {
    struct level2_quotes_record {
      const std::string price;
      const std::string quantity;
    };
    
    struct level2_quotes {
      level2_quotes(const lua::State& l, const size_t idx) :
        l_(l), idx_(int(idx)) {
        if (!l_.is_table(idx_)) {
          throw std::runtime_error("level2_quotes: can't create, not a table");
        }
      }

      const std::string bid_count() const {
        l_.push<const char*>("bid_count");
        l_.raw_get(idx_ - 1);
        auto rslt = ::lua::entity<::lua::type_policy<const std::string>>(l_, -1).get();
        l_.pop(1);
        return rslt;
      }

      const std::string offer_count() const {
        l_.push<const char*>("offer_count");
        l_.raw_get(idx_ - 1);
        auto rslt = ::lua::entity<::lua::type_policy<const std::string>>(l_, -1).get();
        l_.pop(1);
        return rslt;
      }
      
      const std::vector<level2_quotes_record> offer() const {
        return get_table("offer");
      }

      const std::vector<level2_quotes_record> bid() const {
        return get_table("bid");
      }
      
    private:
      const lua::State& l_;
      const int idx_;

      const std::vector<level2_quotes_record> get_table(const char* name) const {
        std::vector<level2_quotes_record> rslt;
        l_.push<const char*>(name);
        l_.raw_get(idx_ - 1);
        if (l_.is_table(-1)) {
          auto sz = l_.raw_len(-1);
          for (int i = 0; i < sz; ++i) {
            l_.raw_get_field(-1, i + 1);
            auto rec = ::lua::entity<::lua::type_policy<::qlua::table::level2_quotes_lua_record>>(l_, -1)();
            rslt.push_back({rec.price(), rec.quantity()});
            l_.pop(1);
          }
        } else {
          throw std::runtime_error("level2_quote get record table failed: not a table");
        }
        l_.pop(1);
        return rslt;
      }
    };
  }
}
