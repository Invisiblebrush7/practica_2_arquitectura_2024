#ifndef RIPES_IO_HEADER
#define RIPES_IO_HEADER
// *****************************************************************************
// * LED_MATRIX_0
// *****************************************************************************
#define LED_MATRIX_0_BASE	(0xf0000000)
#define LED_MATRIX_0_SIZE	(0xdac)
#define LED_MATRIX_0_WIDTH	(0x23)
#define LED_MATRIX_0_HEIGHT	(0x19)


// *****************************************************************************
// * D_PAD_0
// *****************************************************************************
#define D_PAD_0_BASE	(0xf0000dac)
#define D_PAD_0_SIZE	(0x10)
#define D_PAD_0_UP_OFFSET	(0x0)
#define D_PAD_0_UP	(0xf0000dac)
#define D_PAD_0_DOWN_OFFSET	(0x4)
#define D_PAD_0_DOWN	(0xf0000db0)
#define D_PAD_0_LEFT_OFFSET	(0x8)
#define D_PAD_0_LEFT	(0xf0000db4)
#define D_PAD_0_RIGHT_OFFSET	(0xc)
#define D_PAD_0_RIGHT	(0xf0000db8)


#endif // RIPES_IO_HEADER