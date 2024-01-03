
#include <DHT.h>
#include <Fuzzy.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define PH_PIN A0
#define SOIL_PIN A1
#define RELAY_PIN 13

DHT dht(DHTPIN, DHTTYPE);
Fuzzy* fuzzy = new Fuzzy();

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  
  FuzzyInput* temp = new FuzzyInput(1);
  FuzzySet* lowTemp = new FuzzySet(0, 10, 10, 20); 
  FuzzySet* highTemp = new FuzzySet(30, 40, 40, 50);
  temp->addFuzzySet(lowTemp);
  temp->addFuzzySet(highTemp);

  FuzzyInput* humidity = new FuzzyInput(2);
  FuzzySet* lowHumidity = new FuzzySet(0, 10, 10, 20);
  FuzzySet* highHumidity = new FuzzySet(80, 90, 90, 100);
  humidity->addFuzzySet(lowHumidity);
  humidity->addFuzzySet(highHumidity);

  FuzzyInput* ph = new FuzzyInput(3);
  FuzzySet* lowPh = new FuzzySet(0, 3, 3, 6);
  FuzzySet* highPh = new FuzzySet(7, 10, 10, 14);
  ph->addFuzzySet(lowPh);
  ph->addFuzzySet(highPh);

  FuzzyInput* soil = new FuzzyInput(4);
  FuzzySet* drySoil = new FuzzySet(0, 300, 300, 600);
  FuzzySet* wetSoil = new FuzzySet(700, 1000, 1000, 1023);
  soil->addFuzzySet(drySoil);
  soil->addFuzzySet(wetSoil);

  FuzzyOutput* pump = new FuzzyOutput(5);
  FuzzySet* pumpOff = new FuzzySet(0, 0.1, 0.1, 0.2);
  FuzzySet* pumpOn = new FuzzySet(0.8, 0.9, 0.9, 1);
  pump->addFuzzySet(pumpOff);
  pump->addFuzzySet(pumpOn);

  fuzzy->addFuzzyInput(temp);
  fuzzy->addFuzzyInput(humidity);
  fuzzy->addFuzzyInput(ph);
  fuzzy->addFuzzyInput(soil);
  fuzzy->addFuzzyOutput(pump);

  FuzzyRuleAntecedent* ifLowTempAndLowHumidity = new FuzzyRuleAntecedent();
  ifLowTempAndLowHumidity->joinWithAND(lowTemp, lowHumidity);
  FuzzyRuleConsequent* thenPumpOff = new FuzzyRuleConsequent();
  thenPumpOff->addOutput(pumpOff);
  FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifLowTempAndLowHumidity, thenPumpOff);
  fuzzy->addFuzzyRule(fuzzyRule1);

  // Add more rules...
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int ph_val = analogRead(PH_PIN);
  int soil_val = analogRead(SOIL_PIN);
  
  fuzzy->setInput(1, t);
  fuzzy->setInput(2, h);
  fuzzy->setInput(3, ph_val);
  fuzzy->setInput(4, soil_val);
  
  fuzzy->fuzzify();
  
  float output = fuzzy->defuzzify(1);
  
  if (output > 0.5) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
}
