#include <cstdint>
#include <fstream>
#include <png.h>
#include <vector>

void extractDimensions(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("Width:") != std::string::npos) {
            width = std::stoi(line.substr(line.find(":") + 1));
        } else if (line.find("Height:") != std::string::npos) {
            height = std::stoi(line.substr(line.find(":") + 1));
        }
    }

    file.close();
}

template<typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

void YUVtoRGB(uint8_t y, uint8_t u, uint8_t v, uint8_t& r, uint8_t& g, uint8_t& b) {
    int32_t c = y - 16;
    int32_t d = u - 128;
    int32_t e = v - 128;
    
    r = clamp((298 * c + 409 * e + 128) >> 8, 0, 255);
    g = clamp((298 * c - 100 * d - 208 * e + 128) >> 8, 0, 255);
    b = clamp((298 * c + 516 * d + 128) >> 8, 0, 255);
}

int main() {
    int width, height;
    extractDimensions("frame.txt", width, height);

    std::ifstream y_file("./y.txt", std::ios::binary);
    std::ifstream u_file("./u.txt", std::ios::binary);
    std::ifstream v_file("./v.txt", std::ios::binary);

    std::vector<uint8_t> y_data(width * height);
    std::vector<uint8_t> u_data(width / 2 * height / 2);
    std::vector<uint8_t> v_data(width / 2 * height / 2);
    std::vector<uint8_t> rgb_data(width * height * 3);

    y_file.read(reinterpret_cast<char*>(y_data.data()), y_data.size());
    u_file.read(reinterpret_cast<char*>(u_data.data()), u_data.size());
    v_file.read(reinterpret_cast<char*>(v_data.data()), v_data.size());

    y_file.close();
    u_file.close();
    v_file.close();

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int y_index = i * width + j;
            int u_index = (i / 2) * (width / 2) + (j / 2);
            int v_index = (i / 2) * (width / 2) + (j / 2);

            uint8_t y = y_data[y_index];
            uint8_t u = u_data[u_index];
            uint8_t v = v_data[v_index];

            int rgb_index = (i * width + j) * 3;
            YUVtoRGB(y, u, v, rgb_data[rgb_index], rgb_data[rgb_index + 1], rgb_data[rgb_index + 2]);
        }
    }

    const char* filename = "frame.png";
    FILE* fp = fopen(filename, "wb");

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_bytep row_pointers[height];
    for (int i = 0; i < height; ++i) {
        row_pointers[i] = &rgb_data[i * width * 3];
    }

    png_set_rows(png, info, row_pointers);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, nullptr);
    png_destroy_write_struct(&png, &info);
    fclose(fp);

    return 0;
}