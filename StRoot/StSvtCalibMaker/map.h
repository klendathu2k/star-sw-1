//
#define LADDERS1 8
#define LADDERS2 12
#define LADDERS3 16
#define WAFERS1 4
#define WAFERS2 6
#define WAFERS3 7
#define HYBRIDS 2
#define INDEX 3

int key_barrel1[LADDERS1][WAFERS1][HYBRIDS][INDEX] = 
{
 {
  {{12,0,2},{12,0,5}},  {{12,1,0},{12,1,3}},  {{0,1,0},{0,1,3}},  {{0,0,2},{0,0,5}}
 },
 {
  {{13,0,2},{13,0,5}},  {{13,1,0},{13,1,3}},  {{1,1,0},{1,1,3}},  {{1,0,2},{1,0,5}}
 },
 {
  {{15,0,2},{15,0,5}},  {{15,1,0},{15,1,3}},  {{3,1,0},{3,1,3}},  {{3,0,2},{3,0,5}}
 },
 {
  {{16,0,2},{16,0,5}},  {{16,1,0},{16,1,3}},  {{4,1,0},{4,1,3}},  {{4,0,2},{4,0,5}}
 },
 {
  {{18,0,2},{18,0,5}},  {{18,1,0},{18,1,3}},  {{6,1,0},{6,1,3}},  {{6,0,2},{6,0,5}}
 },
 {
  {{19,0,2},{19,0,5}},  {{19,1,0},{19,1,3}},  {{7,1,0},{7,1,3}},  {{7,0,2},{7,0,5}}
 },
 {
  {{21,0,2},{21,0,5}},  {{21,1,0},{21,1,3}},  {{9,1,0},{9,1,3}},  {{9,0,2},{9,0,5}}
 },
 {
  {{22,0,2},{22,0,5}},  {{22,1,0},{22,1,3}},  {{10,1,0},{10,1,3}},  {{10,0,2},{10,0,5}}
 }
};

int key_barrel2[LADDERS2][WAFERS2][HYBRIDS][INDEX] = 
{
 {
  {{14,0,0},{14,0,3}},  {{14,0,1},{14,0,4}},  {{14,2,0},{14,2,3}},  {{2,2,0},{2,2,3}},  {{2,0,1},{2,0,4}},  {{2,0,0},{2,0,3}}
 },
 {
  {{14,0,2},{14,0,5}},  {{14,1,0},{14,1,3}},  {{14,2,1},{14,2,4}},  {{2,2,1},{2,2,4}},  {{2,1,0},{2,1,3}},  {{2,0,2},{2,0,5}}
 },
 {
  {{14,1,1},{14,1,4}},  {{14,1,2},{14,1,5}},  {{14,2,2},{14,2,5}},  {{2,2,2},{2,2,5}},  {{2,1,2},{2,1,5}},  {{2,1,1},{2,1,4}}
 },
 {
  {{17,0,0},{17,0,3}},  {{17,0,1},{17,0,4}},  {{17,2,0},{17,2,3}},  {{5,2,0},{5,2,3}},  {{5,0,1},{5,0,4}},  {{5,0,0},{5,0,3}}
 },
 {
  {{17,0,2},{17,0,5}},  {{17,1,0},{17,1,3}},  {{17,2,1},{17,2,4}},  {{5,2,1},{5,2,4}},  {{5,1,0},{5,1,3}},  {{5,0,2},{5,0,5}}
 },
 {
  {{17,1,1},{17,1,4}},  {{17,1,2},{17,1,5}},  {{17,2,2},{17,2,5}},  {{5,2,2},{5,2,5}},  {{5,1,2},{5,1,5}},  {{5,1,1},{5,1,4}}
 },
 {
  {{20,0,0},{20,0,3}},  {{20,0,1},{20,0,4}},  {{20,2,0},{20,2,3}},  {{8,2,0},{8,2,3}},  {{8,0,1},{8,0,4}},  {{8,0,0},{8,0,3}}
 },
 {
  {{20,0,2},{20,0,5}},  {{20,1,0},{20,1,3}},  {{20,2,1},{20,2,4}},  {{8,2,1},{8,2,4}},  {{8,1,0},{8,1,3}},  {{8,0,2},{8,0,5}}
 },
 {
  {{20,1,1},{20,1,4}},  {{20,1,2},{20,1,5}},  {{20,2,2},{20,2,5}},  {{8,2,2},{8,2,5}},  {{8,1,2},{8,1,5}},  {{8,1,1},{8,1,4}}
 },
 {
  {{23,0,0},{23,0,3}},  {{23,0,1},{23,0,4}},  {{23,2,0},{23,2,3}},  {{11,2,0},{11,2,3}},  {{11,0,1},{11,0,4}},  {{11,0,0},{11,0,3}}
 },
 {
  {{23,0,2},{23,0,5}},  {{23,1,0},{23,1,3}},  {{23,2,1},{23,2,4}},  {{11,2,1},{11,2,4}},  {{11,1,0},{11,1,3}},  {{11,0,2},{11,0,5}}
 },
 {
  {{23,1,1},{23,1,4}},  {{23,1,2},{23,1,5}},  {{23,2,2},{23,2,5}},  {{11,2,2},{11,2,5}},  {{11,1,2},{11,1,5}},  {{11,1,1},{11,1,4}}
 }
};

