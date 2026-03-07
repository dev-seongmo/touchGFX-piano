# FreeRTOS 리듬게임 프로젝트

![프로젝트 사진](project_image.png)

STM32 + TouchGFX 기반으로 제작한 **리듬게임 프로젝트**이다.

이 프로젝트의 목적은 다음 두 가지이다.

* **FreeRTOS 태스크 구조 설계 실습**
* **상태머신 기반 UI 구조 구현**

처음에는 **단일 태스크로 전체 게임을 완성**한 후,
RTOS 환경에서 **입력 / 게임 로직 / 렌더링을 분리**하는 방식으로 구조를 확장하였다.

---

# 주요 기능

* 곡 선택 화면
* 리듬게임 플레이 화면
* 결과 화면
* 버튼 입력 기반 게임 플레이
* 실시간 판정 및 점수 계산

---

# 사용 기술

**Hardware**

* STM32
* LCD
* 버튼 입력

**Software**

* FreeRTOS
* TouchGFX
* C / C++

핵심 실습 포인트

* RTOS 태스크 설계
* 상태 머신 기반 UI 흐름
* 입력 / 게임 로직 / 렌더링 역할 분리

---

# 시스템 구조

## 상태머신

```text
[곡 선택 화면]
      |
      | START
      v
[게임 화면]
      |
      | 게임 종료
      v
[결과 화면]
      |
      | CONFIRM / BACK
      v
[곡 선택 화면]
```

전체 시스템은 **상태 머신 중심으로 동작**한다.

---

## RTOS 태스크 구조

```text
+------------------+
|   Input Task     |
|  button polling  |
+--------+---------+
         |
         | ButtonEvent Queue
         v
+------------------+
| Main/Game Task   |
| state + judgment |
+--------+---------+
         |
         | RenderData Queue
         v
+------------------+
|   Render Task    |
|    LCD / UI      |
+------------------+
```

---

# RTOS 태스크 설계

## 태스크 리스트

* Input Task
* Main/Game Task
* Render Task

---

## Input Task

역할

* 버튼 폴링
* debounce 처리
* edge detect
* 버튼 이벤트 생성
* Queue로 Main/Game Task에 전달

예시 이벤트

```c
typedef enum {
    BTN_EVENT_NONE,
    BTN_EVENT_UP_PRESSED,
    BTN_EVENT_DOWN_PRESSED,
    BTN_EVENT_START_PRESSED,
    BTN_EVENT_HIT_1_PRESSED,
    BTN_EVENT_HIT_2_PRESSED
} ButtonEvent;
```

---

## Main/Game Task

역할

* 상태머신 관리
* 곡 선택 / 게임 / 결과 화면 전환
* 입력 이벤트 해석
* 게임 판정 처리
* 점수 / 콤보 계산
* Render Task로 화면 데이터 전달

상태 enum

```c
typedef enum {
    STATE_SONG_SELECT,
    STATE_GAME,
    STATE_RESULT
} AppState;
```

상태별 동작

**SONG_SELECT**

* UP / DOWN : 곡 이동
* START : 게임 시작

**GAME**

* HIT 버튼 : 판정 처리
* 점수 / 콤보 계산
* 게임 종료 → RESULT

**RESULT**

* START / CONFIRM → SONG_SELECT

---

## Render Task

역할

* 곡 선택 화면 출력
* 게임 화면 출력
* 결과 화면 출력

원칙

* 상태 결정은 하지 않음
* 출력만 담당

---

# 핵심 구조 요약

```text
Input Task -> Main/Game Task -> Render Task
```

입력 / 로직 / 출력 역할을 분리하여
RTOS 환경에서도 **안정적인 게임 구조**를 만들 수 있다.

---

# 설계 포인트

이 구조는 다음을 고려하여 설계하였다.

* 단일 태스크 구조에서 자연스럽게 확장 가능
* 태스크 수를 최소화하여 디버깅 용이
* 입력 / 판정 / 렌더링 역할 분리
* 이후 Game Logic Task 추가 확장 가능

즉, 현재 단계에서는 **실습과 확장성 모두를 고려한 RTOS 구조**이다.

---

# 한 줄 정리

**단일 태스크 리듬게임 → FreeRTOS 태스크 구조로 확장한 프로젝트**
