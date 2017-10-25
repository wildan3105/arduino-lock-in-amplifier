const byte adcPin = 0;
volatile int adcReading;
volatile boolean adcDone;
boolean adcStarted;

void setup ()
{
  Serial.begin (115200);
  // set the analog reference (high two bits of ADMUX) and select the
  // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
  // to 0 (the default).
  ADMUX = bit (REFS0) | (adcPin & 0x07);

}  // end of setup

// ADC complete ISR
ISR (ADC_vect)
  {
  byte low, high;
  
  // we have to read ADCL first; doing so locks both ADCL
  // and ADCH until ADCH is read.  reading ADCL second would
  // cause the results of each conversion to be discarded,
  // as ADCL and ADCH would be locked when it completed.
  low = ADCL;
  high = ADCH;

  adcReading = (high << 8) | low;
  adcDone = true;  
  }  // end of ADC_vect
  
void loop ()
{
  // if last reading finished, process it
  if (adcDone)
    {
    adcStarted = false;

    // do something with the reading, for example, print it
    Serial.println (adcReading);
    delay (500);

    adcDone = false;
    }
    
  // if we aren't taking a reading, start a new one
  if (!adcStarted)
    {
    adcStarted = true;
    // start the conversion
    ADCSRA |= bit (ADSC) | bit (ADIE);
    }    
  
  // do other stuff here

}  // end of loop