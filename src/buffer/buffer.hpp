#pragma once

#include <cstdint>

#include <sstream>
#include <string>
#include <vector>

namespace ShiraNet::NetworkData {

    struct Buffer {
        unsigned int size = 0;
        std::string data{ 0 };
        Buffer(int Size, std::string Data = "") {
            size = Size;
            data = Data;
            data.resize(Size);
        }
    };

    template<typename TemplateData>
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

        template<typename T>
        Message(uint32_t ID, DataField<T> Data) { // Sets payload data and size automagically
            id = ID;
            dataFieldToPayload(Data);
        };

        template<typename T>
        Message(uint32_t ID, T Data) { // Sets payload data and size automagically
            id = ID;
            DataField<T> dataField;
            dataField.data = Data;
            dataField.size = sizeof(Data);
            dataFieldToPayload(dataField);
        };

        Message(uint32_t ID, std::string Data) { // Sets payload data and size automagically
            id = ID;
            payload = Data;
            payloadSize = payload.size();
        };

        // TODO: clean ts up

        template<typename T>
        struct is_std_vector : std::false_type {};

        template<typename T, typename Alloc>
        struct is_std_vector<std::vector<T, Alloc>> : std::true_type {};

        template<typename T>
        void dataFieldToPayload(DataField<T> Data) { // I hate that im not allowed to have this in a .cpp file
            std::ostringstream outputStream;

            if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>) {
                outputStream << Data.data;
            } else if constexpr (is_std_vector<T>::value) {
                outputStream << Data.data.size() << ' ';
                
                using Eliminator = typename T::value_type;
                if constexpr (std::is_same_v<Eliminator, bool>) {
                    for (bool item : Data.data) {
                        outputStream << item << ' ';
                    }
                } else {
                    for (auto& item : Data.data) {
                        if constexpr (std::is_arithmetic_v<Eliminator> || std::is_same_v<Eliminator, std::string>) {
                            outputStream << item << ' ';
                        }
                        else {
                            item.serialize(outputStream);
                        }
                    }
                }
            } else {
                Data.data.serialize(outputStream);
            }

            payload = outputStream.str();
            payloadSize = payload.size();
        }

        template<typename T>
        void payloadToDataField(DataField<T>& Data) {
            std::istringstream inputStream(payload);

            if constexpr (std::is_arithmetic_v<T> || std::is_same_v<T, std::string>) {
                inputStream >> Data.data;
            } else if constexpr (is_std_vector<T>::value) {
                using Eliminator = typename T::value_type;

                size_t size;
                inputStream >> size;

                Data.data.resize(size);

               if constexpr (std::is_same_v<Eliminator, bool>) {
                    for (auto item : Data.data) {
                        bool temp;
                        inputStream >> temp;
                        item = temp;
                    }
                } else {
                    for (auto& item : Data.data) {
                        if constexpr (std::is_arithmetic_v<Eliminator> || std::is_same_v<Eliminator, std::string>) {
                            inputStream >> item;
                        } 
                        else {
                            item.deserialize(inputStream);
                        }
                    }
                }
            } else {
                Data.data.deserialize(inputStream);
            }
        }
    };

} // namespace ShiraNet::NetworkData
