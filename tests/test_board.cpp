#include <gtest/gtest.h>
#include "../board/board.h"

class BoardTest : public ::testing::Test {
protected:
    Board board;

    void SetUp() override {
        board = Board();
    }

    void TearDown() override {
    }
};


TEST_F(BoardTest, CheckInitialState) {
    EXPECT_EQ(board.check_win(), 0);
}

TEST_F(BoardTest, DetectaPeligroInminente) {

    board.putPos(1,true);


    board.putPos(8, false);
    board.putPos(2, true);

    // AHORA: El tablero está así:
    //  X  X  .
    //  O  .  .
    //  .  .  .
    // Es el turno de O. O debería estar ATERRADO.

    int puntaje = board.evaluate(false);

    // 3. ASSERT: El puntaje no puede ser 0
    std::cout << "Puntaje devuelto: " << puntaje << std::endl;

    // Si el puntaje es 0, el bot cree que está seguro -> ¡BUG ENCONTRADO!
    EXPECT_NE(puntaje, 0) << "El bot cree que la situacion es neutra, pero va a perder.";
}

