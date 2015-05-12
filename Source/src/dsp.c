// local includes
#include <dsp.h>

// arm cmsis library includes
#define ARM_MATH_CM4
#include "stm32f4xx.h"
#include <arm_math.h>

// arm c library includes
#include <stdbool.h>

// the user button switch
extern volatile int user_mode;
int old_user_mode;

#define NUM_FIR_TAPS 48
#define BLOCKSIZE 8
#define L 512

// allocate the buffer signals and the filter coefficients on the heap
arm_fir_instance_q15 DTMF_FIR;
q15_t outSignalQ15[L];
q15_t hfir_coeffs_q15_lp[NUM_FIR_TAPS] = {870, 4105, 1067, 1762, 1385, 995, 395, -287, -972, -1546,
	                                      -1890, -1912, -1547, -769, 389, 1851, 3490, 5150, 6671, 7888,
	                                       8682, 8952, 8682, 7888, 6671, 5150, 3490, 1851, 389, -769,
                                        -1547, -1912, -1890, -1546, -972, -287, 395, 995, 1385, 1762,
                                         1067, 4105, 870, 0, 0, 0, 0, 0};  // low pass at 1KHz
q15_t hfir_coeffs_q15_hp[NUM_FIR_TAPS] = {5945, -2431, -2150, -1949, -1735, -1436, -1012, -459, 183, 839,
                                          1406, 1774, 1829, 1517, 772, -359, -1812, -3471, -5173, -6742,
                                         -8001, -8819, 32767, -8819, -8001, -6742, -5173, -3471, -1812, -359,
                                          772, 1517, 1829, 1774, 1406, 839, 183, -459, -1012, -1436,
                                         -1735, -1949, -2150, -2431, 5945}; // high pass at 1kHz
q15_t hfir_state_q15[NUM_FIR_TAPS + BLOCKSIZE] = {0};
bool firstStart = false;

// the core dsp function
void dsp(int16_t* buffer, int length)
{
	// only enable the filter if the user button is pressed
	if (user_mode & 1)
	{
	  int i;
	  // we initiate the filter only if needed to prevent clitches at the beginning of new buffers
		if (firstStart == false || old_user_mode != user_mode)
		{
			initFilter();
			old_user_mode = user_mode;
			firstStart = true;
		}
		
  	// process with FIR
	  for(i = 0; i < length; i += BLOCKSIZE) 
	    arm_fir_fast_q15(&DTMF_FIR, buffer + i, outSignalQ15 + i, BLOCKSIZE);

  	// copy the result
	  for(i = 0; i < length; i++)
      buffer[i] = outSignalQ15[i];
  }
}

// we initialize and switch the filter here
void initFilter()
{
	// apply the low pass filter
	if (user_mode & 1)
		arm_fir_init_q15(&DTMF_FIR, NUM_FIR_TAPS, &hfir_coeffs_q15_lp[0], &hfir_state_q15[0], BLOCKSIZE);
	// or applay the high pass filter depending on the user button switch mode
	if (user_mode & 2)
		arm_fir_init_q15(&DTMF_FIR, NUM_FIR_TAPS, &hfir_coeffs_q15_hp[0], &hfir_state_q15[0], BLOCKSIZE);
}

/* --> Below the previous filter implementation with the arm cmsis library
// local includes
#include "dsp.h"
 
// the user button switch
extern volatile int user_mode;
 
// our core dsp function
void dsp(int16_t* buffer, int length)
{
  // initialize some values
  static float previous;
  int i;
     
  // if switched on, apply the filter
  if (user_mode & 1)
  {     
    // perform an simple first order high pass with 12dB/octave
    for (i=0; i<length; i++)
    {
      buffer[i] = (int16_t)( (float)buffer[i] -(float)previous * 0.75f );
      previous = buffer[i];
    }
  }
}
*/
