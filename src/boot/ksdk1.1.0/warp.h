#include "fsl_spi_master_driver.h"

#define	min(x,y)	((x) < (y) ? (x) : (y))
#define	max(x,y)	((x) > (y) ? (x) : (y))
#define	USED(x)		(void)(x)

/*
 *	On Glaux, we use PTA0/IRQ0/LLWU_P7 (SWD_CLK) as the interrupt line
 *	for the RV8803C7 RTC. The original version of this function for the
 *	FRDMKL03 was using PTB0.
 *
 *	The following taken from KSDK_1.1.0//boards/frdmkl03z/board.h. We
 *	don't include that whole file verbatim since we have a custom board.
 */
#define BOARD_SW_HAS_LLWU_PIN		1
#define BOARD_SW_LLWU_EXT_PIN		7
#define BOARD_SW_LLWU_PIN		0
#define BOARD_SW_LLWU_BASE		PORTA_BASE
#define BOARD_SW_LLWU_IRQ_HANDLER	PORTA_IRQHandler
#define BOARD_SW_LLWU_IRQ_NUM		PORTA_IRQn

typedef enum
{
	kWarpStatusOK			= 0,

	kWarpStatusDeviceNotInitialized,
	kWarpStatusDeviceCommunicationFailed,
	kWarpStatusBadDeviceCommand,

	/*
	 *	Generic comms error
	 */
	kWarpStatusCommsError,

	/*
	 *	Power mode routines
	 */
	kWarpStatusPowerTransitionErrorVlpr2Wait,
	kWarpStatusPowerTransitionErrorVlpr2Stop,
	kWarpStatusPowerTransitionErrorRun2Vlpw,
	kWarpStatusPowerTransitionErrorVlpr2Vlpr,
	kWarpStatusErrorPowerSysSetmode,
	kWarpStatusBadPowerModeSpecified,

	/*
	 *	Always keep this as the last item.
	 */
	kWarpStatusMax
} WarpStatus;

typedef enum
{
	/*
	 *	NOTE: This order is depended on by POWER_SYS_SetMode()
	 *
	 *	See KSDK13APIRM.pdf Section 55.5.3
	 */
	kWarpPowerModeWAIT,
	kWarpPowerModeSTOP,
	kWarpPowerModeVLPR,
	kWarpPowerModeVLPW,
	kWarpPowerModeVLPS,
	kWarpPowerModeVLLS0,
	kWarpPowerModeVLLS1,
	kWarpPowerModeVLLS3,
	kWarpPowerModeRUN,
} WarpPowerMode;

typedef enum
{
	kWarpSensorMMA8451Q,
	kWarpSensorINA219,
	kWarpSensorBME680,
	kWarpSensorTMP006B,
	kWarpSensorMAG3110,
	kWarpSensorL3GD20H,
	kWarpSensorLPS25H,
	kWarpSensorTCS34725,
	kWarpSensorSI4705,
	kWarpSensorSI7021,
	kWarpSensorPAN1326,
	kWarpSensorSCD30,
} WarpSensorDevice;

typedef enum
{
	kWarpModeDisableAdcOnSleep		= (1 << 0),
} WarpModeMask;

typedef enum
{
	kWarpSensorConfigurationRegisterMMA8451QF_SETUP			= 0x09,
	kWarpSensorConfigurationRegisterMMA8451QCTRL_REG1		= 0x2A,

    kWarpSensorConfigurationRegisterINA219CONFIG			= 0x00,
	kWarpSensorConfigurationRegisterINA219CALIBRATE		    = 0x05,

	kWarpSensorConfigurationRegisterMAG3110CTRL_REG1		= 0x10,
	kWarpSensorConfigurationRegisterMAG3110CTRL_REG2		= 0x11,

	kWarpSensorConfigurationRegisterL3GD20HCTRL1			= 0x20,
	kWarpSensorConfigurationRegisterL3GD20HCTRL2			= 0x21,
	kWarpSensorConfigurationRegisterL3GD20HCTRL5			= 0x24,
	
} WarpSensorConfigurationRegister;

