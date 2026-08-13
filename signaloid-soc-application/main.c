/*
 *	Copyright (c) 2026, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */


#include <uxhw.h>
#include "C0HAL.h"


typedef enum
{
	kCalculateNoCommand = 0,        /* Go to idle */
	kCalculateAddition  = 1,        /* Calculate addition */

	/*
	 *	Add your own command IDs here (and a matching `case` in the switch
	 *	below). Keep them in sync with the host application.
	 */
} SignaloidSoCCommand;


int
main(void)
{
	float   inputDistributionA;
	float   inputDistributionB;
	float   result;

	float   mean;
	float   variance;

	uint32_t command;

	while (1)
	{
		/*
		 *	Set status to "waitingForCommand"
		 */
		C0HALSetStatusRegister(kSignaloidSoCStatusWaitingForCommand);

		/*
		 *	Block until command is issued
		 */
		while ((command = C0HALGetCommandRegister()) == kCalculateNoCommand) {}

		/*
		 *	Set status to inform host that calculation will start
		 */
		C0HALSetStatusRegister(kSignaloidSoCStatusCalculating);

		/*
		 *	Turn on status LED
		 */
		C0HALSetLed(true);

		switch (command)
		{
			/*
			 *	Example command. This is the core of the template: read
			 *	inputs from kC0HALInputBuffer*, compute, and write results
			 *	to kC0HALOutputBuffer*. Replace the body with your own
			 *	computation, or add more `case`s for additional commands.
			 */
			case kCalculateAddition:
				/*
				 *	Create distributional values from inputs
				 */
				inputDistributionA  = UxHwFloatUniformDist(kC0HALInputBufferFloat[0], kC0HALInputBufferFloat[1]);
				inputDistributionB  = UxHwFloatUniformDist(kC0HALInputBufferFloat[2], kC0HALInputBufferFloat[3]);

				/*
				 *	Calculate
				 */
				result = inputDistributionA + inputDistributionB;

				/*
				 *	Compute the mean (first moment) and variance
				 *	(second centralised moment) of the result.
				 */
				mean        = UxHwFloatNthMoment(result, 1);
				variance    = UxHwFloatNthMoment(result, 2);

				/*
				 *	Return the mean and variance as two floats by writing them
				 *	to the output buffer.
				 */
				kC0HALOutputBufferFloat[0]  = mean;
				kC0HALOutputBufferFloat[1]  = variance;

				/*
				 *	Set status
				 */
				C0HALSetStatusRegister(kSignaloidSoCStatusDone);
				break;

			default:
				C0HALSetStatusRegister(kSignaloidSoCStatusInvalidCommand);
				break;
		}

		/*
		 *	Turn off status LED
		 */
		C0HALSetLed(false);

		/*
		 *	Block until command is cleared
		 */
		while (C0HALGetCommandRegister() != kCalculateNoCommand) {}
	}
}
