#include <gui/play_view_screen/play_viewView.hpp>

#include <cstring>
#include <touchgfx/Color.hpp>

namespace
{
static const int32_t HIT_WINDOW_MS = 300;
static const int Y_HIT = 200;
static const int Y_SPAWN = -30;
static const uint32_t TRAVEL_MS = 1200;
static const int Y_KILL = 360;
static const uint8_t MAX_ACTIVE_NOTES = 10;

struct ActiveNote
{
    bool active;
    uint8_t lane;
    uint32_t hit_ms;
};

static const SongChart* currentSong = 0;
static ActiveNote activeNotes[MAX_ACTIVE_NOTES];
static uint16_t nextSongIndex = 0;
static uint8_t previousInputMask = 0;
static int combo = 0;
static bool finishPending = false;
static uint32_t finishStartMs = 0;

static bool hasActiveNotes()
{
    for (uint8_t i = 0; i < MAX_ACTIVE_NOTES; i++)
    {
        if (activeNotes[i].active)
        {
            return true;
        }
    }

    return false;
}
}

play_viewView::play_viewView()
{
    std::memset(judgeBuf, 0, sizeof(judgeBuf));
    judge_text.setWildcard(judgeBuf);
}

void play_viewView::handleTickEvent()
{
    play_viewViewBase::handleTickEvent();

    const uint32_t now = presenter->getSongMs();
    const uint8_t inputMask = presenter->getInputMask();

    touchgfx::Widget* widgets[MAX_ACTIVE_NOTES] = {
        &note_0, &note_1, &note_2, &note_3, &note_4,
        &note_5, &note_6, &note_7, &note_8, &note_9
    };

    touchgfx::Box* highlights[8] = {
        &highlight_1, &highlight_2, &highlight_3, &highlight_4,
        &highlight_5, &highlight_6, &highlight_7, &highlight_8
    };

    for (uint8_t i = 0; i < 8; i++)
    {
        const bool currentVisible = ((inputMask & (1u << i)) != 0u);
        const bool previousVisible = ((previousInputMask & (1u << i)) != 0u);

        if (currentVisible != previousVisible)
        {
            highlights[i]->setVisible(currentVisible);
            highlights[i]->invalidate();
        }
    }

    while ((currentSong != 0) && (nextSongIndex < currentSong->note_count))
    {
        const uint32_t hitMs = currentSong->notes[nextSongIndex].hit_ms;
        const uint32_t spawnMs = (hitMs > TRAVEL_MS) ? (hitMs - TRAVEL_MS) : 0u;

        if (now < spawnMs)
        {
            break;
        }

        int slot = -1;
        for (uint8_t i = 0; i < MAX_ACTIVE_NOTES; i++)
        {
            if (!activeNotes[i].active)
            {
                slot = i;
                break;
            }
        }

        if (slot < 0)
        {
            break;
        }

        activeNotes[slot].active = true;
        activeNotes[slot].lane = currentSong->notes[nextSongIndex].lane;
        activeNotes[slot].hit_ms = hitMs;

        widgets[slot]->moveTo(getLaneX(activeNotes[slot].lane), Y_SPAWN);
        widgets[slot]->setVisible(true);
        widgets[slot]->invalidate();

        nextSongIndex++;
    }

    uint8_t missedCount = 0;
    for (uint8_t i = 0; i < MAX_ACTIVE_NOTES; i++)
    {
        if (!activeNotes[i].active)
        {
            continue;
        }

        const uint32_t hitMs = activeNotes[i].hit_ms;

        if ((int32_t)now - (int32_t)hitMs > HIT_WINDOW_MS)
        {
            activeNotes[i].active = false;
            widgets[i]->setVisible(false);
            widgets[i]->invalidate();
            missedCount++;
            continue;
        }

        const uint32_t spawnMs = (hitMs > TRAVEL_MS) ? (hitMs - TRAVEL_MS) : 0u;
        int32_t dt = (int32_t)(now - spawnMs);
        int y = 0;

        if (dt < 0)
        {
            dt = 0;
        }

        if (dt <= (int32_t)TRAVEL_MS)
        {
            y = Y_SPAWN + (int)((int32_t)(Y_HIT - Y_SPAWN) * dt / (int32_t)TRAVEL_MS);
        }
        else
        {
            const int32_t dtAfterHit = dt - (int32_t)TRAVEL_MS;
            y = Y_HIT + (int)((int32_t)(Y_HIT - Y_SPAWN) * dtAfterHit / (int32_t)TRAVEL_MS);
        }

        widgets[i]->invalidate();
        widgets[i]->moveTo(getLaneX(activeNotes[i].lane), y);
        widgets[i]->invalidate();

        if (y > Y_KILL)
        {
            activeNotes[i].active = false;
            widgets[i]->setVisible(false);
            widgets[i]->invalidate();
        }
    }

    if (missedCount > 0u)
    {
        combo = 0;
        updateComboText();
        updateJudgeText("MISS");

        for (uint8_t i = 0; i < missedCount; i++)
        {
            presenter->addMiss();
        }
    }

    const uint8_t risingEdges = (uint8_t)(inputMask & (uint8_t)(~previousInputMask));
    for (uint8_t i = 0; i < 8; i++)
    {
        if ((risingEdges & (1u << i)) != 0u)
        {
            checkLaneInput((uint8_t)(i + 1u));
        }
    }

    previousInputMask = inputMask;

    if ((currentSong != 0) && (currentSong->note_count > 0))
    {
        const uint32_t lastHitMs = currentSong->notes[currentSong->note_count - 1].hit_ms;
        const bool songFinished = (nextSongIndex >= currentSong->note_count) &&
                                  !hasActiveNotes() &&
                                  (now > (lastHitMs + HIT_WINDOW_MS));

        if (songFinished)
        {
            if (!finishPending)
            {
                finishPending = true;
                finishStartMs = now;
            }
            else if ((now - finishStartMs) >= 3000u)
            {
                presenter->stopSong();
                application().gotoScore_viewScreenNoTransition();
            }
        }
        else
        {
            finishPending = false;
        }
    }
}

