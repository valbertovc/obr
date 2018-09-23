#include <Ultrasonic.h>

#define ML1 2          // motor esquerda
#define MLV 3          // velocidade esquerda 
#define ML2 4          // motor esquerda
#define MR1 5          // motor direita
#define MRV 6          // velocidade direita 
#define MR2 7          // motor direita

#define IREE A0        // sensor esquerda
#define IRME A1        // sensor meio esquerda
#define IRCC A2        // sensor central
#define IRMD A3        // sensor meio direita
#define IRDD A4        // sensor direita

int ee = 0;            // valor do sensor esquerda
int me = 0;            // valor do sensor meio esquerda
int cc = 0;            // valor do sensor central
int md = 0;            // valor do sensor meio direita
int dd = 0;            // valor do sensor direita

int mov = 0;           // movimento atual
int cont = 0;          // conta a repetição dos movimentos
#define PARE 0         // pare
#define SIGA 1         // siga em frente
#define ESQU 2         // esquerda
#define DIRE 3         // direita
#define VOLT 4         // volte em marcha ré
#define NENH 5         // nenhum movimento - decidir no próximo loop
#define VELOCIDADE 115 // velocidade para seguir em frente
#define ESQM 6         // esquerda meio
#define DIRM 7         // direita meio

Ultrasonic ultrasonic(12, 13);
int distancia;

void setup() {

  Serial.begin(9600);

  pinMode(IREE, INPUT);
  pinMode(IRME, INPUT);
  pinMode(IRCC, INPUT);
  pinMode(IRMD, INPUT);
  pinMode(IRDD, INPUT);

  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MLV, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(MRV, OUTPUT);
}

void loop() {
  delay(20);
  distancia = ultrasonic.read();

  ler_faixa();

  Serial.print("Distance: ");
  Serial.print(distancia);
  Serial.print("cm ");
  Serial.print(" | Sensors: ");
  Serial.print(ee);
  Serial.print("; ");
  Serial.print(me);
  Serial.print("; ");
  Serial.print(cc);
  Serial.print("; ");
  Serial.print(md);
  Serial.print("; ");
  Serial.print(dd);
  Serial.print(" | Mov: ");

  

 if (distancia > 0 && distancia < 5)
  {
    Serial.println("obstaculo detectado");
    pare();
    delay(100);
    direita();
    delay(650);
    siga();
    delay(1500);
    esquerda();
    delay(650);
    siga();    
    delay(1500);
    esquerda();
    delay(650);
    siga();
    delay(1500);
    direita();
    delay(650);
  }
  
  
  
  // faixa sobre um único sensor
  
       if (ee == LOW  && me == HIGH && cc == HIGH && md == HIGH && dd == HIGH) { mov = ESQU; }
  else if (ee == HIGH && me == LOW  && cc == HIGH && md == HIGH && dd == HIGH) { mov = ESQM; }
  else if (ee == HIGH && me == HIGH && cc == LOW  && md == HIGH && dd == HIGH) { mov = SIGA; }
  else if (ee == HIGH && me == HIGH && cc == HIGH && md == LOW  && dd == HIGH) { mov = DIRM; }
  else if (ee == HIGH && me == HIGH && cc == HIGH && md == HIGH && dd == LOW ) { mov = DIRE; }

  // faixa final
  
  else if (esta_preto()) { mov = PARE; }

  // faixa dos extremos ao centro
  
  else if (ee == LOW  && me == LOW  && cc == LOW  && md == HIGH && dd == HIGH) { mov = ESQU; } 
  else if (ee == HIGH && me == HIGH && cc == LOW  && md == LOW  && dd == LOW ) { mov = DIRE; }

  // faixa nos extremos
  
  else if (ee == LOW  && me == LOW  && cc == HIGH && md == HIGH && dd == HIGH) { mov = ESQU; }
  else if (ee == HIGH && me == HIGH && cc == HIGH && md == LOW  && dd == LOW ) { mov = DIRE; }

  // faixa nos meios
  
  else if (ee == HIGH && me == LOW  && cc == LOW  && md == HIGH && dd == HIGH) // mov = ESQU
  { 
    if (mov == ESQU || mov == ESQM || mov== SIGA || mov == PARE) 
    { 
      mov = ESQM; 
    } 
  }
  else if (ee == HIGH && me == HIGH && cc == LOW  && md == LOW  && dd == HIGH) // mov = DIRE
  { 
    if(mov == DIRE || mov == DIRM || mov == SIGA || mov == PARE)
    {
      mov = DIRM;       
    }
  }
  else {
    // os parametros não casaram com nenhuma situação. deve-se repetir o movimento anterior.    
  }

  Serial.print(mov);
  Serial.print(", ");

  executar_movimento(mov);
  
  Serial.println("");
}

void ler_faixa(){
  ee = !digitalRead(IREE);
  me = !digitalRead(IRME);
  cc = !digitalRead(IRCC);
  md = !digitalRead(IRMD);
  dd = !digitalRead(IRDD);
}

bool esta_preto() {
  return ee == LOW  && me == LOW  && cc == LOW  && md == LOW  && dd == LOW;
}

bool esta_branco() {
  return ee == HIGH && me == HIGH && cc == HIGH && md == HIGH && dd == HIGH;
}

void executar_movimento(int mov){
       if (mov == ESQU) { esquerda();      }
  else if (mov == ESQM) { esquerda_meio(); }
  else if (mov == DIRE) { direita();       }
  else if (mov == DIRM) { direita_meio();  }
  else if (mov == SIGA) { siga();          }
  else if (mov == PARE) { pare();          }  
}

void pare() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, LOW);
  analogWrite(MLV, 0);
  analogWrite(MRV, 0);
  Serial.print("pare, ");
}

void siga() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(MLV, VELOCIDADE);
  analogWrite(MRV, VELOCIDADE);
  Serial.print("siga, ");
}

void esquerda_meio() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(MLV, VELOCIDADE * 1.1);      // volta
  analogWrite(MRV, VELOCIDADE * 1.8);      // segue
  Serial.print("esquerda meio, ");
}

void esquerda() {
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, HIGH);
  digitalWrite(MR2, LOW);
  analogWrite(MLV, VELOCIDADE * 1.4);      // volta
  analogWrite(MRV, VELOCIDADE * 1.8);      // segue
  Serial.print("esquerda, ");
}

void direita_meio() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(MLV, VELOCIDADE * 1.8);      // segue
  analogWrite(MRV, VELOCIDADE * 1.1);      // volta
  Serial.print("direita meio, ");
}

void direita() {
  digitalWrite(ML1, HIGH);
  digitalWrite(ML2, LOW);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(MLV, VELOCIDADE * 1.8);      // segue
  analogWrite(MRV, VELOCIDADE * 1.4);      // volta
  Serial.print("direita, ");
}

void volte() {
  analogWrite(MLV, VELOCIDADE * 0);
  analogWrite(MRV, VELOCIDADE * 0);
  delay(100);
  digitalWrite(ML1, LOW);
  digitalWrite(ML2, HIGH);
  digitalWrite(MR1, LOW);
  digitalWrite(MR2, HIGH);
  analogWrite(MLV, VELOCIDADE);
  analogWrite(MRV, VELOCIDADE);
  Serial.print("volte, ");
}
