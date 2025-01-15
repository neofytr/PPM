# YUV4MPEG2 Format Specification

## Overview

YUV4MPEG2 is a simple file format designed to hold uncompressed frames of YCbCr video. It supports multiple formats including YCbCr 4:2:0, YCbCr 4:2:2, and YCbCr 4:4:4, primarily intended for MPEG-2 encoding.

Note: Despite having "YUV" in its name, this format uses YCbCr color space (used in digital media) rather than YUV (used in analog PAL applications like TV and video tapes). This naming confusion is historically common in the industry.

## Color Matrix Specification

The format does not explicitly specify the color matrix used. While the original MJPEGTools documentation suggests BT.601 as the standard, this is not strictly enforced in practice. There is no widely-recognized extension field for indicating the matrix in use.

## File Structure

### 1. File Header

The file begins with a header containing:

#### Required Components:
- **Signature**: First 10 bytes must be "YUV4MPEG2" (including a trailing space, ASCII 0x20)
- **Parameters**: Following the signature, parameters are space-separated (ASCII 0x20)

#### Required Parameters:
- **Width**: `W` followed by integer (e.g., `W720`)
- **Height**: `H` followed by integer (e.g., `H480`)
- **Frame Rate**: `F` followed by frames/second as fraction:
  - `F30:1` - 30 FPS
  - `F25:1` - 25 FPS (PAL/SECAM)
  - `F24:1` - 24 FPS (Film)
  - `F30000:1001` - 29.97 FPS (NTSC)
  - `F24000:1001` - 23.976 FPS (Film-to-NTSC)

#### Optional Parameters:

1. **Interlacing** (`I`):
   - `Ip` - Progressive
   - `It` - Top field first
   - `Ib` - Bottom field first
   - `Im` - Mixed modes

2. **Pixel Aspect Ratio** (`A`):
   - `A0:0` - Unknown
   - `A1:1` - Square pixels
   - `A4:3` - NTSC-SVCD
   - `A4:5` - NTSC-DVD narrow-screen
   - `A32:27` - NTSC-DVD wide-screen

3. **Color Space** (`C`):
   - `C420jpeg` - 4:2:0 with biaxially-displaced chroma (0.5 right, 0.5 down)
   - `C420paldv` - 4:2:0 with coincident R and vertical B displacement
   - `C420mpeg2` - 4:2:0 with vertical chroma displacement
   - `C420` - 4:2:0 (interpretation varies by implementation)
   - `C422` - 4:2:2 coincident
   - `C444` - 4:4:4 coincident
   - `C444alpha` - 4:4:4 with alpha channel
   - `Cmono` - YCbCr plane only

4. **Extensions** (`X`):
   - General comments or extensions
   - FFMPEG-specific extensions:
     - `XCOLORRANGE=FULL` - JPEG/Full swing range
     - `XCOLORRANGE=LIMITED` - MPEG/Studio swing range
     - `XYSCSS` - Non-standard pixel formats

### 2. Frame Data

Each frame consists of:

1. **Frame Header**:
   - Starts with "FRAME"
   - Optional parameters (space-separated)
   - Ends with newline (0x0A)

2. **Raw Data**:
   - Planes ordered as Y-Cb-Cr
   - Frame size calculations:
     - 4:2:0: width × height × 3/2
     - 4:2:2: width × height × 2
     - 4:4:4: width × height × 3

## Color Space Conversion: RGB to YCbCr 4:4:4

### Standard Conversion (BT.601)

The conversion from RGB to YCbCr 4:4:4 uses these equations:

```
Y  =  0.299   × R + 0.587   × G + 0.114   × B
Cb = -0.16874 × R - 0.33126 × G + 0.5     × B + 128
Cr =  0.5     × R - 0.41869 × G - 0.08131 × B + 128
```

### Full Range vs Limited Range

#### Limited Range (TV/Studio Swing)
```
Y  = (219  × Y'/255) + 16   [16..235]
Cb = (224 × Cb'/255) + 16   [16..240]
Cr = (224 × Cr'/255) + 16   [16..240]
```

#### Full Range (JPEG/PC Swing)
```
Y  = Y'                     [0..255]
Cb = Cb'                    [0..255]
Cr = Cr'                    [0..255]
```

### Inverse Conversion (YCbCr to RGB)

For completeness, here's how to convert back to RGB:
```
R = Y + 1.402   × (Cr - 128)
G = Y - 0.34414 × (Cb - 128) - 0.71414 × (Cr - 128)
B = Y + 1.772   × (Cb - 128)
```

### Implementation Notes

1. All calculations are typically done in floating point and then rounded to the nearest integer
2. Results should be clamped to valid range (0-255 for full range, 16-235/240 for limited range)
3. Different standards (BT.601, BT.709, BT.2020) use different coefficients for the conversion
4. For 4:4:4 format, each component (Y, Cb, Cr) has the same resolution as the input RGB

## Implementation Notes

- FFmpeg's interpretation of chroma sample locations differs from the original specification
- There are discrepancies in how different implementations handle the C420 format
- FFmpeg's yuv4mpegenc incorrectly uses C420paldv for "top-left" coincidental sampling

For detailed implementation references, consult:
- y4m::Colorspace documentation
- aom-av1 y4menc.c
- ffmpeg yuv4mpegenc.c