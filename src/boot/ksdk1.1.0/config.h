/*
	Authored 2021, Phillip Stanley-Marbell.

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/

#define		WARP_BUILD_ENABLE_SEGGER_RTT_PRINTF		1
#define		WARP_BUILD_BOOT_TO_CSVSTREAM			0
#define		WARP_BUILD_BOOT_TO_VLPR				1

#define		WARP_BUILD_ENABLE_FRDMKL03			1

#if (WARP_BUILD_ENABLE_FRDMKL03)
	#define		WARP_BUILD_ENABLE_DEVMMA8451Q		1
	#define		WARP_BUILD_ENABLE_DEVRV8803C7		0
	#define     WARP_BUILD_ENABLE_DEVSSD1331    1
	#define     WARP_BUILD_ENABLE_DEVINA219     1
#endif

typedef enum
{
	/*
	 *	Speeds
	 */
	kWarpDefaultI2cBaudRateKbps				= 200,
	kWarpDefaultUartBaudRateBps				= 115200,
	kWarpDefaultSpiBaudRateKbps				= 10000,

	/*
	 *	Times
	 */
	kWarpDefaultSleeptimeSeconds				= 0,
	kWarpDefaultI2cTimeoutMilliseconds			= 5,
	kWarpDefaultUartTimeoutMilliseconds			= 1000,
	kWarpDefaultSpiTimeoutMicroseconds			= 5,
	kWarpDefaultMenuPrintDelayMilliseconds			= 10,
	kWarpDefaultSupplySettlingDelayMilliseconds		= 1,

	/*
	 *	Sizes
	*/
	kWarpDefaultPrintBufferSizeBytes			= 64,
	kWarpMemoryCommonSpiBufferBytes				= 64,
	kWarpSizesI2cBufferBytes				= 4,
	kWarpSizesSpiBufferBytes				= 7,
	kWarpSizesUartBufferBytes				= 8,

	/*
	 *	Voltages
	 */
	kWarpDefaultSupplyVoltageMillivolts			= 1800,
	kWarpDefaultSupplyVoltageMillivoltsMMA8451Q		= 1800,
	kWarpDefaultSupplyVoltageMillivoltsINA219		= 1800,
	kWarpDefaultSupplyVoltageMillivoltsRV8803C7		= 1800,
} WarpDefaults;
