#include "Mdatevent.h"

#include <iostream>     // std::cout


namespace mevent {

Mdatevent::Mdatevent() {
	// TODO Auto-generated constructor stub

}

Mdatevent::~Mdatevent() {
	// TODO Auto-generated destructor stub
}


Mdatevent MakeNeutron(int modID, int slotID, int amplitude, int position, int timestamp){
	Mdatevent NewEvent { };

	NewEvent.ID = IDClass::neutron;
	return NewEvent;
}

void Mdatevent::PrintEvent(){

	int myvalue { 0 };
    switch (Mdatevent::ID) {
        case IDClass::neutron : myvalue=0; break;
		case IDClass::trigger : myvalue=1; break;
    }

	std::cout<< myvalue;
}



} /* namespace mevent */
