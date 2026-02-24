#include <gui/play_view_screen/play_viewView.hpp>
#include <stdint.h>

extern "C" volatile uint32_t g_song_ms;
extern "C" volatile uint8_t  g_song_playing;

static const int32_t HIT_WINDOW_MS = 300;

static int combo = 0;
static Unicode::UnicodeChar comboBuf[16];
static Unicode::UnicodeChar judgeBuf[512];

struct NoteEvent { uint8_t lane; uint32_t hit_ms; };

static const NoteEvent star[] = {

    {1,4000},{1,4500},{5,5000},{5,5500},{6,6000},{6,6500},{5,7500},

    {4,8000},{4,8500},{3,9000},{3,9500},{2,10000},{2,10500},{1,11500},

    {5,12000},{5,12500},{4,13000},{4,13500},{3,14000},{3,14500},{2,15500},

    {5,16000},{5,16500},{4,17000},{4,17500},{3,18000},{3,18500},{2,19500},

    {1,20000},{1,20500},{5,21000},{5,21500},{6,22000},{6,22500},{5,23500},

    {4,24000},{4,24500},{3,25000},{3,25500},{2,26000},{2,26500},{1,27500}
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

    const uint32_t now = g_song_ms;

    // note 위젯 포인터 배열
    touchgfx::Widget* w[10] = {
        &note_0,&note_1,&note_2,&note_3,&note_4,
        &note_5,&note_6,&note_7,&note_8,&note_9
    };

    // lane -> x 좌표
    const int laneX[9] = {0,0, 40, 80, 120, 160, 200, 240, 280};

    // ---------- 입력(하이라이트) 에지 감지 ----------
    static uint8_t prev1 = 0, prev2 = 0, prev3 = 0, prev4 = 0;
    static uint8_t prev5 = 0, prev6 = 0, prev7 = 0, prev8 = 0;

    uint8_t cur1 = highlight1_visible;
    uint8_t cur2 = highlight2_visible;
    uint8_t cur3 = highlight3_visible;
    uint8_t cur4 = highlight4_visible;
    uint8_t cur5 = highlight5_visible;
    uint8_t cur6 = highlight6_visible;
    uint8_t cur7 = highlight7_visible;
    uint8_t cur8 = highlight8_visible;

    // 표시 갱신
    if (prev1 != cur1) { highlight_1.setVisible(cur1); highlight_1.invalidate(); }
    if (prev2 != cur2) { highlight_2.setVisible(cur2); highlight_2.invalidate(); }
    if (prev3 != cur3) { highlight_3.setVisible(cur3); highlight_3.invalidate(); }
    if (prev4 != cur4) { highlight_4.setVisible(cur4); highlight_4.invalidate(); }
    if (prev5 != cur5) { highlight_5.setVisible(cur5); highlight_5.invalidate(); }
    if (prev6 != cur6) { highlight_6.setVisible(cur6); highlight_6.invalidate(); }
    if (prev7 != cur7) { highlight_7.setVisible(cur7); highlight_7.invalidate(); }
    if (prev8 != cur8) { highlight_8.setVisible(cur8); highlight_8.invalidate(); }

    // ---------- 1) 스폰 ----------
    while (nextIdx < STAR_LEN)
    {
        uint32_t hit   = star[nextIdx].hit_ms;
        uint32_t spawn = (hit > travel_ms) ? (hit - travel_ms) : 0;

        if (now < spawn) break;

        // 빈 슬롯 찾기
        int slot = -1;
        for (int i=0;i<10;i++)
        {
            if (!pool[i].active) { slot = i; break; }
        }
        if (slot < 0) break; // 풀 꽉 참

        pool[slot].active = true;
        pool[slot].lane   = star[nextIdx].lane;
        pool[slot].hit_ms = hit;

        int x = laneX[pool[slot].lane];
        w[slot]->moveTo(x, y_spawn);
        w[slot]->setVisible(true);
        w[slot]->invalidate();

        nextIdx++;
    }
    bool missedThisTick = false;
    // ---------- 2) 이동 + 자동 MISS ----------
    for (int i=0;i<10;i++)
    {
        if (!pool[i].active) continue;

        uint32_t hit = pool[i].hit_ms;

        if ((int32_t)now - (int32_t)hit > HIT_WINDOW_MS)
{
    pool[i].active = false;
    w[i]->setVisible(false);
    w[i]->invalidate();

    missedThisTick = true;   // ✅ 표시/콤보는 나중에 한 번만
    continue;
}
        uint32_t spawn = (hit > travel_ms) ? (hit - travel_ms) : 0;

int32_t dt = (int32_t)(now - spawn);
if (dt < 0) dt = 0;

// dt가 travel_ms보다 크면 hit 이후 구간
int y;
if (dt <= (int32_t)travel_ms)
{
    // spawn -> hit
    y = y_spawn + (int)((int32_t)(y_hit - y_spawn) * dt / (int32_t)travel_ms);
}
else
{
    // hit 이후에도 계속 내려가게 (속도 동일하게)
    int32_t dt2 = dt - (int32_t)travel_ms;     // hit 이후 경과시간
    y = y_hit + (int)((int32_t)(y_hit - y_spawn) * dt2 / (int32_t)travel_ms);
}

        w[i]->invalidate();
        w[i]->moveTo(laneX[pool[i].lane], y);
        w[i]->invalidate();

        // 화면 정리용
        if (y > y_kill)
        {
            pool[i].active = false;
            w[i]->setVisible(false);
            w[i]->invalidate();
        }
    }

    // ---------- 3) 입력 판정 (0 -> 1 에지) ----------
    if (prev1 == 0 && cur1 == 1) checkLaneInput(1);
    if (prev2 == 0 && cur2 == 1) checkLaneInput(2);
    if (prev3 == 0 && cur3 == 1) checkLaneInput(3);
    if (prev4 == 0 && cur4 == 1) checkLaneInput(4);
    if (prev5 == 0 && cur5 == 1) checkLaneInput(5);
    if (prev6 == 0 && cur6 == 1) checkLaneInput(6);
    if (prev7 == 0 && cur7 == 1) checkLaneInput(7);
    if (prev8 == 0 && cur8 == 1) checkLaneInput(8);

    // 마지막 상태 저장
    prev1 = cur1; prev2 = cur2; prev3 = cur3; prev4 = cur4;
    prev5 = cur5; prev6 = cur6; prev7 = cur7; prev8 = cur8;
    if (missedThisTick)
{
    combo = 0;
    Unicode::snprintf(comboBuf, 16, "%d", combo);
    combo_text.setWildcard(comboBuf);
    combo_text.invalidate();

    updateJudgeText("MISS");
}
}

