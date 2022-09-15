#ifndef CARDOUTPUTTEXT_H
#define CARDOUTPUTTEXT_H

#define TOTAL_PLAYER 4
#define MAX_STRINGLENGTH 10


enum Card_Number{
    ACE = 0,TWO,THREE,
    FOUR,FIVE,SIX,
    SEVEN,EIGHT,NINE,
    TEN,JACK,QUEEN,
    KING,CARDS_13 = 13
};
enum Card_Deal{
    FIRSTCARD = 1,
    SECONDCARD,
    THIRDCARD,
    FOURTHCARD,
    FIFTHCARD,
    SIXTHCARD,
    SEVENTHCARD,
    EIGHTTHCARD
};

enum Card_Color{
    HEART,
    SPADE,
    CLUB,
    DIAMOND
};

enum Game_State{
    PLAYERTURN,
    COM1TURN,
    COM2TURN,
    DEALERTURN
};

class CardOutputText
{
public:
    CardOutputText();


    void showDealAndDraw(int icardID, int iwhosTurn);
private:
           //used to output from A to K
    int m_icardID;                             //used to count card's Type and Number
    int m_itotalDraw;                              //record total number of draw
};

#endif // CARDOUTPUTTEXT_H
