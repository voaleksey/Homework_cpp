#include <algorithm>
#include <cstdint>
#include <stdio.h>
#include <vector>

struct __attribute__ ((__packed__)) BMPHeader{             // Total: 54 bytes
  uint16_t  type;             // Magic identifier: 0x4d42
  uint32_t  size;             // File size in bytes
  uint16_t  reserved1;        // Not used
  uint16_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint16_t  num_planes;       // Number of color planes
  uint16_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
};

std::vector<uint8_t> read_header(FILE* in, BMPHeader& header) {
    fread((void*) &header, 1, sizeof(header), in);
    char* ptr = reinterpret_cast<char*>(&header); 
    std::vector<uint8_t> end;
    int end_size = header.dib_header_size - sizeof(header);
    end.resize(end_size > 0 ? end_size : 0);
    if (end_size > 0)
        fread((void*) end.data(), 1, end_size, in);
    return end;
}

struct __attribute__ ((__packed__)) pixel {
    uint8_t b = 0;
    uint8_t g = 0;
    uint8_t r = 0;
};

using picture = std::vector<std::vector<pixel>>;
void read_pixels(FILE* in, picture& pic, BMPHeader& header) {
    const int size = header.size;
    const int w = header.width_px;
    const int h = header.height_px;
    const int padding = (4 - (w * 3) % 4) % 4;
    pic.reserve(std::max(w, h));
    pic.resize(h);
    for (auto& row : pic) {
        row.reserve(std::max(w, h));
        row.resize(w);
    }
    for (int i = 0; i < pic.size(); ++i) {
        fread((void*) pic[i].data(), 3, pic[i].size(), in);
       for (int j = 0; j < padding; ++j)
            fgetc(in);
    }
}

void output_file(const char* output_name, const picture& pic, const BMPHeader& header, const std::vector<uint8_t>& header_end) {
    FILE* out_file = fopen(output_name, "w");
    fwrite((void*) &header, 1, sizeof(header), out_file);
    fwrite((void*) header_end.data(), 1, header_end.size(), out_file);
    const int w = header.width_px;
    const int h = header.height_px;
    const int padding = (4 - (w * 3) % 4) % 4;
    for (int i = 0; i < h; ++i) {
        fwrite((void*) pic.data(), 3, pic[i].size(), out_file);
        for (int j = 0; j < padding; ++j)
            fputc(0, out_file);
    }
    fclose(out_file);
}

void rotate_right(const picture& pic, const BMPHeader& header, const std::vector<uint8_t>& header_end) {
    FILE* rr_file = fopen("rotate_right.bmp", "w");
    BMPHeader new_header = header;
    const int w = new_header.width_px = header.height_px;
    const int h = new_header.height_px = header.width_px;
    const int padding = (4 - (w * 3) % 4) % 4;
    new_header.size = w * h * 3 + sizeof(header) + header_end.size(); 
    fwrite((void*) &new_header, 1, sizeof(new_header), rr_file);
    fwrite((void*) header_end.data(), 1, header_end.size(), rr_file);
    for (int i = h - 1; i >= 0 ; --i) {
        for (int j = 0; j < w; ++j) {
            const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&pic[j][i]);
            for (int i = 0; i < 3; ++i)
                fputc(*(ptr++), rr_file);
        }
        for (int j = 0; j < padding; ++j)
            fputc(0, rr_file);
    }
    fclose(rr_file);
}


BMPHeader rotate_left(const picture& pic, const BMPHeader& header, const std::vector<uint8_t>& header_end) {
    FILE* rl_file = fopen("rotate_left.bmp", "w");
    BMPHeader new_header = header;
    const int w = new_header.width_px = header.height_px;
    const int h = new_header.height_px = header.width_px;
    const int padding = (4 - (w * 3) % 4) % 4;
    new_header.size = w * h * 3 + sizeof(header) + header_end.size(); 
    fwrite((void*) &new_header, 1, sizeof(new_header), rl_file);
    fwrite((void*) header_end.data(), 1, header_end.size(), rl_file);
    for (int i = 0; i < h; ++i) {
        for (int j = w - 1; j >= 0; --j) {
            const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&pic[j][i]);
            for (int i = 0; i < 3; ++i)
                fputc(*(ptr++), rl_file);
        }
        for (int j = 0; j < padding; ++j)
            fputc(0, rl_file);
    }
    fclose(rl_file);
    return new_header;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Provide input and output file\n");
    }
    const char* in = argv[1];
    const char* out = argv[2];
    FILE* ifile = fopen(in, "rb");
    if (!ifile) {
        printf("%s\n", "ERROR: input file not opened");
        return 1;
    }
    BMPHeader header;
    std::vector<uint8_t> header_end = read_header(ifile, header);
    printf("h:%d w:%d\nsize of file:%d\ninfo_header_size:%d\n",
		    header.height_px,
		    header.width_px,
		    header.size,
		    header.dib_header_size);
    std::vector<std::vector<pixel>> pic;
    read_pixels(ifile, pic, header);
    printf("%d\n", header.dib_header_size);
    fclose(ifile);
    output_file(out, pic, header, header_end);
    rotate_right(pic, header, header_end);
    BMPHeader new_header = rotate_left(pic, header, header_end);
}