void play_viewView::setupScreen()
{
    play_viewViewBase::setupScreen();

    currentSong = &getTwinkleTwinkleSong();
    nextSongIndex = 0;
    previousInputMask = 0;
    combo = 0;
    finishPending = false;
    finishStartMs = 0;

    presenter->setSoundProfile(RHYTHM_SOUND_TWINKLE);
    presenter->resetScore();
    std::memset(judgeBuf, 0, sizeof(judgeBuf));
    judge_text.setWildcard(judgeBuf);
    updateComboText();
    updateJudgeText("READY");

    for (uint8_t i = 0; i < MAX_ACTIVE_NOTES; i++)
    {
        activeNotes[i].active = false;
        activeNotes[i].lane = 0;
        activeNotes[i].hit_ms = 0;
    }

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

    highlight_1.setVisible(false); highlight_1.invalidate();
    highlight_2.setVisible(false); highlight_2.invalidate();
    highlight_3.setVisible(false); highlight_3.invalidate();
    highlight_4.setVisible(false); highlight_4.invalidate();
    highlight_5.setVisible(false); highlight_5.invalidate();
    highlight_6.setVisible(false); highlight_6.invalidate();
    highlight_7.setVisible(false); highlight_7.invalidate();
    highlight_8.setVisible(false); highlight_8.invalidate();

    presenter->startSong();
}

void play_viewView::tearDownScreen()
{
    presenter->stopSong();
    play_viewViewBase::tearDownScreen();
}

void play_viewView::checkLaneInput(uint8_t lane)
{
    const uint32_t now = presenter->getSongMs();
    touchgfx::Widget* widgets[MAX_ACTIVE_NOTES] = {
        &note_0, &note_1, &note_2, &note_3, &note_4,
        &note_5, &note_6, &note_7, &note_8, &note_9
    };
    int bestIdx = -1;
    int32_t bestAbs = 0x7FFFFFFF;

    for (uint8_t i = 0; i < MAX_ACTIVE_NOTES; i++)
    {
        if (!activeNotes[i].active || activeNotes[i].lane != lane)
        {
            continue;
        }

        const int32_t diff = (int32_t)now - (int32_t)activeNotes[i].hit_ms;
        const int32_t absDiff = (diff < 0) ? -diff : diff;

        if (absDiff < bestAbs)
        {
            bestAbs = absDiff;
            bestIdx = i;
        }
    }

    if ((bestIdx >= 0) && (bestAbs <= HIT_WINDOW_MS))
    {
        const int32_t diff = (int32_t)now - (int32_t)activeNotes[bestIdx].hit_ms;
        const int32_t absDiff = (diff < 0) ? -diff : diff;
        const char* judge = "GOOD";

        if (absDiff <= 80)
        {
            judge = "PERFECT";
            presenter->addPerfect();
        }
        else if (absDiff <= 160)
        {
            judge = "GREAT";
            presenter->addGreat();
        }
        else
        {
            presenter->addGood();
        }

        activeNotes[bestIdx].active = false;
        widgets[bestIdx]->setVisible(false);
        widgets[bestIdx]->invalidate();

        combo++;
        updateComboText();
        updateJudgeText(judge);
    }
    else
    {
        combo = 0;
        updateComboText();
        updateJudgeText("MISS");
        presenter->addMiss();
    }
}

void play_viewView::updateComboText()
{
    combo_text.invalidate();
    Unicode::snprintf(combo_textBuffer, COMBO_TEXT_SIZE, "%d", combo);
    combo_text.resizeToCurrentText();
    combo_text.invalidate();
}

void play_viewView::updateJudgeText(const char* txt)
{
    judge_text.invalidate();
    setJudgeColor(txt);
    std::memset(judgeBuf, 0, sizeof(judgeBuf));
    Unicode::fromUTF8(reinterpret_cast<const uint8_t*>(txt), judgeBuf, sizeof(judgeBuf) / sizeof(judgeBuf[0]));
    judge_text.setWildcard(judgeBuf);
    judge_text.setAlpha(255);
    judge_text.resizeToCurrentText();
    judge_text.invalidate();
}

void play_viewView::setJudgeColor(const char* txt)
{
    if (std::strcmp(txt, "PERFECT") == 0)
    {
        judge_text.setColor(touchgfx::Color::getColorFromRGB(0, 251, 255));
    }
    else if (std::strcmp(txt, "GREAT") == 0)
    {
        judge_text.setColor(touchgfx::Color::getColorFromRGB(0, 255, 64));
    }
    else if (std::strcmp(txt, "GOOD") == 0)
    {
        judge_text.setColor(touchgfx::Color::getColorFromRGB(255, 230, 0));
    }
    else if (std::strcmp(txt, "MISS") == 0)
    {
        judge_text.setColor(touchgfx::Color::getColorFromRGB(194, 194, 194));
    }
    else
    {
        judge_text.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    }
}

int play_viewView::getLaneX(uint8_t lane) const
{
    static const int laneX[9] = {0, 0, 40, 80, 120, 160, 200, 240, 280};
    return laneX[lane];
}
