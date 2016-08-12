# RC-Servo-Tester-
DIY RC Servo Tester (STM32F0 &amp; LABVIEW GUI)

http://www.mylv.net/opensource/opensource_view.aspx?BID=96435


<object width="845" height="510">
<iframe width="1280" height="720" src="https://www.youtube.com/embed/h2mnOkbDfCk" frameborder="0" allowfullscreen></iframe>
</object>


http://youtu.be/h2mnOkbDfCk

<img src="https://github.com/mocona05/RC-Servo-Tester-/blob/master/1.png">

<img src="https://github.com/mocona05/RC-Servo-Tester-/blob/master/2.png">

<img src="https://github.com/mocona05/RC-Servo-Tester-/blob/master/3.png">

<img src="https://github.com/mocona05/RC-Servo-Tester-/blob/master/4.png">

<img src="https://github.com/mocona05/RC-Servo-Tester-/blob/master/5.png">


집어넣은 기능은 아래와 같습니다.
 1. DIGITAL SERVO MODE  지원(4ms 주기) : 
 주로 쿼드 콥터용 변속기에서 빠른 응답성이 필요해서 순정 변속기를 DIGITAL SERVO MODE 펌웨어로 펍업해서 쓰거나 전용 DIGITAL SERVO MODE로 빠른 펄스 속도를 지원하는 변속기를 구매해서 쓰고있죠.
  볼륨을 최소로 놓고 PUSH BUTTON을 2초이상 누르면 LED3개가 동시에 4번 깜빡이고 DIGITAL SERVO MODE 로 변경됩니다.
   일반 PPM MODE(20ms 펄스주기)로 변경할때에는 볼륨을 최대로 놓고 PUSH BUTTON을 2초이상 누르면 LED가 좌->우->좌 3번 왔다갔다 점멸 하면서 PPM MODE로 변경됩니다.

 2. 서보 테스터 최대 최소 범위 가변 기능 :
볼륨을 중심에 놓고(중심에 볼륨을 놓으면 메뉴얼 모드에서 3개의 LED가 모두 점등되면 중심입니다.) PUSH BUTTON을 2초이상 길게 누르면 PPPM 출력펄스가 최대 최소 값이 변경됩니다.
표준 상태에서는 1.0ms ~ 2.0ms의 HIGH WIDTH PULSE 출력되며, 넓은범위 상태에서는 0.8ms ~ 2.2ms의 펄스 HIGH WIDTH PULSE가 출력되게 됩니다.
3. 가변 자동모드 :
PUSH 버튼을 잛게 누르면 수동제어->중심고정->자동모드 로 동작 모드로 변경됩니다.
자동모드로 설정될경우 맨 우측 LED가 점등되며, 자동모드 상태에서 볼륨을 돌려주면 자동모드로 움직이는 속도가 가변됩니다. (자동모드는 서보테스터에서 최소 PPM PULSE 에서 최대 PPM PULSE가 까지 오르락 내리락 하면서 신호가 출력됩니다.)
턴니지 서보테스터에는 자동 모드는 한가지 속도로 고정되죠
.
4. 출력되는 PPM PULSE의 포지션을 알려주는 LED기능
 넓은 범위 출력으로 서보테스터를 셋팅해서 사용시 PPM PULSE가 1.0ms 포지보다 낮아지면 맨 좌측 LED의 밝기가 어두어 집니다. 반대로 2.0ms 보다 크면 맨 우측 LED가 어두워 지고 중앙(1.498~1.502ms)에 위치할경우 3개의 모든 LED가 들어오게 됩니다.
LED 밝기와 점등 갯수를 보고 서보테스터에서 출력되는 펄스가 1.0인지 2.0인지 중심인지 알수있도록 하였습니다.

5. 원격제어 기능
USB-UART와 서보테스터의 INPUT 핀을 RXD, TXD와 연결하면 PC의 GUI 프로그램에서 서보테스터를 원격으로 제어 가능합니다. GUI프로그램은 LABVIEW로 만들었으며 첨부된 파일에 만들어진 실행 파일 및 소스가 있습니다.

DIGITAL SERVO MODE 와 STANDARD SERVO(ANALOG SERVO) PPM MODE를 변화하는 동영상 입니다.
아날로그 서보라서 DIGITAL SERVO MODE 를 지원 못해서 변경시 웅웅거리는 잡음이 서보에서 나게 됩니다.
이렇게 오래 쓰면 써보 망가집니다. --;
볼륨을 최대로 놓고 푸쉬버튼을 2초이상 눌주면 표준 PPM MODE이고 볼륨을 최소로 놓고 푸쉬버튼을 2초이상 누르면 DIGITAL SERVO MODE로 변경됩니다.
