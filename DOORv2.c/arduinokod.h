

String allCard[10];
int index = 0;



void setup() {
	Serial.begin(9600);
	pinMode(3, OUTPUT); //röd
	pinMode(4, OUTPUT); //grön
}

void Test(String testCard) {
	Serial.println(testCard);
	for (int i = 0; i < index; i++) {
		if (testCard == allCard[i]) {
			digitalWrite(4, HIGH);
			delay(3000);
			digitalWrite(4, LOW);
			return;
		}
	}
	digitalWrite(3, HIGH);
	delay(3000);
	digitalWrite(3, LOW);
	return;
}
void ClearList() {
	for (int i = 0; i < 10; i++) {
		allCard[i] = "";
	}
	index = 0;
	return;
}
void loop() {
	while (Serial.available()) {
		String inData = Serial.readStringUntil('>');
		if (inData == "1") {
			digitalWrite(4, HIGH);
			delay(3000);
			digitalWrite(4, LOW);
		}

		else if (inData.startsWith("<ADDCARD")) {
			allCard[index] = inData.substring(8);
			index++;
			if (index == 9) {
				index = 0;
			}

		}
		else if (inData.startsWith("<TEST")) {

			Test(inData.substring(5));

		}

		else if (inData.startsWith("<LIST")) {
			Serial.println(index);
			for (int i = 0; i < 10; i++) {
				Serial.println(allCard[i]);

			}
		}
		else if (inData.startsWith("<CLEAR")) {
			ClearList();
		}
	}
	return;
}

	