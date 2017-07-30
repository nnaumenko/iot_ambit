#ifndef TESTDATA_H
#define TESTDATA_H

#define SIZE_INPUT_1 401
#define DELTA_TIME_MS_INPUT1 5
#define DECIMALS_INPUT1 3
extern const int32_t PROGMEM input1 [SIZE_INPUT_1];

#define SIZE_INPUT_2 401
#define DELTA_TIME_MS_INPUT2 5
#define DECIMALS_INPUT2 3
extern const int32_t PROGMEM input2 [SIZE_INPUT_2];

#define SIZE_INPUT_3 100
#define DELTA_TIME_MS_INPUT3 5
#define DECIMALS_INPUT3 3
extern const int32_t PROGMEM input3 [SIZE_INPUT_3];

#define MOVING_AVERAGE_A_POINTS 2
#define MOVING_AVERAGE_B_POINTS 16
#define LOWPASS_A_FREQUENCY_HZ 20
#define SQUARE_SCALE_A_DECIMALS 1
#define SQUARE_SCALE_A_X1 1
#define SQUARE_SCALE_A_Y1 50
#define SQUARE_SCALE_A_X2 5
#define SQUARE_SCALE_A_Y2 750
#define SQUARE_SCALE_A_X3 9
#define SQUARE_SCALE_A_Y3 900
#define SPLINE_SCALE_A_DECIMALS 1
#define SPLINE_SCALE_A_X1 1
#define SPLINE_SCALE_A_Y1 50
#define SPLINE_SCALE_A_X2 5
#define SPLINE_SCALE_A_Y2 750
#define SPLINE_SCALE_A_X3 9
#define SPLINE_SCALE_A_Y3 900
#define SQUARE_SCALE_A_FACTOR_DECIMALS 3
#define SQUARE_SCALE_A_A -171875
#define SQUARE_SCALE_A_B 278125
#define SQUARE_SCALE_A_C -21094


#define DECIMALS_OUTPUT1_MOVING_AVERAGE_A 3
extern const int32_t PROGMEM output1_movingAverage_a [SIZE_INPUT_1];
#define DECIMALS_OUTPUT1_MOVING_AVERAGE_B 3
extern const int32_t PROGMEM output1_movingAverage_b [SIZE_INPUT_1];
#define DECIMALS_OUTPUT1_LOWPASS_A 3
extern const int32_t PROGMEM output1_lowpass_a [SIZE_INPUT_1];

#define DECIMALS_OUTPUT2_MOVING_AVERAGE_A 3
extern const int32_t PROGMEM output2_movingAverage_a [SIZE_INPUT_2];
#define DECIMALS_OUTPUT2_MOVING_AVERAGE_B 3
extern const int32_t PROGMEM output2_movingAverage_b [SIZE_INPUT_2];
#define DECIMALS_OUTPUT2_LOWPASS_A 3
extern const int32_t PROGMEM output2_lowpass_a [SIZE_INPUT_2];

#define DECIMALS_OUTPUT3_SQUARE_SCALE_A 4
extern const int32_t PROGMEM output3_squareScale_a [SIZE_INPUT_3];

#define DECIMALS_OUTPUT3_SPLINE_SCALE_A 3
extern const int32_t PROGMEM output3_splineScale_a [SIZE_INPUT_3];


#endif
