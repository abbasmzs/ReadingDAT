#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>

struct PRFData {
    struct Header {
        uint16_t prf_angle;
        uint16_t prf_number;
        uint16_t video_length;
        uint16_t short_range_offset;
    } header;
    std::vector<float> i;
    std::vector<float> q;
};

int main(int argc, char** argv) {
    const int MAX_PRF_ANGLE = 8192;
    const int size = 2600;
    std::ifstream file("File_path", std::ios::in | std::ios::binary);

    std::vector<PRFData> prfData(MAX_PRF_ANGLE);

    for (int k = 0; k < MAX_PRF_ANGLE; ++k) {
        auto& prf = prfData[k];

        // read headers
        uint16_t head;
        file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
        prf.header.prf_angle = head;
        file.read(reinterpret_cast<char*>(&head), sizeof(uint16_t));
        prf.header.prf_number = head;
        prf.header.video_length = size;
        prf.header.short_range_offset = 100;

        // read video i
        std::vector<int16_t> data(size);
        file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
        prf.i.resize(size);
        for (int i = 0; i < size; i++) {
            prf.i[i] = static_cast<float>(data[i]);
        }

        // read video q
        file.read(reinterpret_cast<char*>(data.data()), sizeof(int16_t) * size);
        prf.q.resize(size);
        for (int i = 0; i < size; i++) {
            prf.q[i] = static_cast<float>(data[i]);
        }

        // skip other data
        file.seekg(sizeof(int16_t) * size, std::ios::cur);
    }

    // do something with prfData
    for (int k = 0; k < 10; ++k) {
        const auto& prf = prfData[k];
        std::cout << "PRF angle: " << prf.header.prf_angle << std::endl;
        std::cout << "PRF number: " << prf.header.prf_number << std::endl;
        std::cout << "Video length: " << prf.header.video_length << std::endl;
        std::cout << "Short range offset: " << prf.header.short_range_offset << std::endl;
        std::cout << "i[0]: " << prf.i[55] << std::endl;
        std::cout << "i[1]: " << prf.i[1000] << std::endl;
        std::cout << "i[2]: " << prf.i[24] << std::endl;
        std::cout << "q[0]: " << prf.q[2500] << std::endl;
        std::cout << "q[1]: " << prf.q[156] << std::endl;
        std::cout << "q[2]: " << prf.q[23] << std::endl;
    }

    return 0;
}