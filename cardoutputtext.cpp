#include "cardoutputtext.h"
#include <Qstring>
#include <QDebug>



//this class is for qdebug text
CardOutputText::CardOutputText()
{

}



void CardOutputText::showDealAndDraw(int icardID, int iwhosTurn)
{
    m_icardID = icardID;
    char ccardNumberDisplay[Card_Number::CARDS_13+1] = {"A23456789TJQK"};
    char cnameShow[TOTAL_PLAYER][MAX_STRINGLENGTH] = {
        {"Player"},{"Com1"},{"Com2"},{"Dealer"}
    };
        switch(m_icardID/Card_Number::CARDS_13){
        case Card_Color::HEART:
            qDebug()<<cnameShow[iwhosTurn] <<"have draw :Heart"<<ccardNumberDisplay[m_icardID%Card_Number::CARDS_13];
            break;
        case Card_Color::SPADE:
            qDebug()<<cnameShow[iwhosTurn]<<"have draw :Spade"<<ccardNumberDisplay[m_icardID%Card_Number::CARDS_13];
            break;
        case Card_Color::CLUB:
            qDebug()<<cnameShow[iwhosTurn]<<"have draw :Club"<<ccardNumberDisplay[m_icardID%Card_Number::CARDS_13];
            break;
        case Card_Color::DIAMOND:
            qDebug()<<cnameShow[iwhosTurn]<<"have draw :Diamond"<<ccardNumberDisplay[m_icardID%Card_Number::CARDS_13];
            break;
        }
}



