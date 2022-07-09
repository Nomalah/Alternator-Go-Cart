#pragma once

#pragma pack(push, 1)
struct message { // 3-bytes
    bool reverse; // 1-byte
    bool ebrake; // 1-byte
    char throttle; // 1-byte
};
#pragma pack(pop)

static_assert(sizeof(message) == 3, "message is not the size expected");