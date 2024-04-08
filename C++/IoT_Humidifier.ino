//delay << 공통 1000 = 1초 딜레이

//온습도 라이브러리
#include <DHT.h>
//시리얼 라이브러리
#include <SoftwareSerial.h>

//ESP8266 모듈 관련
//BT_RXD, BT_TXD rx,tx 12,11로 정의
#define BT_RXD 12
#define BT_TXD 11

//온습도측정 모듈 관련
//DHTPIN은 6번째 디지털핀이라는 것을 정의
#define DHTPIN 6
//DHT22 버전을 쓰기에 DHTTYPE은 DHT22
#define DHTTYPE DHT22
//DHT 라이브러리 사용 규칙
DHT dht(DHTPIN, DHTTYPE);

//소프트웨어시리얼 
SoftwareSerial mySerial(2, 3);
SoftwareSerial ESP_wifi(BT_RXD, BT_TXD);

//가습모듈, RGB, 음성인식 디지털 핀 변수
int water = 5;
int blue = 8;
int green = 9;
int red = 10;
byte voice_recogn = 0; 

//음성인식 데이터 처리 후 반복문할때 필요한 true/false 담는 변수
boolean Enabled = false;

void setup() {

  //보드레이트 9600으로
  Serial.begin(9600);
  mySerial.begin(9600);
  ESP_wifi.begin(9600);

  //함수 호출
  setTotal();
  setWifi();
  
}

//가습모듈, RGB, 음성인식 설정 함수

void setTotal() {
  
  mySerial.listen();

  delay(1000);

  //가습모듈 핀모드 지정
  pinMode(water, OUTPUT);
  Serial.println("load Water Atomizer");
  //RGB모듈 핀모드 지정
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.println("load RGB");
  delay(1000);
  
  //음성인식 모듈, 음성을 읽기 위해 컴팩트 모드로 변경
  mySerial.write(0xAA);
  mySerial.write(0x37);
  delay(1000);
  //음성 명령어 임포트
  mySerial.write(0xAA);
  mySerial.write(0x21);
  Serial.println("load Voice");
}

void setWifi() {
  
  ESP_wifi.listen();

  while (ESP_wifi.available()) {
    ESP_wifi.read();
  }

  delay(2000);
  //ESP8266 모듈 초기화
  ESP_wifi.println("AT+RST\r\n");
  delay(1000);
  //ESP8266 모듈 디바이스, AP 기능 모두 사용 명령어
  ESP_wifi.println("AT+CWMODE=3\r\n");
  delay(1000);
  //와이파이 연결 명령어 (규칙 : "아이디","비밀번호")
  ESP_wifi.println("AT+CWJAP=\"iptime102\",\“PASSWORD\"\"\r\n");
  delay(3000);

  Serial.println("load WiFi");
}

//문자를 만날때까지 문자열을 읽음

void printResponse() {
  while (ESP_wifi.available()) {
    Serial.println(ESP_wifi.readStringUntil('\n'));
  }
}

//API 구조로 웹 연결을 통해 데이터베이스에 집어넣는 함수

void insertDB (float arduino_temp, float arduino_hum) {

  //소수점 형식으로 담아온 데이터 스트링으로 변환
  String te = String(arduino_temp);
  String hu = String(arduino_hum);
  //웹 연결 주소 구조에 맞게 변경
  String t_h_sum = "temp=" + te + "&humi=" + hu;

  ESP_wifi.listen();

  while (ESP_wifi.available()) {
    ESP_wifi.read();
  }

  //멀티 접속 허용
  ESP_wifi.println("AT+CIPMUX=1");
  delay(1000);
  printResponse();
  //TCP 형식으로 해당 웹서버 아이피, 포트에 연결 시도
  ESP_wifi.println("AT+CIPSTART=4,\"TCP\",\"61.98.10.34\",80");
  //웹 요청을 데이터베이스 INSERT 할 웹 주소 구조에 맞게 변경 후 cmd에 스트링 형식으로 담음
  String cmd = "GET http://61.98.10.34/process.php?" + t_h_sum + " HTTP/1.0";
  //4번 채널에 cmd 문자열 길이 + 4만큼 보낸다는 명령어
  ESP_wifi.println("AT+CIPSEND=4," + String(cmd.length() + 4));
  delay(1000);
  //웹 요청
  ESP_wifi.println(cmd);
  delay(200);
  ESP_wifi.println();
  //printResponse 함수 호출
  printResponse();
}

void loop() {

  //dht 함수로 리턴받은 값을 h와 t에 담음
  
  mySerial.listen();

  if (Enabled == true) {
    /*
    Serial.print("humidity : ");
    Serial.println(h);
    Serial.print("temperature : ");
    Serial.println(t);
    */
    } else if (Enabled == false) {

  }

  while(mySerial.available()) {
    Serial.println("voice input");
    
    mySerial.read();

    //음성 인식
    voice_recogn = mySerial.read();
    //녹음한 그룹1 데이터의 1~5번째 케이스와 동일한 데이터값이라면
    switch(voice_recogn) {
      case 0x11: //켜줘
        break;
      case 0x12: //꺼줘
        break;
 case 0x13: //Blue
        Enabled = true;
        //blue에 설정된 디지털 핀만 키기
        digitalWrite(blue, HIGH);
        digitalWrite(green, LOW);
        digitalWrite(red, LOW);
        //가습기 가동
        digitalWrite(water, HIGH);
        Serial.println("BLUE");
        break;
      case 0x14: //Red
        Enabled = false;
        //red에 설정된 디지털 핀만 키기
        digitalWrite(blue, LOW);
        digitalWrite(green, LOW);
        digitalWrite(red, HIGH);
        //가습기 중지
        digitalWrite(water, LOW);
        Serial.println("RED");
        break;
      case 0x15: //On
        //t, h 플롯형태로 dht값 담음
        float t = dht.readTemperature();
        float h = dht.readHumidity();
        Serial.println("ON");
        //insertDB 함수 호출
        insertDB(t,h);
        break;
    }
  }
  
  delay(3000);
  /*
  while(ESP_wifi.available() > 0) {
    Serial.write(ESP_wifi.read());
    //insertDB 함수 호출
    insertDB(t,h);
  }*/
}