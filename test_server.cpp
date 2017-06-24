/* test server to deliver a sequence of numbers at given registers
*/

#include "modbus.h"
#include <iostream>
#include <string>
#include <stdint.h>
#include <errno.h>

int main()
{
	int s = -1;
	modbus_mapping_t *mb_mapping = NULL;
	modbus_t *ctx = NULL;
	
	ctx = modbus_new_tcp("127.0.0.1", 1502);
	s = modbus_tcp_listen(ctx, 1);
	modbus_tcp_accept(ctx, &s);

	mb_mapping = modbus_mapping_new(0, 0,
									5, 0);

	mb_mapping->tab_registers[0] = 1234;
	mb_mapping->tab_registers[1] = 4321;
	mb_mapping->tab_registers[2] = 1212;
	mb_mapping->tab_registers[3] = 2121;
	mb_mapping->tab_registers[4] = 9999;;

	if (mb_mapping == NULL) {
		std::cout << "Failed to allocate the mapping " 
				  << modbus_strerror(errno)
				  << std::endl;
		modbus_free(ctx);
		return -1;
	}

	while(true) {
		uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
		int rc;

		rc = modbus_receive(ctx, query);
		if (rc > 0) {
			modbus_reply(ctx, query, rc, mb_mapping);
		} else if (rc == -1) {
			break;
		}
	}

		std::cout << modbus_strerror(errno) << std::endl;

		modbus_mapping_free(mb_mapping);

		modbus_close(ctx);
		modbus_free(ctx);	

	return 0;
}
