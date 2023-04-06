/* wave tables for a frequency of 161.82
 * with sampling rate of 21600.0
 */
#define WAV_DATA_LENGTH 134
#define WAV_FREQUENCY 161.82

const float clkDiv=8.0;
uint16_t wrap=99;
int clockFreq=172800;

uint8_t SIN_WAV_DATA[] = {
    50, 52, 54, 57, 59, 61, 63, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 85, 87, 88, 90, 91, 93, 94, 95, 96, 97, 97, 98, 98, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 97, 96, 95, 94, 93, 92, 91, 90, 88, 87, 85, 83, 81, 80, 78, 76, 74, 72, 69, 67, 65, 63, 61, 58, 56, 54, 51, 49, 47, 44, 42, 40, 37, 35, 33, 31, 28, 26, 24, 22, 20, 18, 17, 15, 13, 12, 10, 9, 7, 6, 5, 4, 3, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 13, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 37, 39, 41, 44, 46, 48, 51
};
uint8_t SQR_WAV_DATA[] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
uint8_t TRI_WAV_DATA[] = {
    50, 51, 52, 54, 55, 57, 58, 60, 61, 63, 64, 66, 67, 69, 70, 72, 73, 75, 76, 78, 79, 81, 82, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 99, 97, 96, 94, 93, 91, 90, 88, 87, 85, 84, 82, 81, 79, 78, 76, 75, 73, 72, 70, 69, 67, 66, 64, 63, 61, 60, 58, 57, 55, 54, 52, 51, 49, 48, 46, 45, 43, 42, 40, 39, 37, 36, 34, 33, 31, 30, 28, 27, 25, 24, 22, 21, 19, 18, 16, 15, 13, 12, 10, 9, 7, 6, 4, 3, 1, 0, 1, 2, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 19, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 35, 37, 38, 40, 41, 43, 44, 46, 47, 49, 50
};
uint8_t SAW_WAV_DATA[] = {
    50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 61, 62, 63, 64, 64, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 73, 73, 74, 75, 76, 76, 77, 78, 79, 79, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 88, 88, 89, 90, 91, 91, 92, 93, 94, 94, 95, 96, 97, 97, 98, 99, 100, 0, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 21, 22, 23, 24, 24, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 33, 33, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 48, 48, 49, 50
};
uint8_t R_SAW_WAV_DATA[] = {
    100, 99, 98, 97, 97, 96, 95, 94, 94, 93, 92, 91, 91, 90, 89, 88, 88, 87, 86, 85, 85, 84, 83, 82, 82, 81, 80, 79, 79, 78, 77, 76, 76, 75, 74, 73, 73, 72, 71, 70, 70, 69, 68, 67, 67, 66, 65, 64, 64, 63, 62, 61, 61, 60, 59, 58, 58, 57, 56, 55, 55, 54, 53, 52, 52, 51, 50, 49, 49, 48, 47, 46, 46, 45, 44, 43, 43, 42, 41, 40, 40, 39, 38, 37, 37, 36, 35, 34, 34, 33, 32, 31, 31, 30, 29, 28, 28, 27, 26, 25, 25, 24, 23, 22, 22, 21, 20, 19, 19, 18, 17, 16, 16, 15, 14, 13, 13, 12, 11, 10, 10, 9, 8, 7, 7, 6, 5, 4, 4, 3, 2, 1, 1, 0, 0
};
uint8_t PRBA_WAV_DATA[] = {
    99, 97, 94, 91, 88, 85, 82, 80, 77, 74, 72, 69, 67, 64, 62, 60, 57, 55, 53, 51, 49, 46, 44, 42, 41, 39, 37, 35, 33, 31, 30, 28, 27, 25, 24, 22, 21, 19, 18, 17, 16, 14, 13, 12, 11, 10, 9, 8, 7, 7, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 23, 24, 26, 27, 29, 31, 32, 34, 36, 38, 40, 42, 43, 45, 48, 50, 52, 54, 56, 58, 61, 63, 66, 68, 71, 73, 76, 78, 81, 84, 87, 89, 92, 95, 98, 101
};
uint8_t HARMONIC2_WAV_DATA[] = {
    50, 54, 59, 63, 68, 72, 76, 80, 84, 87, 90, 93, 95, 97, 98, 99, 99, 99, 99, 98, 97, 95, 93, 91, 88, 85, 81, 78, 74, 69, 65, 61, 56, 51, 47, 42, 37, 33, 28, 24, 20, 17, 13, 10, 7, 5, 3, 2, 0, 0, 0, 0, 0, 1, 3, 5, 7, 10, 13, 16, 20, 24, 28, 32, 37, 41, 46, 51, 55, 60, 65, 69, 73, 77, 81, 85, 88, 91, 93, 95, 97, 98, 99, 99, 99, 99, 98, 97, 95, 93, 90, 87, 84, 81, 77, 73, 68, 64, 59, 55, 50, 45, 41, 36, 32, 27, 23, 19, 16, 12, 9, 7, 5, 3, 1, 0, 0, 0, 0, 1, 2, 3, 5, 8, 11, 14, 17, 21, 25, 29, 33, 38, 43, 47, 52
};
uint8_t HARMONIC3_WAV_DATA[] = {
    50, 57, 63, 70, 76, 82, 87, 91, 95, 97, 99, 99, 99, 98, 95, 92, 88, 83, 78, 72, 65, 58, 51, 44, 37, 31, 24, 18, 13, 9, 5, 2, 0, 0, 0, 1, 3, 6, 10, 14, 20, 26, 32, 39, 46, 53, 60, 67, 73, 79, 85, 89, 93, 96, 98, 99, 99, 99, 97, 94, 90, 86, 81, 75, 68, 62, 55, 48, 41, 34, 27, 21, 16, 11, 7, 4, 1, 0, 0, 0, 2, 4, 8, 12, 17, 23, 29, 36, 43, 50, 57, 64, 70, 76, 82, 87, 91, 95, 97, 99, 99, 99, 98, 95, 92, 88, 83, 78, 72, 65, 58, 51, 44, 37, 30, 24, 18, 13, 9, 5, 2, 0, 0, 0, 1, 3, 6, 10, 15, 20, 26, 32, 39, 46, 53
};
uint8_t HARMONIC4_WAV_DATA[] = {
    50, 59, 68, 76, 84, 90, 95, 98, 99, 99, 97, 93, 88, 81, 74, 65, 56, 47, 37, 28, 20, 13, 7, 3, 0, 0, 0, 3, 7, 13, 20, 28, 37, 46, 55, 65, 73, 81, 88, 93, 97, 99, 99, 98, 95, 90, 84, 77, 68, 59, 50, 41, 32, 23, 16, 9, 5, 1, 0, 0, 2, 5, 11, 17, 25, 33, 43, 52, 61, 70, 78, 85, 91, 96, 98, 99, 99, 96, 92, 87, 80, 72, 63, 54, 44, 35, 26, 18, 12, 6, 2, 0, 0, 1, 4, 8, 15, 22, 30, 39, 48, 58, 67, 75, 83, 89, 94, 98, 99, 99, 97, 94, 89, 82, 75, 66, 57, 48, 38, 29, 21, 14, 8, 4, 1, 0, 0, 3, 7, 12, 19, 27, 36, 45, 54
};
uint8_t HARMONIC5_WAV_DATA[] = {
    50, 61, 72, 82, 90, 96, 99, 99, 97, 92, 85, 76, 65, 54, 42, 31, 20, 12, 5, 1, 0, 1, 5, 11, 20, 30, 41, 53, 65, 75, 85, 92, 97, 99, 99, 96, 90, 82, 73, 62, 50, 38, 27, 17, 9, 4, 0, 0, 2, 7, 14, 23, 33, 45, 57, 68, 78, 87, 94, 98, 99, 98, 94, 88, 80, 69, 58, 46, 35, 24, 15, 7, 2, 0, 0, 3, 8, 16, 26, 37, 48, 60, 71, 81, 89, 95, 99, 99, 97, 93, 86, 77, 66, 55, 43, 32, 21, 12, 6, 1, 0, 1, 4, 11, 19, 29, 40, 52, 64, 74, 84, 91, 97, 99, 99, 96, 91, 83, 74, 63, 51, 39, 28, 18, 10, 4, 0, 0, 1, 6, 13, 22, 32, 44, 56
};
uint8_t HARMONIC6_WAV_DATA[] = {
    50, 63, 76, 87, 95, 99, 99, 95, 88, 78, 65, 51, 37, 24, 13, 5, 0, 0, 3, 10, 20, 32, 46, 60, 73, 85, 93, 98, 99, 97, 90, 81, 68, 55, 41, 27, 16, 7, 1, 0, 2, 8, 17, 29, 43, 57, 70, 82, 91, 97, 99, 98, 92, 83, 72, 58, 44, 30, 18, 9, 2, 0, 1, 6, 15, 26, 39, 53, 67, 79, 89, 96, 99, 99, 94, 86, 75, 62, 48, 34, 21, 11, 4, 0, 0, 4, 12, 23, 36, 50, 64, 76, 87, 95, 99, 99, 95, 88, 78, 65, 51, 37, 24, 13, 5, 0, 0, 3, 10, 20, 32, 46, 60, 73, 85, 93, 98, 99, 97, 90, 80, 68, 55, 40, 27, 16, 7, 1, 0, 2, 8, 17, 29, 43, 57
};
uint8_t HARMONIC7_WAV_DATA[] = {
    50, 66, 80, 91, 98, 99, 95, 87, 74, 58, 42, 26, 13, 4, 0, 1, 7, 18, 32, 48, 65, 79, 91, 98, 99, 96, 87, 75, 59, 43, 27, 14, 5, 0, 1, 7, 17, 31, 47, 64, 78, 90, 97, 99, 96, 88, 76, 60, 44, 28, 15, 5, 0, 0, 6, 16, 30, 46, 62, 77, 89, 97, 99, 97, 89, 77, 62, 45, 29, 16, 6, 0, 0, 5, 15, 29, 45, 61, 76, 89, 97, 99, 97, 89, 78, 63, 46, 30, 16, 6, 0, 0, 5, 15, 28, 44, 60, 75, 88, 96, 99, 97, 90, 79, 64, 47, 31, 17, 7, 1, 0, 4, 14, 27, 43, 59, 74, 87, 96, 99, 98, 91, 79, 65, 49, 33, 18, 7, 1, 0, 4, 13, 26, 42, 58
};
uint8_t HARMONIC8_WAV_DATA[] = {
    50, 68, 84, 95, 99, 97, 88, 74, 56, 37, 20, 7, 0, 0, 7, 20, 37, 55, 73, 88, 97, 99, 95, 84, 68, 50, 32, 16, 5, 0, 2, 11, 25, 43, 61, 78, 91, 98, 99, 92, 80, 63, 44, 26, 12, 2, 0, 4, 15, 30, 48, 67, 83, 94, 99, 97, 89, 75, 57, 38, 21, 8, 1, 0, 7, 19, 36, 54, 72, 87, 97, 99, 95, 85, 69, 51, 33, 16, 5, 0, 1, 10, 24, 42, 60, 77, 91, 98, 99, 93, 80, 64, 45, 27, 12, 3, 0, 3, 14, 29, 47, 66, 82, 94, 99, 98, 89, 76, 58, 39, 22, 9, 1, 0, 6, 18, 35, 53, 71, 86, 96, 99, 96, 86, 70, 52, 34, 17, 5, 0, 1, 9, 23, 40, 59
};
uint8_t HARMONIC9_WAV_DATA[] = {
    50, 70, 87, 97, 99, 92, 78, 58, 37, 18, 5, 0, 3, 14, 32, 53, 73, 89, 98, 99, 90, 75, 55, 34, 16, 4, 0, 4, 17, 36, 57, 76, 91, 99, 98, 88, 72, 51, 30, 13, 2, 0, 6, 20, 39, 60, 79, 93, 99, 97, 86, 68, 48, 27, 11, 1, 0, 8, 23, 43, 64, 82, 95, 99, 95, 83, 65, 44, 24, 9, 0, 1, 10, 26, 46, 67, 85, 96, 99, 94, 80, 62, 40, 21, 7, 0, 2, 12, 29, 50, 70, 87, 97, 99, 92, 78, 58, 37, 18, 5, 0, 3, 15, 32, 53, 73, 89, 98, 98, 90, 74, 54, 34, 15, 3, 0, 4, 17, 36, 57, 77, 91, 99, 98, 88, 71, 51, 30, 13, 2, 0, 6, 20, 39, 60
};
uint8_t HARMONIC10_WAV_DATA[] = {
    50, 72, 90, 99, 97, 85, 65, 42, 20, 5, 0, 5, 20, 41, 65, 85, 97, 99, 90, 73, 50, 27, 9, 0, 2, 14, 33, 57, 78, 94, 99, 94, 80, 58, 35, 15, 2, 0, 8, 26, 48, 71, 89, 99, 97, 86, 66, 43, 21, 6, 0, 4, 19, 40, 64, 84, 97, 99, 91, 74, 51, 28, 10, 0, 1, 13, 32, 56, 77, 93, 99, 95, 80, 59, 36, 16, 3, 0, 8, 25, 47, 70, 89, 98, 98, 86, 67, 44, 22, 6, 0, 4, 18, 39, 63, 83, 96, 99, 91, 74, 52, 29, 11, 1, 1, 12, 31, 55, 77, 93, 99, 95, 81, 60, 37, 16, 3, 0, 7, 24, 46, 69, 88, 98, 98, 87, 68, 45, 23, 7, 0, 3, 17, 38, 62
};
uint8_t HARMONIC11_WAV_DATA[] = {
    50, 74, 93, 99, 93, 76, 51, 26, 7, 0, 5, 22, 46, 71, 91, 99, 95, 79, 55, 29, 9, 0, 3, 19, 43, 68, 89, 99, 96, 81, 58, 33, 12, 0, 2, 16, 39, 65, 86, 98, 97, 84, 62, 36, 14, 1, 1, 14, 36, 61, 84, 97, 98, 86, 65, 39, 16, 2, 0, 11, 32, 58, 81, 96, 99, 89, 68, 43, 19, 3, 0, 9, 29, 54, 78, 95, 99, 91, 71, 46, 22, 5, 0, 7, 26, 51, 76, 93, 99, 93, 74, 50, 25, 7, 0, 6, 23, 47, 72, 91, 99, 94, 77, 53, 28, 9, 0, 4, 20, 44, 69, 89, 99, 96, 80, 57, 31, 11, 0, 3, 17, 41, 66, 87, 99, 97, 83, 60, 35, 13, 1, 2, 15, 37, 63
};
uint8_t HARMONIC12_WAV_DATA[] = {
    50, 76, 95, 99, 88, 65, 37, 13, 0, 3, 20, 46, 73, 93, 99, 90, 68, 41, 16, 1, 2, 17, 43, 70, 91, 99, 92, 72, 44, 18, 2, 1, 15, 39, 67, 89, 99, 94, 75, 48, 21, 4, 0, 12, 36, 64, 87, 99, 95, 78, 51, 24, 5, 0, 10, 32, 60, 85, 98, 97, 80, 55, 27, 7, 0, 8, 29, 57, 82, 97, 98, 83, 58, 30, 9, 0, 6, 26, 53, 79, 96, 98, 86, 61, 34, 11, 0, 4, 23, 50, 77, 95, 99, 88, 65, 37, 13, 0, 3, 20, 46, 74, 93, 99, 90, 68, 40, 15, 1, 2, 17, 43, 70, 91, 99, 92, 71, 44, 18, 2, 1, 15, 39, 67, 90, 99, 94, 74, 47, 21, 3, 0, 12, 36, 64
};
uint8_t HARMONIC13_WAV_DATA[] = {
    50, 78, 97, 98, 81, 54, 24, 4, 0, 14, 41, 71, 93, 99, 87, 62, 32, 8, 0, 9, 33, 64, 89, 99, 92, 69, 39, 13, 0, 5, 26, 55, 83, 98, 96, 77, 48, 19, 2, 2, 19, 47, 76, 96, 98, 83, 56, 26, 5, 0, 13, 39, 69, 92, 99, 89, 64, 34, 9, 0, 8, 31, 61, 87, 99, 93, 71, 42, 15, 0, 4, 24, 53, 81, 98, 97, 78, 50, 21, 3, 1, 17, 45, 75, 95, 99, 85, 58, 28, 6, 0, 12, 37, 67, 91, 99, 90, 66, 36, 11, 0, 7, 29, 59, 86, 99, 94, 73, 44, 16, 1, 3, 22, 51, 80, 97, 97, 80, 52, 23, 3, 1, 16, 43, 73, 94, 99, 86, 60, 30, 7, 0, 10, 35, 65
};
const float lowestFrequency=37.927;
const float highestFrequency=1380.864;
const float freqList[]={
   40.455, 43.152, 45.512, 48.546, 50.569, 53.94, 56.89, 60.682, 64.728, 67.425, 72.819, 75.853, 80.91, 86.304, 91.024, 97.092, 101.137, 107.88, 113.78, 121.365, 129.456, 134.85, 145.638, 151.706, 161.82, 172.608, 182.047, 194.184, 202.275, 215.76, 227.559, 242.73, 258.912, 269.7, 291.276, 303.412, 323.64, 345.216, 364.095, 388.368, 404.55, 431.52, 455.119, 485.46, 517.824, 539.4, 582.552, 606.825, 647.28, 690.432, 728.19, 776.736, 809.1, 863.04, 910.237, 970.92, 1035.648, 1078.8, 1165.104, 1213.65, 1294.56
};
