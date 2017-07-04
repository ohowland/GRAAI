
/** Displays the Modbus configuration values held
   	in the ModbusPkguration vector<ModbusData>.
 */ 
void ModbusPkg::print() const
{
	std::cout << "----------------------------------" 
			  << std::endl
			  << "MODBUS CONFIGURATION FILE:"
			  << std::endl
		      << "NAME | Address | Datatype | Access" << std::endl;	

	std::sort(tags_.begin(), tags_.end(), sortByAddress);

	for (std::vector<ModbusTag>::const_iterator it = tags_.begin();
	     it < tags_.end(); it++)
   	{
		std::cout << it->name() << " "
				  << it->address() << " ";

		switch(it->datatype()) {
		case U16:
			std::cout << "U16 ";
			break;
		case U32:
			std::cout << "U32 ";
			break;
		case FLOAT:
			std::cout << "SINGLE FLOAT ";
			break;
		case DOUBLE:
			std::cout << "DOUBLE FLOAT ";
			break;
		default:
			std::cout << "UNDEFINED ";
			break;
		}

		switch(it->access()) {
		case R:
			std::cout << "READ";
			break;
		case W:
			std::cout << "WRITE";
			break;
		case RW:
			std::cout << "READ/WRITE";
			break;
		default:
			std::cout << "UNDEFINED";
			break;
		}
		std::cout << std::endl;

	}
	std::cout << "----------------------------------"
			  << std::endl;
}