void play_viewView::setupScreen()
{
    play_viewViewBase::setupScreen();
    combo = 0;
    Unicode::snprintf(comboBuf, 16, "%d", combo);
    combo_text.setWildcard(comboBuf);
    combo_text.invalidate();

    updateJudgeText("READY");
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


//판정함수
void play_viewView::checkLaneInput(uint8_t lane)
{
    const uint32_t now = g_song_ms;

    touchgfx::Widget* w[10] = { 
        &note_0,&note_1,&note_2,&note_3,&note_4,
        &note_5,&note_6,&note_7,&note_8,&note_9 
    };

    int bestIdx = -1;
    int32_t bestAbs = 0x7FFFFFFF;

    for (int i = 0; i < 10; i++)
    {
        if (!pool[i].active) continue;
        if (pool[i].lane != lane) continue;

        int32_t diff = (int32_t)now - (int32_t)pool[i].hit_ms;
        int32_t ad = diff < 0 ? -diff : diff;

        if (ad < bestAbs)
        {
            bestAbs = ad;
            bestIdx = i;
        }
    }

    if (bestIdx >= 0 && bestAbs <= HIT_WINDOW_MS)
    {
        const int32_t diff = (int32_t)now - (int32_t)pool[bestIdx].hit_ms;
        const int32_t ad   = diff < 0 ? -diff : diff;

        const char* judge = "GOOD";
        if (ad <= 80)       judge = "PERFECT";
        else if (ad <= 160) judge = "GREAT";

        pool[bestIdx].active = false;
        w[bestIdx]->setVisible(false);
        w[bestIdx]->invalidate();

        combo++;

        Unicode::snprintf(comboBuf, 16, "%d", combo);
        combo_text.setWildcard(comboBuf);
        combo_text.invalidate();

        updateJudgeText(judge);
    }
    else
    {
        combo = 0;

        Unicode::snprintf(comboBuf, 16, "%d", combo);
        combo_text.setWildcard(comboBuf);
        combo_text.invalidate();

        updateJudgeText("MISS");
    }
}


void play_viewView::updateJudgeText(const char* txt)
{
    Unicode::snprintf(judgeBuf, 512, "%s", txt);
    judge_text.setWildcard(judgeBuf);
    judge_text.invalidate();
}