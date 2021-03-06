package com.codingame.game.gota;

public enum Direction {
    N(0,-1),
    E(1, 0),
    S(0, 1),
    W(-1, 0);

    int x, y;

    Direction(int x, int y) {
        this.x = x;
        this.y = y;
    }
}
