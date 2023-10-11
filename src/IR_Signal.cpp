#include "IR_Signal.h"

void IR_Signal::setup_ir()
{
	irrecv.enableIRIn();
	IR_queue = xQueueCreate(20, sizeof(IRData));
}

uint16_t* IR_Signal::get_ir_command()
{
	if (IrReceiver.decode())
	{
		this->ir_data = IrReceiver.decodedIRData;
		IrReceiver.resume();
		if (this->ir_data.flags & IRDATA_FLAGS_IS_REPEAT)
		{
			return nullptr;
		}
		return &this->ir_data.command;
	}
	else
	{
		return nullptr;
	}
	// if (Serial.available())
	// {
	// 	switch (Serial.read())
	// 	{
	// 	case 'w':
	// 		this->ir_data.command = IR_UP;
	// 		break;
	// 	case 's':
	// 		this->ir_data.command = IR_DOWN;
	// 		break;
	// 	case 'a':
	// 		this->ir_data.command = IR_LEFT;
	// 		break;
	// 	case 'd':
	// 		this->ir_data.command = IR_RIGHT;
	// 	default:
	// 		break;
	// 	}
	// 	xQueueSend(IR_queue, &(this->ir_data), 50);
	// }
}
