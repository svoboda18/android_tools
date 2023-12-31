/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdint.h>

typedef unsigned int uid_t;
typedef unsigned int gid_t;

// Binary format for the runtime <partition>/etc/fs_config_(dirs|files) filesystem override files.
struct fs_path_config_from_file {
    uint16_t len;
    uint16_t mode;
    uint16_t uid;
    uint16_t gid;
    uint64_t capabilities;
    char prefix[];
} __attribute__((__aligned__(sizeof(uint64_t))));

struct fs_path_config {
    unsigned mode;
    unsigned uid;
    unsigned gid;
    uint64_t capabilities;
    const char* prefix;
};
