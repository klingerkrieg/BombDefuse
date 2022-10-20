#include <LiquidCrystal.h>
 
// Define as conexões e cria o objeto para acesso
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int backLight = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int buzzerPIN = 53;
 
// Limites para detecção das teclas, em ordem crescente
struct {
  int limite;
  char *nome;
} teclas[] = {
  {   50, "Direita " },
  {  150, "Cima    " },
  {  310, "Baixo   " },
  {  500, "Esquerda" },
  {  750, "Select  " },
  { 1024, "        " }  // nenhuma tecla apertada
};
 
// Iniciação
void setup() {

  Serial.begin(9600);

  
  //Progama o pino de backlight como saída
  pinMode(backLight, OUTPUT);
  pinMode(buzzerPIN,OUTPUT);
  
  // Inicia o display e coloca uma mensagem
  lcd.begin(16, 2);
  lcd.print("--:--");
  // Acende o backlight
  digitalWrite(backLight, HIGH);
}


bool ativada = false;
int min_ = 10;
int seg_ = 0;


 
// Laço principal
void loop() {

  if (ativada){
    lcd.setCursor(0,0);

    if (min_ < 10){
      lcd.print("0");
    }
    lcd.print(min_);
    lcd.print(":");
    if (seg_ < 10){
      lcd.print("0");
    }
    lcd.print(seg_);
  
    tone(buzzerPIN, 100); // Send 1KHz sound signal...
    delay(50);        // ...for 1 sec
    noTone(buzzerPIN);     // Stop sound...
    delay(950);        // ...for 1sec
  
    seg_--;
  
    if( seg_ == -1){
      seg_=59;
      min_--;
      if( min_ == -1){
        lcd.setCursor(0,1);
        lcd.print("xxx");
        ativada = false;
      }
    }
  }


  
  static int teclaAnt = -1;   // última tecla detectada
 
  // Lê a tensão no pino A0
  int leitura = analogRead(A0);

  Serial.println(leitura);
 
  // Identifica a tecla apertada pela tensão lida
  int teclaNova;
  for (teclaNova = 0; ; teclaNova++) {
    if (leitura < teclas[teclaNova].limite) {
      break;
    }
  }

  //starta a bomba quando apertar o select
  if (teclaNova == 4){
    ativada = true;  
  }
 
  // Atualiza a tela se pressionou uma nova tecla
  if (teclaNova != teclaAnt) {
    lcd.setCursor(0,1);
    lcd.print(teclas[teclaNova].nome);
    teclaAnt = teclaNova;
  }
 
}
