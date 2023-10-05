#include "IR_Signal.h"

void IR_Signal::setup_ir()
{
	irrecv.enableIRIn();
	IR_queue = xQueueCreate(20, sizeof(IRData));
}

IRData IR_Signal::get_from_queue()
{
	this->ir_data.command = IR_NIL;
	xQueueReceive(this->IR_queue, &this->ir_data, 10);
	return this->ir_data;
}

void IR_Signal::enqueue_ir_commands()
{
	if (IrReceiver.decode())
	{
		this->ir_data = IrReceiver.decodedIRData;
		Serial.printf("Adding to IR Queue: %d\n", this->ir_data.command);
		xQueueSend(IR_queue, &(this->ir_data), 0);
	}
	IrReceiver.resume();
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
