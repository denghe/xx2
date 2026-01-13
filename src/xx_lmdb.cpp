#pragma once
#include "xx_lmdb.h"

namespace xx {

    LMDB::~LMDB() {
        mdb_env_close(env);
        //if (r != MDB_SUCCESS) xx::CoutN("mdb error: ", mdb_strerror(r));
    }

    // return 0 mean success
    int32_t LMDB::Init(std::filesystem::path const& dir_) {
        auto u8s = dir_.u8string();
        auto path = (const char*)u8s.c_str();
        if (r = mdb_env_create(&env); r != MDB_SUCCESS) return __LINE__;
        if (r = mdb_env_set_mapsize(env, 1024 * 1024 * 8); r != MDB_SUCCESS) return __LINE__;
        if (r = mdb_env_set_maxreaders(env, 1); r != MDB_SUCCESS) return __LINE__;
        if (r = mdb_env_set_maxdbs(env, 1); r != MDB_SUCCESS) return __LINE__;
        if (r = mdb_env_open(env, path, 0, 0664); r != MDB_SUCCESS) return __LINE__;
        return 0;
    }

    // return empty mean error. need check if r == MDB_NOTFOUND
    std::string_view LMDB::Load(std::string_view k_) {
        if (r = mdb_txn_begin(env, nullptr, 0, &txn); r != MDB_SUCCESS) return {};
        auto sg_txn = xx::MakeScopeGuard([&] {
            mdb_txn_abort(txn);
            });
        if (r = mdb_dbi_open(txn, nullptr, 0, &dbi); r != MDB_SUCCESS) return {};
        key.mv_size = k_.size();
        key.mv_data = (void*)k_.data();
        if (r = mdb_get(txn, dbi, &key, &value); r != MDB_SUCCESS) return {};
        return { (char const*)value.mv_data, value.mv_size };
    }

    // return 0 mean success
    int32_t LMDB::Save(std::string_view k_, std::string_view v_) {
        if (r = mdb_txn_begin(env, nullptr, 0, &txn); r != MDB_SUCCESS) return __LINE__;
        auto sg_txn = xx::MakeScopeGuard([&] {
            mdb_txn_abort(txn);
            });
        if (r = mdb_dbi_open(txn, nullptr, 0, &dbi); r != MDB_SUCCESS) return __LINE__;
        key.mv_size = k_.size();
        key.mv_data = (void*)k_.data();
        value.mv_size = v_.size();
        value.mv_data = (void*)v_.data();
        if (r = mdb_put(txn, dbi, &key, &value, 0); r != MDB_SUCCESS) return __LINE__;
        if (r = mdb_txn_commit(txn); r != MDB_SUCCESS) return __LINE__;
        sg_txn.Cancel();
        return 0;
    }
        
    // ...
}
