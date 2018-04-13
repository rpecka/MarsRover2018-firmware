#include "mbed.h"
#include "DS1820.h"

AnalogIn ecSensor(A1);
DS1820 tempSensor(D7);

float getEC();

// main() runs in its own thread in the OS
int main() {
	float electricalConductivity;
	while (true) {
		electricalConductivity = getEC();
		printf("%.2f ms/cm", electricalConductivity);
		wait(10);
	}
}
// Returns the electrical conductivity in ms/cm
float getEC() {
	float voltage = ecSensor.read();
	float temperature = tempSensor.temperature();
	float temperatureCoefficient = 1.0 + 0.0185 * (temperature - 25.0);
	float coefficientVoltage = voltage/temperatureCoefficient;
	if (coefficientVoltage < 150 || coefficientVoltage > 3300) {
		printf("The voltage is out of the sensor's accuracy range");
	} else {
		float electricalConductivity;
		if (coefficientVoltage <= 448) {
			electricalConductivity = 6.84 * coefficientVoltage - 64.32; //1ms/cm<EC<=3ms/cm
		} else if (coefficientVoltage <= 1457) {
			electricalConductivity = 6.98 * coefficientVoltage - 127; //3ms/cm<EC<=10ms/cm
		} else {
			electricalConductivity = 5.3 * coefficientVoltage + 2278; //10ms/cm<EC<20ms/cm
		}
		electricalConductivity /= 1000; //convert us/cm to ms/cm
		return electricalConductivity;
	}
	
}

