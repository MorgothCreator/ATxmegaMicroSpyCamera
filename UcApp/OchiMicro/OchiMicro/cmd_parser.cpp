/*
 * cmd_parser.cpp
 *
 * Created: 7/31/2016 12:41:29 AM
 *  Author: John Smith
 */ 

 #include <string.h>
 #include "config.h"
 #include "cmd_parser.h"
 #include "io_usart.h"



void receive_char_service(void *structure, char data)
{
	Cmd_Parser *_cmd_parser = (Cmd_Parser *)structure;
	if(_cmd_parser->buff_cnt < sizeof(_cmd_parser->buffer));
}

Cmd_Parser::Cmd_Parser(Io_Usart *io_dev)
{
	this->io_dev = io_dev;
	buff_cnt = 0;
	io_dev->rx_char_receive_func = receive_char_service;
}

void Cmd_Parser::receive_char(void *structure, char data)
{
	receive_char_service(this, data);
}