int key_barrel3[LADDERS3][WAFERS3][HYBRIDS][INDEX] = 
{
 {
  {{12,1,1},{12,1,4}},  {{12,1,2},{12,1,5}},  {{12,2,2},{12,2,5}},  {{0,2,1},{0,2,4}},  {{0,2,0},{0,2,3}},  {{0,0,1},{0,0,4}},  {{0,0,0},{0,0,3}}
 },
 {
  {{12,0,0},{12,0,3}},  {{12,0,1},{12,0,4}},  {{12,2,0},{12,2,3}},  {{12,2,1},{12,2,4}},  {{0,2,2},{0,2,5}},  {{0,1,2},{0,1,5}},  {{0,1,1},{0,1,4}}
 },
 {
  {{13,1,1},{13,1,4}},  {{13,1,2},{13,1,5}},  {{13,2,2},{13,2,5}},  {{1,2,1},{1,2,4}},  {{1,2,0},{1,2,3}},  {{1,0,1},{1,0,4}},  {{1,0,0},{1,0,3}}
 },
 {
  {{13,0,0},{13,0,3}},  {{13,0,1},{13,0,4}},  {{13,2,0},{13,2,3}},  {{13,2,1},{13,2,4}},  {{1,2,2},{1,2,5}},  {{1,1,2},{1,1,5}},  {{1,1,1},{1,1,4}}
 },
 {
  {{15,1,1},{15,1,4}},  {{15,1,2},{15,1,5}},  {{15,2,2},{15,2,5}},  {{3,2,1},{3,2,4}},  {{3,2,0},{3,2,3}},  {{3,0,1},{3,0,4}},  {{3,0,0},{3,0,3}}
 },
 {
  {{15,0,0},{15,0,3}},  {{15,0,1},{15,0,4}},  {{15,2,0},{15,2,3}},  {{15,2,1},{15,2,4}},  {{3,2,2},{3,2,5}},  {{3,1,2},{3,1,5}},  {{3,1,1},{3,1,4}}
 },
 {
  {{16,1,1},{16,1,4}},  {{16,1,2},{16,1,5}},  {{16,2,2},{16,2,5}},  {{4,2,1},{4,2,4}},  {{4,2,0},{4,2,3}},  {{4,0,1},{4,0,4}},  {{4,0,0},{4,0,3}}
 },
 {
  {{16,0,0},{16,0,3}},  {{16,0,1},{16,0,4}},  {{16,2,0},{16,2,3}},  {{16,2,1},{16,2,4}},  {{4,2,2},{4,2,5}},  {{4,1,2},{4,1,5}},  {{4,1,1},{4,1,4}}
 },
 {
  {{18,1,1},{18,1,4}},  {{18,1,2},{18,1,5}},  {{18,2,2},{18,2,5}},  {{6,2,1},{6,2,4}},  {{6,2,0},{6,2,3}},  {{6,0,1},{6,0,4}},  {{6,0,0},{6,0,3}}
 },
 {
  {{18,0,0},{18,0,3}},  {{18,0,1},{18,0,4}},  {{18,2,0},{18,2,3}},  {{18,2,1},{18,2,4}},  {{6,2,2},{6,2,5}},  {{6,1,2},{6,1,5}},  {{6,1,1},{6,1,4}}
 },
 {
  {{19,1,1},{19,1,4}},  {{19,1,2},{19,1,5}},  {{19,2,2},{19,2,5}},  {{7,2,1},{7,2,4}},  {{7,2,0},{7,2,3}},  {{7,0,1},{7,0,4}},  {{7,0,0},{7,0,3}}
 },
 {
  {{19,0,0},{19,0,3}},  {{19,0,1},{19,0,4}},  {{19,2,0},{19,2,3}},  {{19,2,1},{19,2,4}},  {{7,2,2},{7,2,5}},  {{7,1,2},{7,1,5}},  {{7,1,1},{7,1,4}}
 },
 {
  {{21,1,1},{21,1,4}},  {{21,1,2},{21,1,5}},  {{21,2,2},{21,2,5}},  {{9,2,1},{9,2,4}},  {{9,2,0},{9,2,3}},  {{9,0,1},{9,0,4}},  {{9,0,0},{9,0,3}}
 },
 {
  {{21,0,0},{21,0,3}},  {{21,0,1},{21,0,4}},  {{21,2,0},{21,2,3}},  {{21,2,1},{21,2,4}},  {{9,2,2},{9,2,5}},  {{9,1,2},{9,1,5}},  {{9,1,1},{9,1,4}}
 },
 {
  {{22,1,1},{22,1,4}},  {{22,1,2},{22,1,5}},  {{22,2,2},{22,2,5}},  {{10,2,1},{10,2,4}},  {{10,2,0},{10,2,3}},  {{10,0,1},{10,0,4}},  {{10,0,0},{10,0,3}}
 },
 {
  {{22,0,0},{22,0,3}},  {{22,0,1},{22,0,4}},  {{22,2,0},{22,2,3}},  {{22,2,1},{22,2,4}},  {{10,2,2},{10,2,5}},  {{10,1,2},{10,1,5}},  {{10,1,1},{10,1,4}}
 }
};