/* wave tables for a frequency of 108.0
 * with sampling rate of 21600.0
 */
#define WAV_DATA_LENGTH 200
#define WAV_FREQUENCY 108.0

const float clkDiv=10.667;
uint16_t wrap=74;
int clockFreq=172800;

uint8_t SIN_WAV_DATA[] = {
    37, 38, 39, 41, 42, 43, 44, 45, 46, 47, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 64, 65, 66, 67, 67, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 73, 74, 74, 74, 74, 74, 74, 74, 75, 74, 74, 74, 74, 74, 74, 74, 73, 73, 73, 72, 72, 71, 71, 70, 70, 69, 69, 68, 67, 67, 66, 65, 64, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 47, 46, 45, 44, 43, 42, 41, 39, 38, 37, 36, 35, 33, 32, 31, 30, 29, 28, 27, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 10, 9, 8, 7, 7, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30, 31, 32, 33, 35, 36, 37, 38
};
uint8_t SQR_WAV_DATA[] = {
    75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
uint8_t TRI_WAV_DATA[] = {
    37, 38, 39, 39, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 48, 48, 49, 50, 51, 51, 52, 53, 54, 54, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66, 67, 68, 68, 69, 70, 71, 72, 72, 73, 74, 75, 74, 73, 72, 71, 71, 70, 69, 68, 68, 67, 66, 65, 65, 64, 63, 62, 62, 61, 60, 59, 59, 58, 57, 56, 56, 55, 54, 53, 53, 52, 51, 50, 50, 49, 48, 47, 47, 46, 45, 44, 44, 43, 42, 41, 41, 40, 39, 38, 38, 37, 36, 35, 35, 34, 33, 32, 32, 31, 30, 30, 29, 28, 27, 27, 26, 25, 24, 24, 23, 22, 21, 21, 20, 19, 18, 18, 17, 16, 15, 15, 14, 13, 12, 12, 11, 10, 9, 9, 8, 7, 6, 6, 5, 4, 3, 3, 2, 1, 0, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 8, 8, 9, 10, 11, 11, 12, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 23, 23, 24, 25, 26, 26, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38
};
uint8_t SAW_WAV_DATA[] = {
    37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 45, 45, 45, 46, 46, 46, 47, 47, 48, 48, 48, 49, 49, 49, 50, 50, 50, 51, 51, 52, 52, 52, 53, 53, 54, 54, 54, 55, 55, 55, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 60, 60, 60, 61, 61, 61, 62, 62, 63, 63, 63, 64, 64, 64, 65, 65, 66, 66, 66, 67, 67, 67, 68, 68, 69, 69, 69, 70, 70, 70, 71, 71, 72, 72, 72, 73, 73, 73, 74, 74, 75, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 31, 31, 31, 32, 32, 32, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 37, 37, 37
};
uint8_t R_SAW_WAV_DATA[] = {
    75, 74, 74, 73, 73, 73, 72, 72, 72, 71, 71, 70, 70, 70, 69, 69, 69, 68, 68, 67, 67, 67, 66, 66, 66, 65, 65, 64, 64, 64, 63, 63, 63, 62, 62, 61, 61, 61, 60, 60, 60, 59, 59, 58, 58, 58, 57, 57, 57, 56, 56, 55, 55, 55, 54, 54, 53, 53, 53, 52, 52, 52, 51, 51, 50, 50, 50, 49, 49, 49, 48, 48, 47, 47, 47, 46, 46, 46, 45, 45, 44, 44, 44, 43, 43, 43, 42, 42, 41, 41, 41, 40, 40, 40, 39, 39, 38, 38, 38, 37, 37, 37, 36, 36, 36, 35, 35, 34, 34, 34, 33, 33, 33, 32, 32, 31, 31, 31, 30, 30, 30, 29, 29, 28, 28, 28, 27, 27, 27, 26, 26, 25, 25, 25, 24, 24, 24, 23, 23, 22, 22, 22, 21, 21, 21, 20, 20, 19, 19, 19, 18, 18, 18, 17, 17, 16, 16, 16, 15, 15, 15, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 10, 10, 10, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0
};
uint8_t PRBA_WAV_DATA[] = {
    75, 73, 72, 70, 69, 67, 66, 64, 63, 62, 60, 59, 58, 56, 55, 54, 52, 51, 50, 49, 47, 46, 45, 44, 43, 42, 41, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 27, 26, 25, 24, 23, 22, 21, 21, 20, 19, 18, 18, 17, 16, 15, 15, 14, 13, 13, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 13, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 21, 22, 23, 24, 25, 26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 41, 42, 43, 44, 45, 46, 47, 49, 50, 51, 52, 54, 55, 56, 58, 59, 60, 62, 63, 64, 66, 67, 69, 70, 72, 73, 74, 76
};
uint8_t HARMONIC2_WAV_DATA[] = {
    37, 39, 42, 44, 46, 49, 51, 53, 55, 57, 59, 61, 63, 64, 66, 67, 69, 70, 71, 72, 73, 73, 74, 74, 74, 75, 74, 74, 74, 73, 73, 72, 71, 70, 69, 67, 66, 64, 63, 61, 59, 57, 55, 53, 51, 49, 46, 44, 42, 39, 37, 35, 32, 30, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10, 8, 7, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 30, 32, 35, 37, 39, 42, 44, 46, 49, 51, 53, 55, 57, 59, 61, 63, 64, 66, 67, 69, 70, 71, 72, 73, 73, 74, 74, 74, 75, 74, 74, 74, 73, 73, 72, 71, 70, 69, 67, 66, 64, 63, 61, 59, 57, 55, 53, 51, 49, 46, 44, 42, 39, 37, 35, 32, 30, 28, 25, 23, 21, 19, 17, 15, 13, 11, 10, 8, 7, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 7, 8, 10, 11, 13, 15, 17, 19, 21, 23, 25, 28, 30, 32, 35, 37, 39
};
uint8_t HARMONIC3_WAV_DATA[] = {
    37, 41, 44, 47, 51, 54, 57, 60, 63, 65, 67, 69, 71, 72, 73, 74, 74, 74, 74, 74, 73, 71, 70, 68, 66, 64, 61, 58, 55, 52, 49, 45, 42, 38, 35, 31, 28, 24, 21, 18, 15, 12, 10, 7, 5, 4, 2, 1, 0, 0, 0, 0, 0, 1, 2, 4, 5, 7, 10, 12, 15, 18, 21, 24, 28, 31, 35, 38, 42, 45, 49, 52, 55, 58, 61, 64, 66, 68, 70, 71, 73, 74, 74, 74, 74, 74, 73, 72, 71, 69, 67, 65, 63, 60, 57, 54, 51, 47, 44, 41, 37, 33, 30, 27, 23, 20, 17, 14, 11, 9, 7, 5, 3, 2, 1, 0, 0, 0, 0, 0, 1, 3, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 32, 36, 39, 43, 46, 50, 53, 56, 59, 62, 64, 67, 69, 70, 72, 73, 74, 74, 75, 74, 74, 73, 72, 70, 69, 67, 64, 62, 59, 56, 53, 50, 46, 43, 39, 36, 32, 29, 25, 22, 19, 16, 13, 10, 8, 6, 4, 3, 1, 0, 0, 0, 0, 0, 1, 2, 3, 5, 7, 9, 11, 14, 17, 20, 23, 27, 30, 33, 37, 41
};
uint8_t HARMONIC4_WAV_DATA[] = {
    37, 42, 46, 51, 55, 59, 63, 66, 69, 71, 73, 74, 74, 74, 74, 73, 71, 69, 66, 63, 59, 55, 51, 46, 42, 37, 32, 28, 23, 19, 15, 11, 8, 5, 3, 1, 0, 0, 0, 0, 1, 3, 5, 8, 11, 15, 19, 23, 28, 32, 37, 42, 46, 51, 55, 59, 63, 66, 69, 71, 73, 74, 74, 74, 74, 73, 71, 69, 66, 63, 59, 55, 51, 46, 42, 37, 32, 28, 23, 19, 15, 11, 8, 5, 3, 1, 0, 0, 0, 0, 1, 3, 5, 8, 11, 15, 19, 23, 28, 32, 37, 42, 46, 51, 55, 59, 63, 66, 69, 71, 73, 74, 74, 74, 74, 73, 71, 69, 66, 63, 59, 55, 51, 46, 42, 37, 32, 28, 23, 19, 15, 11, 8, 5, 3, 1, 0, 0, 0, 0, 1, 3, 5, 8, 11, 15, 19, 23, 28, 32, 37, 42, 46, 51, 55, 59, 63, 66, 69, 71, 73, 74, 74, 74, 74, 73, 71, 69, 66, 63, 59, 55, 51, 46, 42, 37, 32, 28, 23, 19, 15, 11, 8, 5, 3, 1, 0, 0, 0, 0, 1, 3, 5, 8, 11, 15, 19, 23, 28, 32, 37, 42
};
uint8_t HARMONIC5_WAV_DATA[] = {
    37, 43, 49, 54, 59, 64, 67, 70, 73, 74, 75, 74, 73, 70, 67, 64, 59, 54, 49, 43, 37, 31, 25, 20, 15, 10, 7, 4, 1, 0, 0, 0, 1, 4, 7, 10, 15, 20, 25, 31, 37, 43, 49, 54, 59, 64, 67, 70, 73, 74, 75, 74, 73, 70, 67, 64, 59, 54, 49, 43, 37, 31, 25, 20, 15, 10, 7, 4, 1, 0, 0, 0, 1, 4, 7, 10, 15, 20, 25, 31, 37, 43, 49, 54, 59, 64, 67, 70, 73, 74, 75, 74, 73, 70, 67, 64, 59, 54, 49, 43, 37, 31, 25, 20, 15, 10, 7, 4, 1, 0, 0, 0, 1, 4, 7, 10, 15, 20, 25, 31, 37, 43, 49, 54, 59, 64, 67, 70, 73, 74, 75, 74, 73, 70, 67, 64, 59, 54, 49, 43, 37, 31, 25, 20, 15, 10, 7, 4, 1, 0, 0, 0, 1, 4, 7, 10, 15, 20, 25, 31, 37, 43, 49, 54, 59, 64, 67, 70, 73, 74, 75, 74, 73, 70, 67, 64, 59, 54, 49, 43, 37, 31, 25, 20, 15, 10, 7, 4, 1, 0, 0, 0, 1, 4, 7, 10, 15, 20, 25, 31, 37, 43
};
uint8_t HARMONIC6_WAV_DATA[] = {
    37, 44, 51, 57, 63, 67, 71, 73, 74, 74, 73, 70, 66, 61, 55, 49, 42, 35, 28, 21, 15, 10, 5, 2, 0, 0, 0, 2, 5, 10, 15, 21, 28, 35, 42, 49, 55, 61, 66, 70, 73, 74, 74, 73, 71, 67, 63, 57, 51, 44, 37, 30, 23, 17, 11, 7, 3, 1, 0, 0, 1, 4, 8, 13, 19, 25, 32, 39, 46, 53, 59, 64, 69, 72, 74, 75, 74, 72, 69, 64, 59, 53, 46, 39, 32, 25, 19, 13, 8, 4, 1, 0, 0, 1, 3, 7, 11, 17, 23, 30, 37, 44, 51, 57, 63, 67, 71, 73, 74, 74, 73, 70, 66, 61, 55, 49, 42, 35, 28, 21, 15, 10, 5, 2, 0, 0, 0, 2, 5, 10, 15, 21, 28, 35, 42, 49, 55, 61, 66, 70, 73, 74, 74, 73, 71, 67, 63, 57, 51, 44, 37, 30, 23, 17, 11, 7, 3, 1, 0, 0, 1, 4, 8, 13, 19, 25, 32, 39, 46, 53, 59, 64, 69, 72, 74, 75, 74, 72, 69, 64, 59, 53, 46, 39, 32, 25, 19, 13, 8, 4, 1, 0, 0, 1, 3, 7, 11, 17, 23, 30, 37, 44
};
uint8_t HARMONIC7_WAV_DATA[] = {
    37, 45, 53, 60, 66, 70, 73, 74, 74, 71, 67, 62, 55, 47, 39, 31, 23, 16, 10, 5, 1, 0, 0, 2, 5, 10, 17, 24, 32, 41, 49, 56, 63, 68, 72, 74, 74, 73, 70, 65, 59, 52, 44, 36, 28, 20, 13, 7, 3, 0, 0, 0, 3, 7, 13, 20, 28, 36, 44, 52, 59, 65, 70, 73, 74, 74, 72, 68, 63, 56, 49, 41, 32, 24, 17, 10, 5, 2, 0, 0, 1, 5, 10, 16, 23, 31, 39, 47, 55, 62, 67, 71, 74, 74, 73, 70, 66, 60, 53, 45, 37, 29, 21, 14, 8, 4, 1, 0, 0, 3, 7, 12, 19, 27, 35, 43, 51, 58, 64, 69, 73, 74, 74, 72, 69, 64, 57, 50, 42, 33, 25, 18, 11, 6, 2, 0, 0, 1, 4, 9, 15, 22, 30, 38, 46, 54, 61, 67, 71, 74, 75, 74, 71, 67, 61, 54, 46, 38, 30, 22, 15, 9, 4, 1, 0, 0, 2, 6, 11, 18, 25, 33, 42, 50, 57, 64, 69, 72, 74, 74, 73, 69, 64, 58, 51, 43, 35, 27, 19, 12, 7, 3, 0, 0, 1, 4, 8, 14, 21, 29, 37, 45
};
uint8_t HARMONIC8_WAV_DATA[] = {
    37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46, 55, 63, 69, 73, 74, 74, 71, 66, 59, 51, 42, 32, 23, 15, 8, 3, 0, 0, 1, 5, 11, 19, 28, 37, 46
};
uint8_t HARMONIC9_WAV_DATA[] = {
    37, 47, 57, 65, 71, 74, 74, 71, 66, 58, 49, 38, 28, 18, 10, 4, 0, 0, 2, 7, 15, 24, 35, 45, 55, 64, 70, 74, 74, 72, 67, 60, 51, 41, 30, 20, 11, 5, 1, 0, 1, 6, 13, 22, 32, 43, 53, 62, 69, 73, 75, 73, 69, 62, 53, 43, 32, 22, 13, 6, 1, 0, 1, 5, 11, 20, 30, 41, 51, 60, 67, 72, 74, 74, 70, 64, 55, 45, 35, 24, 15, 7, 2, 0, 0, 4, 10, 18, 28, 38, 49, 58, 66, 71, 74, 74, 71, 65, 57, 47, 37, 27, 17, 9, 3, 0, 0, 3, 8, 16, 25, 36, 46, 56, 64, 70, 74, 74, 72, 67, 59, 50, 39, 29, 19, 10, 4, 0, 0, 2, 7, 14, 23, 33, 44, 54, 63, 69, 73, 74, 73, 68, 61, 52, 42, 31, 21, 12, 5, 1, 0, 1, 5, 12, 21, 31, 42, 52, 61, 68, 73, 74, 73, 69, 63, 54, 44, 33, 23, 14, 7, 2, 0, 0, 4, 10, 19, 29, 39, 50, 59, 67, 72, 74, 74, 70, 64, 56, 46, 36, 25, 16, 8, 3, 0, 0, 3, 9, 17, 27, 37, 47
};
uint8_t HARMONIC10_WAV_DATA[] = {
    37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49, 59, 67, 73, 75, 73, 67, 59, 49, 37, 25, 15, 7, 1, 0, 1, 7, 15, 25, 37, 49
};
uint8_t HARMONIC11_WAV_DATA[] = {
    37, 50, 61, 69, 74, 74, 70, 62, 51, 38, 25, 14, 5, 0, 0, 4, 11, 22, 35, 47, 59, 68, 73, 74, 71, 64, 53, 41, 28, 16, 7, 1, 0, 3, 10, 20, 32, 45, 57, 67, 73, 74, 72, 65, 55, 43, 30, 18, 8, 2, 0, 2, 8, 18, 30, 43, 55, 65, 72, 74, 73, 67, 57, 45, 32, 20, 10, 3, 0, 1, 7, 16, 28, 41, 53, 64, 71, 74, 73, 68, 59, 47, 35, 22, 11, 4, 0, 0, 5, 14, 25, 38, 51, 62, 70, 74, 74, 69, 61, 50, 37, 24, 13, 5, 0, 0, 4, 12, 23, 36, 49, 60, 69, 74, 74, 70, 63, 52, 39, 27, 15, 6, 1, 0, 3, 10, 21, 33, 46, 58, 67, 73, 74, 71, 64, 54, 42, 29, 17, 7, 1, 0, 2, 9, 19, 31, 44, 56, 66, 72, 75, 72, 66, 56, 44, 31, 19, 9, 2, 0, 1, 7, 17, 29, 42, 54, 64, 71, 74, 73, 67, 58, 46, 33, 21, 10, 3, 0, 1, 6, 15, 27, 39, 52, 63, 70, 74, 74, 69, 60, 49, 36, 23, 12, 4, 0, 0, 5, 13, 24, 37, 50
};
uint8_t HARMONIC12_WAV_DATA[] = {
    37, 51, 63, 71, 74, 73, 66, 55, 42, 28, 15, 5, 0, 0, 5, 15, 28, 42, 55, 66, 73, 74, 71, 63, 51, 37, 23, 11, 3, 0, 1, 8, 19, 32, 46, 59, 69, 74, 74, 69, 59, 46, 32, 19, 8, 1, 0, 3, 11, 23, 37, 51, 63, 71, 74, 73, 66, 55, 42, 28, 15, 5, 0, 0, 5, 15, 28, 42, 55, 66, 73, 74, 71, 63, 51, 37, 23, 11, 3, 0, 1, 8, 19, 32, 46, 59, 69, 74, 74, 69, 59, 46, 32, 19, 8, 1, 0, 3, 11, 23, 37, 51, 63, 71, 74, 73, 66, 55, 42, 28, 15, 5, 0, 0, 5, 15, 28, 42, 55, 66, 73, 74, 71, 63, 51, 37, 23, 11, 3, 0, 1, 8, 19, 32, 46, 59, 69, 74, 74, 69, 59, 46, 32, 19, 8, 1, 0, 3, 11, 23, 37, 51, 63, 71, 74, 73, 66, 55, 42, 28, 15, 5, 0, 0, 5, 15, 28, 42, 55, 66, 73, 74, 71, 63, 51, 37, 23, 11, 3, 0, 1, 8, 19, 32, 46, 59, 69, 74, 74, 69, 59, 46, 32, 19, 8, 1, 0, 3, 11, 23, 37, 51
};
uint8_t HARMONIC13_WAV_DATA[] = {
    37, 52, 64, 72, 74, 70, 61, 47, 32, 18, 7, 0, 0, 6, 17, 31, 46, 60, 70, 74, 73, 65, 53, 38, 23, 10, 2, 0, 3, 12, 25, 41, 55, 67, 73, 74, 69, 58, 44, 29, 15, 5, 0, 1, 8, 20, 35, 50, 63, 71, 75, 71, 63, 50, 35, 20, 8, 1, 0, 5, 15, 29, 44, 58, 69, 74, 73, 67, 55, 41, 25, 12, 3, 0, 2, 10, 23, 38, 53, 65, 73, 74, 70, 60, 46, 31, 17, 6, 0, 0, 7, 18, 32, 47, 61, 70, 74, 72, 64, 52, 37, 22, 10, 2, 0, 4, 13, 27, 42, 56, 67, 74, 74, 68, 57, 43, 28, 14, 4, 0, 1, 9, 21, 36, 51, 64, 72, 74, 71, 62, 49, 33, 19, 7, 1, 0, 5, 16, 30, 45, 59, 69, 74, 73, 66, 54, 39, 24, 11, 3, 0, 3, 11, 24, 39, 54, 66, 73, 74, 69, 59, 45, 30, 16, 5, 0, 1, 7, 19, 33, 49, 62, 71, 74, 72, 64, 51, 36, 21, 9, 1, 0, 4, 14, 28, 43, 57, 68, 74, 74, 67, 56, 42, 27, 13, 4, 0, 2, 10, 22, 37, 52
};
const float lowestFrequency=25.312;
const float highestFrequency=921.6;
const float freqList[]={
   27.0, 28.8, 30.375, 32.4, 33.75, 36.0, 37.969, 40.5, 43.2, 45.0, 48.6, 50.625, 54.0, 57.6, 60.75, 64.8, 67.5, 72.0, 75.938, 81.0, 86.4, 90.0, 97.2, 101.25, 108.0, 115.2, 121.5, 129.6, 135.0, 144.0, 151.875, 162.0, 172.8, 180.0, 194.4, 202.5, 216.0, 230.4, 243.0, 259.2, 270.0, 288.0, 303.75, 324.0, 345.6, 360.0, 388.8, 405.0, 432.0, 460.8, 486.0, 518.4, 540.0, 576.0, 607.5, 648.0, 691.2, 720.0, 777.6, 810.0, 864.0
};
