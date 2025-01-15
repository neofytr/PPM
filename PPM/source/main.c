#include "ppm.h"

#define MAX_HEADER_SIZE 1024

int32_t save_raw_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path)
{
    if (!pixels || width == 0 || height == 0 || !file_path)
    {
        return -1;
    }

    FILE *fp = fopen(file_path, "wb");
    if (!fp)
    {
        perror("fopen");
        return -1;
    }

    char header[MAX_HEADER_SIZE];
    int header_len = snprintf(header, MAX_HEADER_SIZE, "P6\n%zu %zu\n255\n", width, height);
    if (header_len < 0 || header_len >= MAX_HEADER_SIZE)
    {
        perror("snprintf");
        fclose(fp);
        return -1;
    }

    if (fwrite(header, 1, header_len, fp) != (size_t)header_len)
    {
        perror("fwrite");
        fclose(fp);
        return -1;
    }

    // buffer to store RGB values for each pixel
    uint8_t rgb[3];

    for (size_t i = 0; i < width * height; i++)
    {
        uint32_t pixel = pixels[i];
        // extract RGB components (0x00RRGGBB format)
        rgb[0] = (pixel >> 8 * 2) & 0xFF; // Red
        rgb[1] = (pixel >> 8 * 1) & 0xFF; // Green
        rgb[2] = (pixel >> 8 * 0) & 0xFF; // Blue

        if (fwrite(rgb, 1, 3, fp) != 3)
        {
            perror("fwrite");
            fclose(fp);
            return -1;
        }
    }

    if (fclose(fp) != 0)
    {
        perror("fclose");
        return -1;
    }

    return 0;
}

int main(void)
{
    const size_t width = 800;
    const size_t height = 600;
    uint32_t pixels[width * height];

    uint8_t max_color_value = 255;

    for (size_t counter = 0; counter < width * height; counter++)
    {
        size_t x = counter % width;
        size_t y = counter / width;

        uint8_t component[4];

        component[3] = 0x00;
        component[2] = (uint8_t)((x / (float)width) * max_color_value); // integer division would just make x / width 0 for all x < width
        component[1] = (uint8_t)(((x + y) / (float)(width + height)) * max_color_value);
        component[0] = (uint8_t)((y / (float)height) * max_color_value);

        uint32_t pixel = ((uint32_t)component[3] << 24) |
                         ((uint32_t)component[2] << 16) |
                         ((uint32_t)component[1] << 8) |
                         ((uint32_t)component[0]);

        // this can be done via uint32_t pixel = *(uint32_t *)component; but this results in unaligned memory access

        pixels[counter] = pixel;
    }

    int32_t result = save_raw_ppm_file(pixels, width, height, "test.ppm");
    if (result != 0)
    {
        fprintf(stderr, "Failed to save PPM file\n");
        return 1;
    }

    printf("PPM file saved successfully\n");
    return 0;
}