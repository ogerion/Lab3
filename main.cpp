#include <QApplication>
#include <sequence/LinkedListSequence.h>
#include <iostream>
#include <string>
#include"UnorderedMap.h"
#include "UI/mainwindow.h"
#include "Game.h"
#include "IDictionary.h"
#include "tests/testIDictionary.h"
#include "testGame.h"
int main(int argc, char *argv[])
{
    testGameBot();
    testIDictionary();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
