#pragma once

#include <cstdint>

#include <sstream>
#include <string>

namespace ShiraNet::NetworkData {

struct Buffer {
    unsigned int size = 0;
    std::string data{0};
    Buffer(int Size, std::string Data = "") {
        size = Size;
        data = Data;
        data.resize(Size);
    }
};

template <typename TemplateData>
struct DataField {
    unsigned int size = 0;
    TemplateData data;
};

struct Message {
    uint32_t id = 0;
    uint32_t payloadSize = 0;
    std::string payload = "";

    Message(uint32_t ID, uint32_t PayloadSize) {
        id = ID;
        payloadSize = PayloadSize;
    };

    Message(uint32_t ID) {
        id = ID;
    };

    template <typename T>
    Message(uint32_t ID, DataField<T> Data) {
        id = ID;
        dataFieldToPayload(Data);
    };

    Message(uint32_t ID, std::string Data) {
        id = ID;
        payload = Data;
        payloadSize = payload.size();
    };

    template <typename T>
    void dataFieldToPayload(DataField<T> Data) {
        std::ostringstream stream;

        stream.write(reinterpret_cast<const char *>(&Data.size), sizeof(Data.size));

        if constexpr (std::is_same_v<T, std::string>) {
            stream.write(Data.data.data(), Data.data.size());
        } else if constexpr (std::is_trivially_copyable_v<T>) {
            stream.write(reinterpret_cast<const char *>(&Data.data), sizeof(T));
        } else {
            Data.data.serialize(stream);
        }

        payload = stream.str();
        payloadSize = payload.size();
    }
};

} // namespace ShiraNet::NetworkData
