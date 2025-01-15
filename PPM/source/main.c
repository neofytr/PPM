#include "ppm.h"

int main(void)
{
    const size_t width = 800;
    const size_t height = 600;
    uint32_t pixels[width * height];

    

    int32_t result = save_raw_ppm_file(pixels, width, height, "test.ppm");
    if (result != 0)
    {
        fprintf(stderr, "Failed to save PPM file\n");
        return 1;
    }

    printf("PPM file saved successfully\n");
    return 0;
}