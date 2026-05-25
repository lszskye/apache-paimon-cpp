/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Assert utilities is adapted from RocksDB
// https://github.com/facebook/rocksdb/blob/main/test_util/testharness.h

#pragma once

#include <string>

#include "gtest/gtest.h"
#include "paimon/macros.h"
#include "paimon/result.h"
#include "paimon/status.h"

namespace paimon::test {

::testing::AssertionResult AssertStatus(const char* s_expr, const Status& s);

#define ASSERT_OK(expr)                                                                  \
    for (::paimon::Status _st = ::paimon::internal::GenericToStatus((expr)); !_st.ok();) \
    FAIL() << "'" PAIMON_STRINGIFY(expr) "' failed with " << _st.ToString()

#define ASSERT_NOK(expr)                                                                \
    for (::paimon::Status _st = ::paimon::internal::GenericToStatus((expr)); _st.ok();) \
    FAIL() << "'" PAIMON_STRINGIFY(expr) "' did not failed " << _st.ToString()

#define ASSERT_NOK_WITH_MSG(expr, error_msg)                                   \
    do {                                                                       \
        ::paimon::Status _st = ::paimon::internal::GenericToStatus((expr));    \
        ASSERT_TRUE(!_st.ok()) << "'" PAIMON_STRINGIFY(expr) "' did not fail"; \
        ASSERT_TRUE(_st.ToString().find(error_msg) != std::string::npos)       \
            << "'" PAIMON_STRINGIFY(expr) "' failed with " << _st.ToString()   \
            << " but did not contain expected message: " << error_msg;         \
    } while (0)

#define ASSIGN_OR_HANDLE_ERROR_IMPL(handle_error, status_name, lhs, rexpr) \
    auto&& status_name = (rexpr);                                          \
    handle_error(status_name.status());                                    \
    lhs = std::move(status_name).value();

#define ASSERT_OK_AND_ASSIGN(lhs, rexpr) \
    ASSIGN_OR_HANDLE_ERROR_IMPL(         \
        ASSERT_OK, PAIMON_ASSIGN_OR_RAISE_NAME(_error_or_value, __COUNTER__), lhs, rexpr);

#define EXPECT_OK_AND_ASSIGN(lhs, rexpr) \
    ASSIGN_OR_HANDLE_ERROR_IMPL(         \
        EXPECT_OK, PAIMON_ASSIGN_OR_RAISE_NAME(_error_or_value, __COUNTER__), lhs, rexpr);

#define EXPECT_OK(s) EXPECT_PRED_FORMAT1(paimon::test::AssertStatus, s)
#define EXPECT_NOK(s) EXPECT_FALSE((s).ok())

}  // namespace paimon::test
