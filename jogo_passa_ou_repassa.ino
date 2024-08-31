/*
Jogo de passa ou repassa
por András Fleury
Para S&A Segurança e Arte
Versão 1.04.25.08.24
Todos os direitos reservados para
Studio Stern de comunicação LTDA
*/
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);

// Definindo os pinos
const int btnJogador1 = 2;
const int btnJogador2 = 3;
const int ledJogador1 = 4;
const int ledJogador2 = 5;
const int btnAcerto = 6;
const int btnErro = 7;
const int btnFinalizar = 8;
const int buzzer = 9;
const int ledAcerto = 10;
const int ledErro = 11;

// Variáveis de controle
int pontosJogador1 = 0;
int pontosJogador2 = 0;
bool jogador1Ativo = false;
bool jogador2Ativo = false;
bool rodadaEmAndamento = true;

void setup() {
  // Inicializando o LCD
  lcd.begin(16, 2);
  
  // Configurando os pinos dos botões como entrada
  pinMode(btnJogador1, INPUT);
  pinMode(btnJogador2, INPUT);
  pinMode(btnAcerto, INPUT);
  pinMode(btnErro, INPUT);
  pinMode(btnFinalizar, INPUT);
  
  // Configurando os pinos dos LEDs como saída
  pinMode(ledJogador1, OUTPUT);
  pinMode(ledJogador2, OUTPUT);
  pinMode(ledAcerto, OUTPUT);
  pinMode(ledErro, OUTPUT);
  
  // Configurando o pino do buzzer como saída
  pinMode(buzzer, OUTPUT);
  
  // Inicializando LEDs como apagados
  digitalWrite(ledJogador1, LOW);
  digitalWrite(ledJogador2, LOW);
  digitalWrite(ledAcerto, LOW);
  digitalWrite(ledErro, LOW);
  
  // Inicializando comunicação serial para depuração
  Serial.begin(9600);

  // Exibindo pontuação inicial no LCD
  exibirPontuacao();
}

void loop() {
  if (rodadaEmAndamento) {
    // Verificando se algum jogador apertou seu botão
    if (digitalRead(btnJogador1) == HIGH && !jogador2Ativo) {
      jogador1Ativo = true;
      jogador2Ativo = false;
      digitalWrite(ledJogador1, HIGH);
      digitalWrite(ledJogador2, LOW);
    } else if (digitalRead(btnJogador2) == HIGH && !jogador1Ativo) {
      jogador2Ativo = true;
      jogador1Ativo = false;
      digitalWrite(ledJogador2, HIGH);
      digitalWrite(ledJogador1, LOW);
    }

    // Verificando se o mestre apertou o botão de acerto
    if (digitalRead(btnAcerto) == HIGH) {
      if (jogador1Ativo) {
        pontosJogador1++;
        tocarSomSucesso();
      } else if (jogador2Ativo) {
        pontosJogador2++;
        tocarSomSucesso();
      }
      exibirPontuacao();
      resetRodada();
    }

    // Verificando se o mestre apertou o botão de erro
    if (digitalRead(btnErro) == HIGH) {
      tocarSomErro();
      if (jogador1Ativo) {
        jogador1Ativo = false;
        jogador2Ativo = true;
        digitalWrite(ledJogador1, LOW);
        digitalWrite(ledJogador2, HIGH);
      } else if (jogador2Ativo) {
        jogador2Ativo = false;
        jogador1Ativo = true;
        digitalWrite(ledJogador2, LOW);
        digitalWrite(ledJogador1, HIGH);
      }
      exibirPontuacao();
    }

    // Verificando se o mestre apertou o botão de finalizar
    if (digitalRead(btnFinalizar) == HIGH) {
      finalizarJogo();
    }
  }
}

void resetRodada() {
  // Reseta os LEDs e variáveis de controle para uma nova rodada
  digitalWrite(ledJogador1, LOW);
  digitalWrite(ledJogador2, LOW);
  jogador1Ativo = false;
  jogador2Ativo = false;
  rodadaEmAndamento = true;
}

void finalizarJogo() {
  // Finaliza o jogo e exibe o vencedor
  rodadaEmAndamento = false;
  Serial.print("Jogador 1 pontos: ");
  Serial.println(pontosJogador1);
  Serial.print("Jogador 2 pontos: ");
  Serial.println(pontosJogador2);

  // Determina o vencedor e faz a pontuação piscar no LCD
  if (pontosJogador1 > pontosJogador2) {
    Serial.println("Jogador 1 é o vencedor!");
    piscarPontuacao(pontosJogador1, 1);
    piscarLed(ledJogador1);
  } else if (pontosJogador2 > pontosJogador1) {
    Serial.println("Jogador 2 é o vencedor!");
    piscarPontuacao(pontosJogador2, 2);
    piscarLed(ledJogador2);
  } else {
    Serial.println("Empate!");
    exibirEmpate();
  }
  
  // Reinicia o jogo após exibir o vencedor ou empate
  reiniciarJogo();
}

void reiniciarJogo() {
  pontosJogador1 = 0;
  pontosJogador2 = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pronto para o Jogo");
  delay(2000); // Espera 2 segundos antes de reiniciar
  exibirPontuacao();
  rodadaEmAndamento = true;
}

void piscarLed(int led) {
  for (int i = 0; i < 5; i++) { // Pisca 5 vezes
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

// Função para tocar som de sucesso e acender LED de acerto
void tocarSomSucesso() {
  digitalWrite(ledAcerto, HIGH); // Acende LED de acerto
  tone(buzzer, 1000, 500);       // Frequência de 1000 Hz por 500 ms
  delay(500);                    // Aguardar para evitar sobreposição de sons
  digitalWrite(ledAcerto, LOW);  // Apaga LED de acerto após 500 ms
}

// Função para tocar som de erro e acender LED de erro
void tocarSomErro() {
  digitalWrite(ledErro, HIGH);   // Acende LED de erro
  tone(buzzer, 500, 500);        // Frequência de 500 Hz por 500 ms
  delay(500);                    // Aguardar para evitar sobreposição de sons
  digitalWrite(ledErro, LOW);    // Apaga LED de erro após 500 ms
}

// Função para exibir a pontuação atual no LCD
void exibirPontuacao() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Jog1: ");
  lcd.print(pontosJogador1);
  lcd.setCursor(0, 1);
  lcd.print("Jog2: ");
  lcd.print(pontosJogador2);
}

// Função para piscar a pontuação do vencedor no LCD
void piscarPontuacao(int pontos, int jogador) {
  for (int i = 0; i < 5; i++) { // Pisca 5 vezes
    lcd.clear();
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("Vencedor Jog ");
    lcd.print(jogador);
    lcd.setCursor(0, 1);
    lcd.print("Pontos: ");
    lcd.print(pontos);
    delay(500);
  }
}

// Função para exibir empate no LCD
void exibirEmpate() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Empate!");
  delay(2000); // Exibe "Empate!" por 2 segundos
}
