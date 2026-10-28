/**
 * @file output.c
 * @author Sönke Wulf , Taher Keshavarzian
 * @date Mar 2016
 *
 * @brief
 */
#define E_DEFAULT_ERROR -1
#define E_STACK_OVERFLOW -2
#define E_STACK_UNDERFLOW -3
#define E_ZERO_DIVISION -4
#define E_OUT_OF_RANGE -5

void print_error(int const *status);

void print_int(int input);

void clear_output();

// EOF