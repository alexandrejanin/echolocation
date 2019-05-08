// Pin de l'arduino branche au sonar (trig)
#define trigPin 11
// Pin de l'arduino branche au sonar (echo)
#define echoPin 12
// Pin de l'arduino branche au transistor qui controle le vibreur
#define vibrPin 6

// Distance a laquelle les vibrations sont maximales, en cm
#define distMin 10
// Distance a laquelle les vibrations sont minimales, en cm
#define distMax 200
// Delai de vibration minimal, en ms (0 = continu)
#define delayMin 0
// Delai de vibration maximal, en ms
#define delayMax 2000

// Temps ecoule depuis la derniere vibration (en ms)
int currentDelay = 0;
// Temps de la derniere execution de loop()
int lastTime = 0;

// Initialisation
void setup() {
  // Initialise la connexion avec l'ordinateur pour l'affichage des valeurs (9600 bits par seconde)
  Serial.begin(9600);
  // vibrPin = pin de sortie
  pinMode(vibrPin, OUTPUT);
  // trigPin = pin de sortie
  pinMode(trigPin, OUTPUT);
  // echoPin = pin d'entree
  pinMode(echoPin, INPUT);
}

// Distance captee par le capteur en cm
float sonarCm() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Delai de retour d'ultrasons du capteur
  float pulse = pulseIn(echoPin, HIGH);
  // Distance en cm
  float cm = pulse / 2 / 29.1; // duree -> cm (343 m/s)

  return cm;
}

// Boucle principale du programme
void loop() {
  // Recupere la distance mesuree par le capteur
  float cm = sonarCm();
  // Affiche la distance si l'Arduino est branche a un ordinateur
  Serial.print("distance: ");
  Serial.print(cm);
  Serial.print("\n");

  // On limite la distance aux valeurs donnees au debut du fichier
  int constrainedCm = constrain(cm, distMin, distMax);

  // Delai entre les bips selon la distance:
  // Si la distance est minimale, le delai est minimal
  // Si la distance est maximale, le delai est maximal
  int del = map(constrainedCm, distMin, distMax, delayMin, delayMax);

  // On augmente currentDelay du nombre de millisecondes ecoulees
  // depuis la derniere loop()
  // afin de mesurer le temps ecoule depuis la derniere vibration
  int currentTime = millis();
  currentDelay += currentTime - lastTime;
  lastTime = currentTime;

  // Desactive les vibrations
  analogWrite(vibrPin, 0);
  // Si le delai depuis la derniere vibration est superieur au delai
  // calcule d'apres la distance...
  if (currentDelay >= del) {
    // ...on emet une vibration
    analogWrite(vibrPin, 153);
    delay(100);
    // Et on remet le delai actuel a 0
    currentDelay = 0;
  }
}
