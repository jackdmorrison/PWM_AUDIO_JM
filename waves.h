/* wave tables for a frequency of 216.0
 * with sampling rate of 28800.0
 */
#define WAV_DATA_LENGTH 134
#define WAV_FREQUENCY 216.0

const float clkDiv=3.0;
uint16_t wrap=199;
int clockFreq=172800;

uint8_t SIN_WAV_DATA[] = {
    100, 104, 109, 114, 118, 123, 127, 132, 136, 141, 145, 149, 153, 157, 161, 164, 168, 171, 175, 178, 180, 183, 186, 188, 190, 192, 194, 195, 196, 197, 198, 199, 199, 199, 199, 199, 199, 198, 197, 196, 195, 193, 191, 189, 187, 185, 182, 179, 177, 173, 170, 167, 163, 160, 156, 152, 148, 143, 139, 135, 130, 126, 121, 117, 112, 107, 103, 98, 93, 89, 84, 79, 75, 70, 66, 61, 57, 53, 49, 45, 41, 37, 33, 30, 27, 23, 20, 18, 15, 13, 10, 8, 7, 5, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 5, 7, 8, 10, 13, 15, 18, 20, 23, 27, 30, 33, 37, 41, 45, 49, 53, 57, 61, 66, 70, 75, 79, 84, 89, 93, 98, 103
};
uint8_t SQR_WAV_DATA[] = {
    200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
uint8_t TRI_WAV_DATA[] = {
    100, 103, 106, 109, 112, 114, 118, 121, 124, 127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172, 175, 178, 181, 184, 187, 190, 193, 196, 199, 198, 195, 192, 189, 186, 182, 179, 176, 173, 171, 168, 165, 162, 159, 155, 152, 149, 146, 143, 140, 138, 135, 131, 128, 126, 123, 120, 117, 114, 111, 108, 105, 102, 99, 96, 93, 90, 87, 84, 81, 78, 75, 72, 69, 66, 63, 60, 57, 54, 51, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0, 2, 5, 8, 11, 14, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47, 50, 53, 56, 59, 62, 65, 68, 71, 74, 77, 80, 83, 86, 89, 92, 95, 98, 101
};
uint8_t SAW_WAV_DATA[] = {
    100, 101, 103, 104, 106, 107, 109, 110, 112, 113, 114, 116, 118, 119, 121, 122, 124, 125, 127, 128, 130, 131, 133, 134, 136, 137, 139, 140, 142, 143, 145, 146, 148, 149, 151, 152, 154, 155, 157, 158, 160, 161, 163, 164, 166, 167, 169, 170, 172, 173, 175, 176, 178, 179, 181, 182, 184, 185, 187, 188, 190, 191, 192, 194, 195, 197, 198, 200, 1, 3, 4, 6, 7, 9, 10, 12, 13, 15, 16, 18, 19, 21, 22, 24, 25, 27, 28, 30, 31, 33, 34, 36, 37, 39, 40, 42, 43, 45, 46, 48, 49, 51, 52, 54, 55, 57, 58, 60, 61, 63, 64, 66, 67, 69, 70, 72, 73, 75, 76, 78, 79, 81, 82, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100
};
uint8_t R_SAW_WAV_DATA[] = {
    200, 198, 197, 195, 194, 192, 191, 189, 188, 186, 185, 183, 181, 180, 179, 177, 176, 174, 173, 171, 170, 168, 167, 165, 164, 162, 161, 159, 158, 156, 155, 153, 152, 150, 149, 147, 146, 144, 143, 141, 140, 138, 137, 135, 134, 132, 131, 129, 127, 126, 125, 123, 121, 120, 119, 117, 115, 114, 112, 111, 110, 108, 107, 105, 104, 102, 101, 99, 98, 96, 95, 93, 92, 90, 89, 87, 86, 84, 83, 81, 80, 78, 77, 75, 74, 72, 71, 69, 68, 66, 65, 63, 62, 60, 59, 57, 56, 54, 53, 51, 50, 48, 47, 45, 44, 42, 41, 39, 38, 36, 35, 33, 32, 30, 29, 27, 26, 24, 23, 21, 20, 18, 17, 15, 14, 12, 11, 9, 8, 6, 5, 3, 2, 0, 0
};
uint8_t PRBA_WAV_DATA[] = {
    200, 194, 188, 182, 176, 171, 165, 160, 154, 149, 144, 139, 134, 129, 124, 120, 115, 111, 106, 102, 97, 93, 89, 85, 81, 78, 74, 70, 67, 63, 60, 57, 54, 51, 48, 45, 42, 39, 36, 34, 31, 29, 27, 25, 23, 21, 19, 17, 15, 14, 12, 11, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 7, 9, 10, 12, 13, 15, 16, 18, 20, 22, 24, 26, 28, 31, 33, 36, 38, 41, 44, 47, 49, 53, 56, 59, 62, 66, 69, 73, 76, 80, 84, 88, 92, 96, 100, 105, 109, 114, 118, 123, 127, 132, 137, 142, 147, 153, 158, 163, 169, 174, 180, 186, 192, 198, 204
};
uint8_t HARMONIC2_WAV_DATA[] = {
    100, 109, 118, 127, 136, 145, 153, 161, 168, 175, 180, 186, 190, 194, 196, 198, 199, 199, 199, 197, 195, 191, 187, 182, 177, 170, 163, 156, 148, 139, 130, 121, 112, 103, 93, 84, 75, 66, 57, 49, 41, 33, 27, 20, 15, 10, 7, 3, 1, 0, 0, 0, 1, 3, 7, 10, 15, 20, 27, 33, 41, 49, 57, 66, 75, 84, 93, 103, 112, 121, 130, 139, 148, 156, 163, 170, 177, 182, 187, 191, 195, 197, 199, 199, 199, 198, 196, 194, 190, 186, 180, 175, 168, 161, 153, 145, 136, 127, 118, 109, 100, 90, 81, 72, 63, 54, 46, 38, 31, 24, 19, 13, 9, 5, 3, 1, 0, 0, 0, 2, 4, 8, 12, 17, 22, 29, 36, 43, 51, 60, 69, 78, 87, 96, 106
};
uint8_t HARMONIC3_WAV_DATA[] = {
    100, 114, 127, 141, 153, 164, 175, 183, 190, 195, 198, 199, 199, 196, 191, 185, 177, 167, 156, 143, 130, 117, 103, 89, 75, 61, 49, 37, 27, 18, 10, 5, 1, 0, 0, 2, 7, 13, 20, 30, 41, 53, 66, 79, 93, 107, 121, 135, 148, 160, 170, 179, 187, 193, 197, 199, 199, 197, 194, 188, 180, 171, 161, 149, 136, 123, 109, 95, 81, 67, 54, 42, 31, 21, 13, 7, 3, 0, 0, 1, 4, 10, 17, 26, 36, 47, 60, 73, 87, 101, 115, 129, 142, 154, 166, 176, 184, 191, 196, 199, 200, 199, 196, 191, 184, 176, 166, 154, 142, 129, 115, 101, 87, 73, 60, 47, 36, 26, 17, 10, 4, 1, 0, 0, 3, 7, 13, 21, 31, 42, 54, 67, 81, 95, 109
};
uint8_t HARMONIC4_WAV_DATA[] = {
    100, 118, 136, 153, 168, 180, 190, 196, 199, 199, 195, 187, 177, 163, 148, 130, 112, 93, 75, 57, 41, 27, 15, 7, 1, 0, 1, 7, 15, 27, 41, 57, 75, 93, 112, 130, 148, 163, 177, 187, 195, 199, 199, 196, 190, 180, 168, 153, 136, 118, 100, 81, 63, 46, 31, 19, 9, 3, 0, 0, 4, 12, 22, 36, 51, 69, 87, 106, 124, 142, 158, 172, 184, 192, 198, 200, 198, 192, 184, 172, 158, 142, 124, 106, 87, 69, 51, 36, 22, 12, 4, 0, 0, 3, 9, 19, 31, 46, 63, 81, 99, 118, 136, 153, 168, 180, 190, 196, 199, 199, 195, 187, 177, 163, 148, 130, 112, 93, 75, 57, 41, 27, 15, 7, 1, 0, 1, 7, 15, 27, 41, 57, 75, 93, 112
};
uint8_t HARMONIC5_WAV_DATA[] = {
    100, 123, 145, 164, 180, 192, 198, 199, 195, 185, 170, 152, 130, 107, 84, 61, 41, 23, 10, 2, 0, 2, 10, 23, 41, 61, 84, 107, 130, 152, 170, 185, 195, 199, 198, 192, 180, 164, 145, 123, 100, 76, 54, 35, 19, 7, 1, 0, 4, 14, 29, 47, 69, 92, 115, 138, 158, 176, 189, 197, 200, 197, 189, 176, 158, 138, 115, 92, 69, 47, 29, 14, 4, 0, 1, 7, 19, 35, 54, 76, 99, 123, 145, 164, 180, 192, 198, 199, 195, 185, 170, 152, 130, 107, 84, 61, 41, 23, 10, 2, 0, 2, 10, 23, 41, 61, 84, 107, 130, 152, 170, 185, 195, 199, 198, 192, 180, 164, 145, 123, 100, 76, 54, 35, 19, 7, 1, 0, 4, 14, 29, 47, 69, 92, 115
};
uint8_t HARMONIC6_WAV_DATA[] = {
    100, 127, 153, 175, 190, 198, 199, 191, 177, 156, 130, 103, 75, 49, 27, 10, 1, 0, 7, 20, 41, 66, 93, 121, 148, 170, 187, 197, 199, 194, 180, 161, 136, 109, 81, 54, 31, 13, 3, 0, 4, 17, 36, 60, 87, 115, 142, 166, 184, 196, 200, 196, 184, 166, 142, 115, 87, 60, 36, 17, 4, 0, 3, 13, 31, 54, 81, 109, 136, 161, 180, 194, 199, 197, 187, 170, 148, 121, 93, 66, 41, 20, 7, 0, 1, 10, 27, 49, 75, 103, 130, 156, 177, 191, 199, 198, 190, 175, 153, 127, 100, 72, 46, 24, 9, 1, 0, 8, 22, 43, 69, 96, 124, 150, 172, 189, 198, 199, 192, 179, 158, 133, 106, 78, 51, 29, 12, 2, 0, 5, 19, 38, 63, 90, 118
};
uint8_t HARMONIC7_WAV_DATA[] = {
    100, 132, 161, 183, 196, 199, 191, 173, 148, 117, 84, 53, 27, 8, 0, 2, 15, 37, 66, 98, 130, 160, 182, 196, 199, 192, 175, 149, 118, 85, 54, 28, 9, 0, 2, 14, 36, 64, 96, 129, 158, 181, 196, 199, 192, 176, 150, 120, 87, 56, 29, 10, 0, 2, 13, 35, 63, 95, 127, 157, 180, 195, 199, 193, 177, 152, 121, 89, 57, 30, 10, 0, 1, 13, 33, 61, 93, 126, 156, 179, 195, 199, 194, 178, 153, 123, 90, 58, 31, 11, 1, 1, 12, 32, 60, 92, 124, 154, 179, 194, 200, 194, 179, 154, 124, 92, 60, 32, 12, 1, 1, 11, 31, 58, 90, 123, 153, 178, 194, 199, 195, 179, 156, 126, 93, 61, 33, 13, 1, 0, 10, 30, 57, 89, 121
};
uint8_t HARMONIC8_WAV_DATA[] = {
    100, 136, 168, 190, 199, 195, 177, 148, 112, 75, 41, 15, 1, 1, 15, 41, 75, 112, 148, 177, 195, 199, 190, 168, 136, 100, 63, 31, 9, 0, 4, 22, 51, 87, 124, 158, 184, 198, 198, 184, 158, 124, 87, 51, 22, 4, 0, 9, 31, 63, 99, 136, 168, 190, 199, 195, 177, 148, 112, 75, 41, 15, 1, 1, 15, 41, 75, 112, 148, 177, 195, 199, 190, 168, 136, 100, 63, 31, 9, 0, 4, 22, 51, 87, 124, 158, 184, 198, 198, 184, 158, 124, 87, 51, 22, 4, 0, 9, 31, 63, 99, 136, 168, 190, 199, 195, 177, 148, 112, 75, 41, 15, 1, 1, 15, 41, 75, 112, 148, 177, 195, 199, 190, 168, 136, 100, 63, 31, 9, 0, 4, 22, 51, 87, 124
};
uint8_t HARMONIC9_WAV_DATA[] = {
    100, 141, 175, 195, 199, 185, 156, 117, 75, 37, 10, 0, 7, 30, 66, 107, 148, 179, 197, 197, 180, 149, 109, 67, 31, 7, 0, 10, 36, 73, 115, 154, 184, 199, 196, 176, 142, 101, 60, 26, 4, 0, 13, 42, 81, 123, 161, 188, 199, 193, 170, 135, 93, 53, 20, 2, 1, 18, 49, 89, 130, 167, 191, 199, 190, 164, 127, 85, 46, 16, 1, 3, 22, 56, 96, 138, 172, 194, 199, 186, 158, 120, 78, 39, 12, 0, 5, 28, 63, 104, 145, 178, 196, 198, 182, 152, 112, 70, 33, 8, 0, 8, 33, 70, 112, 152, 182, 198, 196, 178, 145, 104, 63, 28, 5, 0, 12, 39, 78, 120, 158, 186, 199, 194, 172, 138, 96, 56, 22, 3, 1, 16, 46, 85, 127
};
uint8_t HARMONIC10_WAV_DATA[] = {
    100, 145, 180, 198, 195, 170, 130, 84, 41, 10, 0, 10, 41, 84, 130, 170, 195, 198, 180, 145, 100, 54, 19, 1, 4, 29, 69, 115, 158, 189, 200, 189, 158, 115, 69, 29, 4, 1, 19, 54, 99, 145, 180, 198, 195, 170, 130, 84, 41, 10, 0, 10, 41, 84, 130, 170, 195, 198, 180, 145, 100, 54, 19, 1, 4, 29, 69, 115, 158, 189, 200, 189, 158, 115, 69, 29, 4, 1, 19, 54, 99, 145, 180, 198, 195, 170, 130, 84, 41, 10, 0, 10, 41, 84, 130, 170, 195, 198, 180, 145, 100, 54, 19, 1, 4, 29, 69, 115, 158, 189, 200, 189, 158, 115, 69, 29, 4, 1, 19, 54, 99, 145, 180, 198, 195, 170, 130, 84, 41, 10, 0, 10, 41, 84, 130
};
uint8_t HARMONIC11_WAV_DATA[] = {
    100, 149, 186, 199, 187, 152, 103, 53, 15, 0, 10, 45, 93, 143, 182, 199, 190, 157, 109, 58, 19, 0, 8, 39, 87, 138, 179, 199, 192, 162, 115, 64, 22, 1, 5, 35, 81, 132, 175, 197, 195, 167, 121, 70, 27, 2, 3, 30, 75, 126, 170, 196, 196, 171, 127, 76, 31, 4, 2, 26, 69, 120, 166, 194, 198, 176, 133, 82, 36, 6, 1, 21, 63, 114, 161, 192, 199, 179, 139, 89, 41, 8, 0, 18, 57, 107, 156, 189, 199, 183, 145, 95, 46, 11, 0, 14, 51, 101, 150, 186, 200, 186, 150, 101, 51, 14, 0, 11, 46, 95, 145, 183, 199, 189, 156, 107, 57, 18, 0, 8, 41, 89, 139, 179, 199, 192, 161, 114, 63, 21, 1, 6, 36, 82, 133
};
uint8_t HARMONIC12_WAV_DATA[] = {
    100, 153, 190, 199, 177, 130, 75, 27, 1, 7, 41, 93, 148, 187, 199, 180, 136, 81, 31, 3, 4, 36, 87, 142, 184, 200, 184, 142, 87, 36, 4, 3, 31, 81, 136, 180, 199, 187, 148, 93, 41, 7, 1, 27, 75, 130, 177, 199, 190, 153, 100, 46, 9, 0, 22, 69, 124, 172, 198, 192, 158, 106, 51, 12, 0, 19, 63, 118, 168, 196, 195, 163, 112, 57, 15, 0, 15, 57, 112, 163, 195, 196, 168, 118, 63, 19, 0, 12, 51, 106, 158, 192, 198, 172, 124, 69, 22, 0, 9, 46, 99, 153, 190, 199, 177, 130, 75, 27, 1, 7, 41, 93, 148, 187, 199, 180, 136, 81, 31, 3, 4, 36, 87, 142, 184, 200, 184, 142, 87, 36, 4, 3, 31, 81, 136
};
uint8_t HARMONIC13_WAV_DATA[] = {
    100, 157, 194, 196, 163, 107, 49, 8, 1, 30, 84, 143, 187, 199, 175, 123, 63, 16, 0, 20, 69, 129, 179, 199, 184, 138, 78, 26, 0, 11, 54, 114, 168, 197, 191, 152, 93, 37, 3, 5, 41, 98, 156, 193, 196, 164, 109, 50, 9, 1, 29, 82, 142, 186, 199, 176, 124, 64, 17, 0, 19, 67, 127, 178, 199, 185, 139, 79, 27, 0, 10, 53, 112, 167, 197, 192, 153, 95, 38, 4, 4, 39, 96, 154, 192, 197, 166, 110, 51, 10, 1, 28, 81, 141, 186, 199, 177, 126, 66, 18, 0, 18, 66, 126, 177, 199, 186, 141, 81, 28, 1, 10, 51, 110, 166, 197, 192, 154, 96, 39, 4, 4, 38, 95, 153, 192, 197, 167, 112, 53, 10, 0, 27, 79, 139
};
const float lowestFrequency=12.656;
const float highestFrequency=460.8;
const float freqList[]={
   13.5, 14.4, 15.188, 16.2, 16.875, 18.0, 18.984, 20.25, 21.6, 22.5, 24.3, 25.312, 27.0, 28.8, 30.375, 32.4, 33.75, 36.0, 37.969, 40.5, 43.2, 45.0, 48.6, 50.625, 54.0, 57.6, 60.75, 64.8, 67.5, 72.0, 75.938, 81.0, 86.4, 90.0, 97.2, 101.25, 108.0, 115.2, 121.5, 129.6, 135.0, 144.0, 151.875, 162.0, 172.8, 180.0, 194.4, 202.5, 216.0, 230.4, 243.0, 259.2, 270.0, 288.0, 303.75, 324.0, 345.6, 360.0, 388.8, 405.0, 432.0
};
