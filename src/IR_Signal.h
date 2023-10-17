#pragma once
#include "config.h"
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>



enum IR_Command : uint8_t
{
	IR_NONE = 0x00,
	IR_UP = 0x18,
	IR_DOWN = 0x52,
	IR_LEFT = 0x08,
	IR_RIGHT = 0x5a,
	IR_OK = 0x1c,
	IR_BACK = 0x16,
	IR_NIL = 0x0D,
	IR_1 = 0x45,
	IR_2 = 0x46,
	IR_3 = 0x47,
	IR_4 = 0x44,
	IR_5 = 0x40,
	IR_6 = 0x43,
	IR_7 = 0x07,
	IR_8 = 0x15,
	IR_9 = 0x09,
	IR_0 = 0x19,
};

class IR_Signal
{
public:
	IR_Signal() {}

	void begin()
	{
		pinMode(IR_RECEIVER_PIN, INPUT);
		IrReceiver.setReceivePin(IR_RECEIVER_PIN);
		IrReceiver.start();
	}

	IR_Command get_ir_command()
	{
		if (IrReceiver.decode())
		{
			IRData ir_data = IrReceiver.decodedIRData;
			IrReceiver.resume();
			if (ir_data.flags & IRDATA_FLAGS_IS_REPEAT)
			{
				return IR_NONE;
			}
			return static_cast<IR_Command>(ir_data.command);
		}
		else
		{
			return IR_NONE;
		}
	}
};

extern IR_Signal ir;
