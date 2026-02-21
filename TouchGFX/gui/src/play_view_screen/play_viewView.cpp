#include <gui/play_view_screen/play_viewView.hpp>
#include <stdint.h>

extern "C" volatile uint32_t g_song_ms;
extern "C" volatile uint8_t  g_song_playing;

struct NoteEvent { uint8_t lane; uint32_t hit_ms; };

static const NoteEvent star[] = {

    {1,4000},{1,5000},{5,6000},{5,7000},{6,8000},{6,9000},{5,10000},

    {4,11000},{4,12000},{3,13000},{3,14000},{2,15000},{2,16000},{1,17000},

    {5,18000},{5,19000},{4,20000},{4,21000},{3,22000},{3,23000},{2,24000},

    {5,25000},{5,26000},{4,27000},{4,28000},{3,29000},{3,30000},{2,31000},

    {1,32000},{1,33000},{5,34000},{5,35000},{6,36000},{6,37000},{5,38000},

    {4,39000},{4,40000},{3,41000},{3,42000},{2,43000},{2,44000},{1,45000}
};
static const int STAR_LEN = sizeof(star)/sizeof(star[0]);

static const int y_hit = 200;
static const int y_spawn = -30;         
static const uint32_t travel_ms = 1200;  
static const int y_kill = 360;          

struct ActiveNote {
    bool active;
    uint8_t lane;
    uint32_t hit_ms;
};

static ActiveNote pool[10];
static int nextIdx = 0;


extern "C" volatile uint8_t highlight1_visible;
extern "C" volatile uint8_t highlight2_visible;
extern "C" volatile uint8_t highlight3_visible;
extern "C" volatile uint8_t highlight4_visible;
extern "C" volatile uint8_t highlight5_visible;
extern "C" volatile uint8_t highlight6_visible;
extern "C" volatile uint8_t highlight7_visible;
extern "C" volatile uint8_t highlight8_visible;


play_viewView::play_viewView()
{

    
}
//ai assisted code becase too slow
void play_viewView::handleTickEvent()
{
    play_viewViewBase::handleTickEvent();
    static uint8_t prev1 = 255;
    static uint8_t prev2 = 255;
    static uint8_t prev3 = 255;
    static uint8_t prev4 = 255;
    static uint8_t prev5 = 255;
    static uint8_t prev6 = 255;
    static uint8_t prev7 = 255;
    static uint8_t prev8 = 255;

    if (prev1 != highlight1_visible) {
        prev1 = highlight1_visible;
        highlight_1.setVisible(prev1);
        highlight_1.invalidate();
    }

    if (prev2 != highlight2_visible) {
        prev2 = highlight2_visible;
        highlight_2.setVisible(prev2);
        highlight_2.invalidate();
    }

    if (prev3 != highlight3_visible) {
        prev3 = highlight3_visible;
        highlight_3.setVisible(prev3);
        highlight_3.invalidate();
    }

    if (prev4 != highlight4_visible) {
        prev4 = highlight4_visible;
        highlight_4.setVisible(prev4);
        highlight_4.invalidate();
    }

    if (prev5 != highlight5_visible) {
        prev5 = highlight5_visible;
        highlight_5.setVisible(prev5);
        highlight_5.invalidate();
    }

    if (prev6 != highlight6_visible) {
        prev6 = highlight6_visible;
        highlight_6.setVisible(prev6);
        highlight_6.invalidate();
    }

    if (prev7 != highlight7_visible) {
        prev7 = highlight7_visible;
        highlight_7.setVisible(prev7);
        highlight_7.invalidate();
    }

    if (prev8 != highlight8_visible) {
        prev8 = highlight8_visible;
        highlight_8.setVisible(prev8);
        highlight_8.invalidate();
    }
    const uint32_t now = g_song_ms;

    // note 위젯 포인터 배열
    touchgfx::Widget* w[10] = { &note_0,&note_1,&note_2,&note_3,&note_4,&note_5,&note_6,&note_7,&note_8,&note_9 };

    // lane -> x 좌표 (8레인 가정, 화면 240 기준 대충 균등배치)
    const int laneX[9] = {0,0, 40, 80, 120, 160, 200, 240, 280};

    // 1) 스폰: now가 (hit_ms - travel_ms) 지나면 풀에서 빈 슬롯 하나 활성화
    while (nextIdx < STAR_LEN)
    {
        uint32_t hit = star[nextIdx].hit_ms;
        uint32_t spawn = (hit > travel_ms) ? (hit - travel_ms) : 0;

        if (now < spawn) break;

        // 빈 슬롯 찾기
        int slot = -1;
        for (int i=0;i<10;i++) { if (!pool[i].active) { slot = i; break; } }
        if (slot < 0) break; // 풀 꽉 참 -> 다음 tick에

        pool[slot].active = true;
        pool[slot].lane   = star[nextIdx].lane;
        pool[slot].hit_ms = hit;

        // 초기 위치 세팅
        int x = laneX[pool[slot].lane];
        w[slot]->moveTo(x, y_spawn);
        w[slot]->setVisible(true);
        w[slot]->invalidate();

        nextIdx++;
    }

    // 2) 이동/삭제
    for (int i=0;i<10;i++)
    {
        if (!pool[i].active) continue;

        uint32_t hit = pool[i].hit_ms;
        uint32_t spawn = (hit > travel_ms) ? (hit - travel_ms) : 0;

        // progress [0..1]
        int32_t dt = (int32_t)(now - spawn);
        int y = y_spawn + (int)((int32_t)(y_hit - y_spawn) * dt / (int32_t)travel_ms);
        // 이전 영역 지우고 이동
        w[i]->invalidate();
        w[i]->moveTo(laneX[pool[i].lane], y);
        w[i]->invalidate();

        // 화면 아래로 충분히 내려가면 비활성화
        if (y > y_kill)
{
    pool[i].active = false;
    w[i]->setVisible(false);
    w[i]->invalidate();
}
    }
}

void play_viewView::setupScreen()
{
    play_viewViewBase::setupScreen();

    g_song_playing = 1;
    nextIdx = 0;

    // 풀 초기화 + 전부 숨김
    for (int i=0;i<10;i++) {
        pool[i] = {false, 0, 0};
    }

    // note_0..note_9 숨기기
    note_0.setVisible(false); note_0.invalidate();
    note_1.setVisible(false); note_1.invalidate();
    note_2.setVisible(false); note_2.invalidate();
    note_3.setVisible(false); note_3.invalidate();
    note_4.setVisible(false); note_4.invalidate();
    note_5.setVisible(false); note_5.invalidate();
    note_6.setVisible(false); note_6.invalidate();
    note_7.setVisible(false); note_7.invalidate();
    note_8.setVisible(false); note_8.invalidate();
    note_9.setVisible(false); note_9.invalidate();
}

void play_viewView::tearDownScreen()
{
    g_song_playing = 0;
    play_viewViewBase::tearDownScreen();
}