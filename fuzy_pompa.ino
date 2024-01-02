
// Library untuk fuzzy logic
#include <Fuzzy.h>

// Inisialisasi input dan output
Fuzzy *fuzzy = new Fuzzy();
FuzzyInput *suhu = new FuzzyInput(1);
FuzzyInput *ph = new FuzzyInput(2);
FuzzyInput *kelembaban = new FuzzyInput(3);
FuzzyOutput *pompa = new FuzzyOutput(4);

// Inisialisasi pin sensor dan pompa
const int pinSuhu = A0;
const int pinPh = A1;
const int pinKelembaban = A2;
const int pinPompa = 13;

// Inisialisasi variabel untuk menyimpan nilai sensor
float nilaiSuhu, nilaiPh, nilaiKelembaban;

// Fungsi untuk membaca nilai sensor
void bacaSensor() {
  nilaiSuhu = analogRead(pinSuhu) * 0.48828125; // Konversi ke derajat Celcius
  nilaiPh = analogRead(pinPh) * 0.0140625; // Konversi ke pH
  nilaiKelembaban = analogRead(pinKelembaban) * 0.09765625; // Konversi ke persen
}

// Fungsi untuk mengatur fuzzy set dan aturan
void aturFuzzy() {
  // Fuzzy set untuk suhu
  suhu->addFuzzySet(new FuzzySet(0, 0, 15, 25)); // Dingin
  suhu->addFuzzySet(new FuzzySet(20, 30, 30, 40)); // Normal
  suhu->addFuzzySet(new FuzzySet(35, 45, 60, 60)); // Panas
  fuzzy->addFuzzyInput(suhu);

  // Fuzzy set untuk pH
  ph->addFuzzySet(new FuzzySet(0, 0, 5, 6)); // Asam
  ph->addFuzzySet(new FuzzySet(5.5, 6.5, 6.5, 7.5)); // Netral
  ph->addFuzzySet(new FuzzySet(7, 8, 14, 14)); // Basa
  fuzzy->addFuzzyInput(ph);

  // Fuzzy set untuk kelembaban
  kelembaban->addFuzzySet(new FuzzySet(0, 0, 30, 40)); // Kering
  kelembaban->addFuzzySet(new FuzzySet(35, 45, 45, 55)); // Lembab
  kelembaban->addFuzzySet(new FuzzySet(50, 60, 100, 100)); // Basah
  fuzzy->addFuzzyInput(kelembaban);

  // Fuzzy set untuk pompa
  pompa->addFuzzySet(new FuzzySet(0, 0, 0, 0)); // Off
  pompa->addFuzzySet(new FuzzySet(0, 0, 0, 10)); // Pendek
  pompa->addFuzzySet(new FuzzySet(5, 15, 15, 25)); // Sedang
  pompa->addFuzzySet(new FuzzySet(20, 30, 30, 40)); // Panjang
  pompa->addFuzzySet(new FuzzySet(35, 40, 40, 40)); // On
  fuzzy->addFuzzyOutput(pompa);

  // Fuzzy rule
  // Jika suhu dingin DAN pH asam DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(1, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(0)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu dingin DAN pH asam DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(2, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(0)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu dingin DAN pH asam DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(3, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(0)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
  // Jika suhu dingin DAN pH netral DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(4, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(1)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu dingin DAN pH netral DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(5, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(1)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu dingin DAN pH netral DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(6, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(1)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
  // Jika suhu dingin DAN pH basa DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(7, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(2)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu dingin DAN pH basa DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(8, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(2)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu dingin DAN pH basa DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(9, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(0), ph->getFuzzySet(2)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
  // Jika suhu normal DAN pH asam DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(10, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(0)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu normal DAN pH asam DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(11, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(0)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu normal DAN pH asam DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(12, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(0)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
  // Jika suhu normal DAN pH netral DAN kelembaban kering, maka pompa pendek
  fuzzy->addFuzzyRule(new FuzzyRule(13, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(1)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(1)));
  // Jika suhu normal DAN pH netral DAN kelembaban lembab, maka pompa off
  fuzzy->addFuzzyRule(new FuzzyRule(14, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(0)));
  // Jika suhu normal DAN pH netral DAN kelembaban basah, maka pompa off
  fuzzy->addFuzzyRule(new FuzzyRule(15, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(1)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(0)));
  // Jika suhu normal DAN pH basa DAN kelembaban kering, maka pompa pendek
  fuzzy->addFuzzyRule(new FuzzyRule(16, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(2)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(1)));
  // Jika suhu normal DAN pH basa DAN kelembaban lembab, maka pompa off
  fuzzy->addFuzzyRule(new FuzzyRule(17, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(2)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(0)));
  // Jika suhu normal DAN pH basa DAN kelembaban basah, maka pompa off
  fuzzy->addFuzzyRule(new FuzzyRule(18, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(1), ph->getFuzzySet(2)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(0)));
  // Jika suhu panas DAN pH asam DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(19, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(0)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu panas DAN pH asam DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(20, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(0)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu panas DAN pH asam DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(21, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(0)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
  // Jika suhu panas DAN pH netral DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(22, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(1)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu panas DAN pH netral DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(23, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(1)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu panas DAN pH netral DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(24, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(1)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
  // Jika suhu panas DAN pH basa DAN kelembaban kering, maka pompa on
  fuzzy->addFuzzyRule(new FuzzyRule(25, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(2)), kelembaban->getFuzzySet(0)), pompa->getFuzzySet(4)));
  // Jika suhu panas DAN pH basa DAN kelembaban lembab, maka pompa panjang
  fuzzy->addFuzzyRule(new FuzzyRule(26, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(2)), kelembaban->getFuzzySet(1)), pompa->getFuzzySet(3)));
  // Jika suhu panas DAN pH basa DAN kelembaban basah, maka pompa sedang
  fuzzy->addFuzzyRule(new FuzzyRule(27, new FuzzyAND(new FuzzyAND(suhu->getFuzzySet(2), ph->getFuzzySet(2)), kelembaban->getFuzzySet(2)), pompa->getFuzzySet(2)));
}

