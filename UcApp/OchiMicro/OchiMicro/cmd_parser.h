/*
 * cmd_parser.h
 *
 * Created: 7/31/2016 12:41:44 AM
 *  Author: John Smith
 */ 


#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include "io_usart.h"

class Cmd_Parser
{
public:
	Cmd_Parser(Io_Usart *io_dev);

	char buffer[32];
	unsigned int buff_cnt;
protected:
private:
	void receive_char(void *structure, char data);
	Io_Usart *io_dev;
};



#endif /* CMD_PARSER_H_ */