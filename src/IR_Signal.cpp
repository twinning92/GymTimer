#include "IR_Signal.h"

void IR_Signal::setup_ir()
{
	irrecv.enableIRIn();
	IR_queue = xQueueCreate(20, sizeof(uint16_t));
}

IRData IR_Signal::get_from_queue()
{
    xQueueReceive(this->IR_queue, &this->ir_data, 10);
    return this->ir_data;
}

void IR_Signal::enqueue_ir_commands()
{
	if (IrReceiver.decode())
	{
		this->ir_data = IrReceiver.decodedIRData;
		xQueueSend(IR_queue, &(this->ir_data), 0);
	}
	IrReceiver.resume();
}