// Fungsi untuk menghitung output fuzzy
float hitungPompa() {
  // Masukkan nilai sensor ke input fuzzy
  fuzzy->setInput(1, nilaiSuhu);
  fuzzy->setInput(2, nilaiPh);
  fuzzy->setInput(3, nilaiKelembaban);

  // Lakukan inferensi fuzzy
  fuzzy->fuzzify();

  // Kembalikan nilai output fuzzy
  return fuzzy->defuzzify(4);
}

// Fungsi setup
void setup() {
  // Inisialisasi serial monitor
  Serial.begin(9600);

  // Inisialisasi pin pompa sebagai output
  pinMode(pinPompa, OUTPUT);

  // Atur fuzzy set dan aturan
  aturFuzzy();
}

// Fungsi loop
void loop() {
  // Baca nilai sensor
  bacaSensor();

  // Tampilkan nilai sensor ke serial monitor
  Serial.print("Suhu: ");
  Serial.print(nilaiSuhu);
  Serial.println(" C");
  Serial.print("pH: ");
  Serial.println(nilaiPh);
  Serial.print("Kelembaban: ");
  Serial.print(nilaiKelembaban);
  Serial.println(" %");

  // Hitung output fuzzy
  float nilaiPompa = hitungPompa();

  // Tampilkan output fuzzy ke serial monitor
  Serial.print("Pompa: ");
  Serial.print(nilaiPompa);
  Serial.println(" detik");

  // Nyalakan pompa sesuai output fuzzy
  digitalWrite(pinPompa, HIGH);
  delay(nilaiPompa * 1000); // Konversi detik ke milidetik

  // Matikan pompa
  digitalWrite(pinPompa, LOW);
  delay(1000);

  // Ulangi loop
}
