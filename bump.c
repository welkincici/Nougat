int highRecord, lowRecord;
void bump();
void bump()
{
	if (digitalRead(49) == HIGH)
		highRecord = millis();
	else
		lowRecord = millis();

	if (millis() - highRecord > 1000 || millis() - lowRecord > 1000)
		detachInterrupt(0);
	else
		attachInterrupt(0, speedMonitor, RISING);
}