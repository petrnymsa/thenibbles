//
// Created by zirko on 5/22/16.
//

#include "IScreen.hpp"


IScreen::IScreen(GameEngine &gameEngine)
        : engine(gameEngine), msgDelay(25), msgTime(0), currentMsg("", COLOR_WHITE), bufferActive(false){
}


IScreen::~IScreen() {

}


void IScreen::AddMessage(const string &msg, short color) {
    if (currentMsg.first.empty() && currentMsg.first.empty()) {
       msgTime = msgDelay;
    }
    messages.push(make_pair(msg, color));
}


void IScreen::AddToBuffer(const Point &point, char printChar, short color) {
    drawBuffer.push(BufferPart(point, printChar, color));
}


void IScreen::AddToBuffer(const BufferPart &part) {
    drawBuffer.push(part);
}


bool IScreen::BufferInitialized() const {
    return bufferActive;
}

void IScreen::ChangeBufferState(bool state) {
    bufferActive = state;
}


/* --------  PROTECTED -------------- */


void IScreen::ClearMsgWin(WINDOW *win) {
    wclear(win);
    wrefresh(win);
    touchwin(win);
    box(win, 0, 0);
    wrefresh(win);
    currentMsg.first.clear();
    currentMsg.second = COLOR_WHITE;

    while (!messages.empty())
        messages.pop();
}

void IScreen::DrawMessages(WINDOW *win) {
    if (!currentMsg.first.empty()) {
        touchwin(win);
        wmove(win, 1, 1);
        init_pair(currentMsg.second, currentMsg.second, COLOR_BLACK);
        wattron(win, COLOR_PAIR(currentMsg.second));
        wprintw(win, currentMsg.first.c_str());
        wrefresh(win);
        wattroff(win, COLOR_PAIR(currentMsg.second));

        box(win, 0, 0);
        wrefresh(win);
    }

}


void IScreen::DrawBuffer(WINDOW *win) {
    while (!drawBuffer.empty()) {
        BufferPart part = drawBuffer.front();
        drawBuffer.pop();
        touchwin(win);
        wmove(win, part.point.y, part.point.x);
        init_pair(part.color, part.color, COLOR_BLACK);
        wattron(win, COLOR_PAIR(part.color));
        waddch(win, part.ch);
        wrefresh(win);
        wattroff(win, COLOR_PAIR(part.color));
    }
}


void IScreen::UpdateMessages(WINDOW *win) {
    if (!messages.empty()) {
        msgTime++;
    }
    if (msgTime >= msgDelay && !messages.empty()) {
        wclear(win);
        wrefresh(win);
        currentMsg = messages.front();
        messages.pop();
        msgTime = 0;
    }
}