typedef enum
{
	kWarpSensorOutputRegisterMMA8451QOUT_X_MSB			= 0x01,
	kWarpSensorOutputRegisterMMA8451QOUT_X_LSB			= 0x02,
	kWarpSensorOutputRegisterMMA8451QOUT_Y_MSB			= 0x03,
	kWarpSensorOutputRegisterMMA8451QOUT_Y_LSB			= 0x04,
	kWarpSensorOutputRegisterMMA8451QOUT_Z_MSB			= 0x05,
	kWarpSensorOutputRegisterMMA8451QOUT_Z_LSB			= 0x06,
	
	kWarpSensorOutputRegisterINA219OUT_Current          = 0x04,

	kWarpSensorOutputRegisterMAG3110OUT_X_MSB			= 0x01,
	kWarpSensorOutputRegisterMAG3110OUT_X_LSB			= 0x02,
	kWarpSensorOutputRegisterMAG3110OUT_Y_MSB			= 0x03,
	kWarpSensorOutputRegisterMAG3110OUT_Y_LSB			= 0x04,
	kWarpSensorOutputRegisterMAG3110OUT_Z_MSB			= 0x05,
	kWarpSensorOutputRegisterMAG3110OUT_Z_LSB			= 0x06,
	kWarpSensorOutputRegisterMAG3110DIE_TEMP			= 0x0F,

	kWarpSensorOutputRegisterL3GD20HOUT_TEMP			= 0x26,
	kWarpSensorOutputRegisterL3GD20HOUT_X_L				= 0x28,
	kWarpSensorOutputRegisterL3GD20HOUT_X_H				= 0x29,
	kWarpSensorOutputRegisterL3GD20HOUT_Y_L				= 0x2A,
	kWarpSensorOutputRegisterL3GD20HOUT_Y_H				= 0x2B,
	kWarpSensorOutputRegisterL3GD20HOUT_Z_L				= 0x2C,
	kWarpSensorOutputRegisterL3GD20HOUT_Z_H				= 0x2D,

	kWarpSensorOutputRegisterBME680press_msb			= 0x1F,
	kWarpSensorOutputRegisterBME680press_lsb			= 0x20,
	kWarpSensorOutputRegisterBME680press_xlsb			= 0x21,
	kWarpSensorOutputRegisterBME680temp_msb				= 0x22,
	kWarpSensorOutputRegisterBME680temp_lsb				= 0x23,
	kWarpSensorOutputRegisterBME680temp_xlsb			= 0x24,
	kWarpSensorOutputRegisterBME680hum_msb				= 0x25,
	kWarpSensorOutputRegisterBME680hum_lsb				= 0x26,

} WarpSensorOutputRegister;

typedef enum
{
	kWarpMiscMarkerForAbsentByte					= 0xFF,
} WarpMisc;

typedef struct
{
	bool			isInitialized;

	uint8_t			i2cAddress;
	uint8_t			i2cBuffer[kWarpSizesI2cBufferBytes];
	uint16_t		operatingVoltageMillivolts;
} WarpI2CDeviceState;

typedef struct
{
	bool			isInitialized;

	/*
	 *	For holding the SPI CS I/O pin idnetifier to make
	 *	the driver independent of board config.
	 */
	int			chipSelectIoPinID;

	uint8_t *		spiSourceBuffer;
	uint8_t *		spiSinkBuffer;
	size_t			spiBufferLength;
	uint16_t		operatingVoltageMillivolts;
} WarpSPIDeviceState;

typedef struct
{
	bool			isInitialized;
	uint8_t			uartTXBuffer[kWarpSizesUartBufferBytes];
	uint8_t			uartRXBuffer[kWarpSizesUartBufferBytes];
	uint16_t		operatingVoltageMillivolts;
} WarpUARTDeviceState;

typedef struct
{
	uint8_t			errorCount;
} WarpPowerManagerCallbackStructure;

void		warpScaleSupplyVoltage(uint16_t voltageMillivolts);
void		warpDisableSupplyVoltage(void);
WarpStatus	warpSetLowPowerMode(WarpPowerMode powerMode, uint32_t sleepSeconds);
void		warpEnableI2Cpins(void);
void		warpDisableI2Cpins(void);
void		warpEnableSPIpins(void);
void		warpDisableSPIpins(void);
void		warpDeasserAllSPIchipSelects(void);
void		warpPrint(const char *fmt, ...);
int		warpWaitKey(void);
