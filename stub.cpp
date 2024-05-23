#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>

struct DiscordYUVFrame {
  uint8_t* y;
  uint8_t* u;
  uint8_t* v;
  int32_t y_stride;
  int32_t u_stride;
  int32_t v_stride;
};

struct DiscordFrame {
  int64_t timestamp_us;
  union {
    DiscordYUVFrame yuv;
  } frame;
  int32_t width;
  int32_t height;
  int32_t type;
};

using DiscordFrameReleaseCB = void (*)(void*);

void releaseFrame(void* i_frame){
  DiscordFrame* frame = (DiscordFrame*)i_frame;
  delete[] frame->frame.yuv.y;
  delete[] frame->frame.yuv.u;
  delete[] frame->frame.yuv.v;
}

extern "C" void DeliverDiscordFrame(const char* streamId, const DiscordFrame& frame, DiscordFrameReleaseCB releaseCB, void* userData) {
  std::freopen("./frame.txt", "w", stdout);
  std::cout << "Stream ID: " << std::string(streamId) << std::endl;
  std::cout << "Release CB: " << releaseCB << std::endl;
  std::cout << "User Data: " << long(userData) << std::endl;
  std::cout << "US Timestamp: " << frame.timestamp_us << std::endl;
    std::ofstream y_file, u_file, v_file;
    y_file.open("./y.txt", std::ios::binary | std::ios::out);
    u_file.open("./u.txt", std::ios::binary | std::ios::out);
    v_file.open("./v.txt", std::ios::binary | std::ios::out);
    y_file.write(reinterpret_cast<char*>(frame.frame.yuv.y), frame.height * frame.width);
    u_file.write(reinterpret_cast<char*>(frame.frame.yuv.u), frame.height / 2 * frame.width / 2);
    v_file.write(reinterpret_cast<char*>(frame.frame.yuv.v), frame.height / 2 * frame.width / 2);
    y_file.close();
    u_file.close();
    v_file.close();
  std::cout << "Y stride: " << frame.frame.yuv.y_stride << std::endl;
  std::cout << "U sride: " << frame.frame.yuv.u_stride << std::endl;
  std::cout << "V stride: " << frame.frame.yuv.v_stride << std::endl;
  std::cout << "Width: " << frame.width << std::endl;
  std::cout << "Height: " << frame.height << std::endl;
  std::cout << "Type: " << frame.type << std::endl;
  fclose (stdout);
}
