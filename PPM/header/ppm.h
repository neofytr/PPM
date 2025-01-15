#ifndef D671767E_C270_4118_85DF_6F60B484FB14
#define D671767E_C270_4118_85DF_6F60B484FB14

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// the PPM file created uses 1 byte color value (0 to 255) for each of RGB field.

int32_t save_raw_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path);
int32_t generate_gradient(uint32_t *pixels, size_t width, size_t height);

#endif /* D671767E_C270_4118_85DF_6F60B484FB14 */
