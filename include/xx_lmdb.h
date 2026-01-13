#pragma once
#include "xx_includes.h"
#include <lmdb.h>

namespace xx {

    struct LMDB {
        MDB_env* env{};
        MDB_dbi dbi{};
        MDB_val key{}, value{};
        MDB_txn* txn{};
        int r{ MDB_SUCCESS };
        operator int() const{ return r; }

        LMDB() = default;
        LMDB(LMDB const&) = delete;
        LMDB& operator=(LMDB const&) = delete;

        ~LMDB();

        // return 0 mean success
        int32_t Init(std::filesystem::path const& dir_);

        XX_INLINE bool NotFound() const {
            return r == MDB_NOTFOUND;
        }

        // return empty mean error. need check if r == MDB_NOTFOUND
        std::string_view Load(std::string_view k_);

        // return 0 mean success
        int32_t Save(std::string_view k_, std::string_view v_);
        
        // ...
    };

}
